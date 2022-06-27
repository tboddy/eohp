// enemies

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "bullets.h"
#include "players.h"
#include "enemies.h"

#define SEAL_OFF FIX16(24)


// lifecycle

void spawnEnemy(struct enemySpawner spawner, void(*updater), void(*suicide), void(*bossUpdater)){
	s16 i = -1;
	for(s16 j = 0; j < ENEMY_COUNT; j++) if(!enemies[j].active && i == -1) i = j;
	if(i > -1){
		enemies[i].active = TRUE;
		enemies[i].off.x = FIX16(spawner.offX);
		enemies[i].off.y = FIX16(spawner.offY);
		enemies[i].pos.x = FIX16(spawner.x);
		enemies[i].pos.y = FIX16(spawner.y);
		enemies[i].dist = intToFix32(spawner.offX - 2);
		enemies[i].speed = spawner.speed;
		enemies[i].angle = spawner.angle;
		enemies[i].boss = spawner.boss;
		enemies[i].shotClock = 0;
		enemies[i].dead = FALSE;
		enemies[i].health = spawner.health ? spawner.health : 1;
		if(spawner.vel.x && spawner.vel.y){
			enemies[i].vel.x = spawner.vel.x;
			enemies[i].vel.y = spawner.vel.y;
		} else {
			enemies[i].vel.x = fix16Mul(cosFix16(spawner.angle), spawner.speed);
			enemies[i].vel.y = fix16Mul(sinFix16(spawner.angle), spawner.speed);
		}
		for(s16 j = 0; j < E_COUNT_INT; j++){
			enemies[i].bools[j] = spawner.bools[j];
			enemies[i].ints[j] = spawner.ints[j];
			enemies[i].fixes[j] = spawner.fixes[j];
		}
		enemies[i].updater = updater;
		enemies[i].bossUpdater = bossUpdater;
		enemies[i].suicide = suicide;
		enemies[i].clock = 0;
		enemies[i].seen = FALSE;
		enemies[i].image = SPR_addSprite(spawner.image,
			fix16ToInt(fix16Sub(enemies[i].pos.x, enemies[i].off.x)) + GAME_X,
			fix16ToInt(fix16Sub(enemies[i].pos.y, enemies[i].off.y)) + GAME_Y,
			TILE_ATTR(PAL1, 0, 0, 0));
		SPR_setAnim(enemies[i].image, spawner.anim ? spawner.anim : 0);
		SPR_setFrame(enemies[i].image, spawner.frame ? spawner.frame : 0);
		SPR_setDepth(enemies[i].image, 5);
		SPR_setVisibility(enemies[i].image, HIDDEN);
		if(spawner.seal){
			enemies[i].seal = TRUE;
			enemies[i].sealImage = SPR_addSprite(&seal,
				fix16ToInt(fix16Sub(enemies[i].pos.x, SEAL_OFF)),
				fix16ToInt(fix16Sub(enemies[i].pos.y, SEAL_OFF)),
				TILE_ATTR(PAL2, 0, 0, 0));
			SPR_setAnim(enemies[i].sealImage, spawner.anim ? spawner.anim : 0);
			SPR_setDepth(enemies[i].sealImage, 6);
		}
	}
}

void killEnemy(s16 i){
	enemies[i].dead = TRUE;
}


// collision

void collideEnemy(s16 i){
	// enemyDist = getApproximatedDistance(
	// 	fix16ToFix32(fix16Sub(players[j].pos.x, enemies[i].pos.x)),
	// 	fix16ToFix32(fix16Sub(players[j].pos.y, enemies[i].pos.y)));
	// if(enemyDist <= fix32Add(players[j].dist, enemies[i].dist)){
	// 	spawnExplosion(players[j].pos.x, players[j].pos.y, 1);
	// 	// XGM_startPlayPCM(random() % 2 < 1 ? SFX_EXPLOSION_1 : SFX_EXPLOSION_2, 1, SOUND_PCM_CH4);
	// 	players[j].invincible = TRUE;
	// 	players[j].invincibleClock = INVINCIBLE_LIMIT;
	// 	// players[j].lives--;
	// 	// if(players[j].lives < 0) killPlayer();
	// 	if(!enemies[i].boss) killEnemy(i);
	// }
}


// helpers

void updateEnemyVel(s16 i){
	enemies[i].vel.x = fix16Mul(cosFix16(enemies[i].angle), enemies[i].speed);
	enemies[i].vel.y = fix16Mul(sinFix16(enemies[i].angle), enemies[i].speed);
}


// loop

static void updateEnemy(s16 i){
	if(enemies[i].seen && (enemies[i].pos.x < fix16Sub(0, enemies[i].off.x) ||
		enemies[i].pos.x > fix16Add(FIX16(GAME_W), enemies[i].off.x) ||
		enemies[i].pos.y < fix16Sub(0, enemies[i].off.y) ||
		enemies[i].pos.y > fix16Add(FIX16(GAME_H), enemies[i].off.y))){
		killEnemy(i);
		enemies[i].suicide(i);
	} else {
		if(!enemies[i].seen && enemies[i].pos.x <= fix16Add(FIX16(GAME_W), enemies[i].off.y)){
			enemies[i].seen = TRUE;
			SPR_setVisibility(enemies[i].image, VISIBLE);
		}
		enemies[i].pos.x = fix16Add(enemies[i].pos.x, enemies[i].vel.x);
		enemies[i].pos.y = fix16Add(enemies[i].pos.y, enemies[i].vel.y);
		if(enemies[i].seen){
			enemies[i].updater(i);
			if(enemies[i].boss) bossHealth = enemies[i].health;
			collideEnemy(i);
			enemies[i].clock++;
			if(enemies[i].clock >= CLOCK_LIMIT) enemies[i].clock -= CLOCK_LIMIT;
			SPR_setPosition(enemies[i].image,
				fix16ToInt(fix16Sub(enemies[i].pos.x, enemies[i].off.x)) + GAME_X,
				fix16ToInt(fix16Sub(enemies[i].pos.y, enemies[i].off.y)) + GAME_Y);
			if(enemies[i].seal){
				SPR_setPosition(enemies[i].sealImage,
					fix16ToInt(fix16Sub(enemies[i].pos.x, SEAL_OFF)) + GAME_X,
					fix16ToInt(fix16Sub(enemies[i].pos.y, SEAL_OFF)) + GAME_Y);
			}
		}
	}
	if(enemies[i].dead){
		enemies[i].active = FALSE;
		SPR_releaseSprite(enemies[i].image);
		if(enemies[i].seal){
			enemies[i].seal = FALSE;
			SPR_releaseSprite(enemies[i].sealImage);
		}
	}
}

void updateEnemies(){
	for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) updateEnemy(i);
}