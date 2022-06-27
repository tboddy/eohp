// bullets

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "enemies.h"
#include "bullets.h"
#include "players.h"
#include "bricks.h"
#include "chrome.h"

// lifecycle

void spawnBullet(struct bulletSpawner spawner, void(*updater)){
	s16 i = -1;
	for(s16 j = 0; j < BULLET_COUNT; j++) if(!bullets[j].active && i == -1) i = j;
	if(i > -1 && (spawner.player || (players[spawner.p2 ? 1 : 0].invincibleClock < INVINCIBLE_MID))){
		bullets[i].active = TRUE;
		bullets[i].pos.x = spawner.x;
		bullets[i].pos.y = spawner.y;
		bullets[i].off.x = FIX16(spawner.big ? 8 : 4);
		bullets[i].off.y = bullets[i].off.x;
		bullets[i].dist = fix16ToFix32(bullets[i].off.x);
		bullets[i].speed = spawner.speed;
		bullets[i].hitClock = 0;
		bullets[i].clock = 0;
		bullets[i].angle = spawner.angle;
		bullets[i].player = spawner.player;
		bullets[i].p2 = spawner.p2;
		bullets[i].used = FALSE;
		for(u8 j = 0; j < B_COUNT_INT; j++){
			bullets[i].bools[j] = spawner.bools[j];
			bullets[i].ints[j] = spawner.ints[j];
			bullets[i].fixes[j] = spawner.fixes[j];
		}
		if(spawner.vel.x && spawner.vel.y){
			bullets[i].vel.x = spawner.vel.x;
			bullets[i].vel.y = spawner.vel.y;
			if(bullets[i].vel.x > 0 && !bullets[i].player) bullets[i].vel.x = -bullets[i].vel.x;
		} else {
			bullets[i].vel.x = fix16Mul(cosFix16(spawner.angle), spawner.speed);
			bullets[i].vel.y = fix16Mul(sinFix16(spawner.angle), spawner.speed);
		}
		bullets[i].updater = updater;
		bullets[i].image = SPR_addSprite(spawner.image,
			fix16ToInt(fix16Sub(bullets[i].pos.x, bullets[i].off.x)) + GAME_X,
			fix16ToInt(fix16Sub(bullets[i].pos.y, bullets[i].off.y)) + GAME_Y,
			TILE_ATTR(PAL1, 0, 0, 0));
		SPR_setDepth(bullets[i].image, 4);
		SPR_setFrame(bullets[i].image, spawner.frame ? spawner.frame : 0);
	}
}

void killBullet(s16 i){
	bullets[i].active = FALSE;
	SPR_releaseSprite(bullets[i].image);
	if(bullets[i].dead){
		spawnExplosion(bullets[i].pos.x, bullets[i].pos.y);
		bullets[i].dead = FALSE;
	}
}


// helpers

void updateBulletVel(s16 i){
	bullets[i].vel.x = fix16Mul(cosFix16(bullets[i].angle), bullets[i].speed);
	bullets[i].vel.y = fix16Mul(sinFix16(bullets[i].angle), bullets[i].speed);
}


// collision

#define BULLET_PLAYER_LIMIT_X fix16Add(BULLET_LIMIT_X, FIX16(4))
#define BULLET_PLAYER_LIMIT_W fix16Sub(BULLET_LIMIT_W, FIX16(4))
#define BULLET_PLAYER_LIMIT_Y fix16Add(BULLET_LIMIT_Y, FIX16(4))
#define BULLET_PLAYER_LIMIT_Z fix16Sub(BULLET_LIMIT_Z, FIX16(4))
#define BULLET_PLAYER_MOD FIX16(-1)

#define FLIPPER_BULLET_OFF_X FIX16(16)
#define FLIPPER_BULLET_OFF_Y FIX16(28)
#define FLIPPER_BULLET_OFF_Y_2 FIX16(14)
#define BULLET_PLAYER_SPEED FIX16(2.5)
#define BULLET_PLAYER_SPEED_HIT FIX16(4)

#define BULLET_LIMIT_X 0
#define BULLET_LIMIT_W FIX16(GAME_W)
#define BULLET_LIMIT_Y 0
#define BULLET_LIMIT_Z FIX16(GAME_H)

fix32 bulletDist;

static void collideBulletWithEnemy(s16 i){
	for(s16 j = 0; j < ENEMY_COUNT; j++) if(enemies[j].active && enemies[j].seen){
		bulletDist = getApproximatedDistance(
			fix16ToFix32(fix16Sub(enemies[j].pos.x, bullets[i].pos.x)),
			fix16ToFix32(fix16Sub(enemies[j].pos.y, bullets[i].pos.y)));
		if(bulletDist <= fix32Add(enemies[j].dist, bullets[i].dist)){

			if(bullets[i].hitClock <= 0){
				enemies[j].health--;
				if(enemies[j].health <= 0){
					enemies[j].suicide(j);
					killEnemy(j);
					spawnExplosion(enemies[j].pos.x, enemies[j].pos.y);
					// SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
				}
				if(bullets[i].player){
					spawnExplosion(bullets[i].pos.x, bullets[i].pos.y);
					if(bullets[i].pos.x >= fix16Sub(enemies[j].pos.x, enemies[j].off.x) && bullets[i].pos.x <= fix16Add(enemies[j].pos.x, enemies[j].off.x))
						bullets[i].angle = 1024 - bullets[i].angle;
					else bullets[i].angle = 512 - bullets[i].angle;
					bullets[i].hitClock = 10;
					bullets[i].angle = bullets[i].angle % 1024;
					bullets[i].speed = BULLET_PLAYER_SPEED;
					updateBulletVel(i);
				} else{
					bullets[i].dead = TRUE;
					killBullet(i);
				}
			}

		}
	}
}

static void collideBulletWithPlayer(s16 i, u8 j){
	// bulletDist = getApproximatedDistance(
	// 	fix16ToFix32(fix16Sub(players[j].pos.x, bullets[i].pos.x)),
	// 	fix16ToFix32(fix16Sub(players[j].pos.y, bullets[i].pos.y)));
	// if(bulletDist <= fix32Add(players[j].dist, bullets[i].dist)){
	// 	spawnExplosion(players[j].pos.x, players[j].pos.y);
	// 	// XGM_startPlayPCM(SFX_EXPLOSION_1, 1, SOUND_PCM_CH4);
	// 	// for(s16 j = 0; j < BULLET_COUNT; j++) if(bullets[j].active){
	// 	// 	if(j < EXPLOSION_LIMIT / 2) spawnExplosion(bullets[j].pos.x, bullets[j].pos.y, 0);
	// 	// 	killBullet(j);
	// 	// }
	// 	// player.invincible = TRUE;
	// 	// player.invincibleClock = INVINCIBLE_LIMIT;
	// 	// player.lives--;
	// 	// if(player.lives < 0) killPlayer();
	// }
}

fix16 flipperDiff;
s16 nextIndex, nextX, nextY, currentX, currentY, currentIndex;
static void collideBullet(s16 i){
	// flipper
	if(bullets[i].angle % 1024 < 512 &&
		bullets[i].pos.y > fix16Sub(players[0].pos.y, FLIPPER_BULLET_OFF_Y) &&
		bullets[i].pos.y < fix16Sub(players[0].pos.y, FLIPPER_BULLET_OFF_Y_2) &&
		bullets[i].pos.x > fix16Sub(players[0].pos.x, FLIPPER_BULLET_OFF_X) &&
		bullets[i].pos.x < fix16Add(players[0].pos.x, FLIPPER_BULLET_OFF_X)){
		flipperDiff = fix16ToInt(fix16Sub(players[0].pos.x, bullets[i].pos.x)) * 8;
		bullets[i].angle = 768 - flipperDiff;
		bullets[i].speed = players[0].shotClock < 20 ? BULLET_PLAYER_SPEED_HIT : BULLET_PLAYER_SPEED;
		bullets[i].used = TRUE;
		updateBulletVel(i);
	} else {
		// sides
		if(bullets[i].pos.y > BULLET_PLAYER_LIMIT_Z || bullets[i].pos.y < BULLET_PLAYER_LIMIT_Y || bullets[i].pos.x > BULLET_PLAYER_LIMIT_W || bullets[i].pos.x < BULLET_PLAYER_LIMIT_X){
			if(bullets[i].pos.y > BULLET_PLAYER_LIMIT_Z || bullets[i].pos.y < BULLET_PLAYER_LIMIT_Y){
				if(bullets[i].player){
					bullets[i].pos.y = bullets[i].pos.y > BULLET_PLAYER_LIMIT_Z ? BULLET_PLAYER_LIMIT_Z : BULLET_PLAYER_LIMIT_Y;
					bullets[i].angle = 1024 - bullets[i].angle;
					bullets[i].speed = BULLET_PLAYER_SPEED;
					updateBulletVel(i);
				} else {
					if(bullets[i].pos.y < fix16Sub(0, bullets[i].off.y)){
						killBullet(i);
					} else if(bullets[i].pos.y > BULLET_PLAYER_LIMIT_Z){
						bullets[i].dead = TRUE;
						killBullet(i);
					}
				}
			}
			if(bullets[i].pos.x > BULLET_PLAYER_LIMIT_W || bullets[i].pos.x < BULLET_PLAYER_LIMIT_X){
				if(bullets[i].player){
					bullets[i].pos.x = bullets[i].pos.x > BULLET_PLAYER_LIMIT_W ? BULLET_PLAYER_LIMIT_W : BULLET_PLAYER_LIMIT_X;
					bullets[i].angle = 512 - bullets[i].angle;
					bullets[i].speed = BULLET_PLAYER_SPEED;
					updateBulletVel(i);
				} else {
					bullets[i].dead = TRUE;
					killBullet(i);
				}
			}
		} else {
			bullets[i].next.x = fix16Add(bullets[i].pos.x, fix16Mul(bullets[i].vel.x, FIX16(1)));
			bullets[i].next.y = fix16Add(bullets[i].pos.y, fix16Mul(bullets[i].vel.y, FIX16(1)));
			nextX = fix16ToInt(fix16Div(bullets[i].next.x, FIX16(16)));
			nextY = fix16ToInt(fix16Div(bullets[i].next.y, FIX16(16)));
			currentX = fix16ToInt(fix16Div(bullets[i].pos.x, FIX16(16)));
			currentY = fix16ToInt(fix16Div(bullets[i].pos.y, FIX16(16)));
			currentIndex = currentX + currentY * BRICK_W;
			nextIndex = nextX + nextY * BRICK_W;
			if(nextIndex < BRICK_COUNT){
				if(bricksStageOne[nextIndex] > 0 && bricks[nextIndex].active){
					if(nextIndex == currentIndex - BRICK_W || nextIndex == currentIndex + BRICK_W)
					if(nextIndex == currentIndex - 1 || nextIndex == currentIndex + 1)
					if(!bricks[nextIndex].invincible) killBrick(nextIndex);
				}
			}
			if(bullets[i].player || bullets[i].used) collideBulletWithEnemy(i);

		}
	}
}


// loop

static void updateBullet(s16 i){
	bullets[i].pos.x = fix16Add(bullets[i].pos.x, bullets[i].vel.x);
	bullets[i].pos.y = fix16Add(bullets[i].pos.y, bullets[i].vel.y);
	if(!bullets[i].used) bullets[i].updater(i);
	collideBullet(i);
	SPR_setPosition(
		bullets[i].image,
		fix16ToInt(fix16Sub(bullets[i].pos.x, bullets[i].off.x)) + GAME_X,
		fix16ToInt(fix16Sub(bullets[i].pos.y, bullets[i].off.y)) + GAME_Y);
	bullets[i].clock++;
	if(bullets[i].hitClock > 0) bullets[i].hitClock--;
}

void updateBullets(){
	if(killBullets){
		for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active){
			if(i % 4 == 0) bullets[i].dead = TRUE;
			killBullet(i);
		}
		killBullets = FALSE;
	} else {
		for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active) updateBullet(i);
	}
}