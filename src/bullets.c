// bullets

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "bullets.h"
#include "players.h"

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
		bullets[i].dist = fix16ToFix32(fix16Sub(bullets[i].off.x, FIX16(3)));
		bullets[i].speed = spawner.speed;
		bullets[i].clock = 0;
		bullets[i].nextX = 0;
		bullets[i].nextY = 0;
		bullets[i].currentX = 0;
		bullets[i].currentY = 0;
		bullets[i].angle = spawner.angle;
		bullets[i].player = spawner.player;
		bullets[i].p2 = spawner.p2;
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
			fix16ToInt(fix16Sub(bullets[i].pos.x, bullets[i].off.x)),
			fix16ToInt(fix16Sub(bullets[i].pos.y, bullets[i].off.y)),
			TILE_ATTR(PAL1, 0, 0, 0));
		SPR_setDepth(bullets[i].image, 2);
		SPR_setFrame(bullets[i].image, spawner.frame ? spawner.frame : 0);
	}
}

void killBullet(s16 i){
	bullets[i].active = FALSE;
	SPR_releaseSprite(bullets[i].image);
}


// helpers

void updateBulletVel(s16 i){
	bullets[i].vel.x = fix16Mul(cosFix16(bullets[i].angle), bullets[i].speed);
	bullets[i].vel.y = fix16Mul(sinFix16(bullets[i].angle), bullets[i].speed);
}


// collision

#define BULLET_LIMIT_X 0
#define BULLET_LIMIT_W FIX16(GAME_W)
#define BULLET_LIMIT_Y 0
#define BULLET_LIMIT_Z FIX16(GAME_H)

static void collideBullet(s16 i){
	if(bullets[i].pos.x < fix16Sub(BULLET_LIMIT_X, bullets[i].off.x) ||
		bullets[i].pos.x > fix16Add(BULLET_LIMIT_W, bullets[i].off.x) ||
		bullets[i].pos.y < fix16Sub(BULLET_LIMIT_Y, bullets[i].off.y) ||
		bullets[i].pos.y > fix16Add(BULLET_LIMIT_Z, bullets[i].off.y)){
		killBullet(i);
	}
}


// loop

static void updateBullet(s16 i){
	bullets[i].pos.x = fix16Add(bullets[i].pos.x, bullets[i].vel.x);
	bullets[i].pos.y = fix16Add(bullets[i].pos.y, bullets[i].vel.y);
	bullets[i].updater(i);
	collideBullet(i);
	SPR_setPosition(
		bullets[i].image,
		fix16ToInt(fix16Sub(bullets[i].pos.x, bullets[i].off.x)),
		fix16ToInt(fix16Sub(bullets[i].pos.y, bullets[i].off.y)));
	bullets[i].clock++;
}

void updateBullets(){
	for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active) updateBullet(i);
}