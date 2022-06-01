// players

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "bullets.h"
#include "players.h"
#include "enemies.h"


// spawn

#define PLAYER_INIT_DIFF 32
#define PLAYER_INIT_X FIX16(32)
#define PLAYER_INIT_Y FIX16(GAME_H / 2 - PLAYER_INIT_DIFF)
#define PLAYER_INIT_Y_P2 FIX16(GAME_H / 2 + PLAYER_INIT_DIFF)

void spawnPlayer(bool p2){
	u8 i = p2 ? 1 : 0;
	players[i].active = TRUE;
	players[i].pos.x = PLAYER_INIT_X;
	players[i].pos.y = p2 ? PLAYER_INIT_Y_P2 : PLAYER_INIT_Y;
	players[i].off.x = FIX16(16);
	players[i].off.y = FIX16(20);
	players[i].dist = FIX32(1);
	players[i].p2 = p2;
	players[i].image = SPR_addSprite(
		p2 ? &remilia : &flandre,
		fix16ToInt(fix16Sub(players[i].pos.x, players[i].off.x)),
		fix16ToInt(fix16Sub(players[i].pos.y, players[i].off.y)),
		TILE_ATTR(PAL1, 0, 0, 0));
	SPR_setDepth(players[i].image, 1);
}


// movement

#define PLAYER_LIMIT_X FIX16(4)
#define PLAYER_LIMIT_W FIX16(GAME_W - 3)
#define PLAYER_LIMIT_Y FIX16(4)
#define PLAYER_LIMIT_H FIX16(GAME_H - 3)

#define PLAYER_SPEED FIX16(2.5)
#define PLAYER_SPEED_FOCUS FIX16(1.5)

static void updatePlayerBounds(u8 i){
	if(players[i].pos.x < PLAYER_LIMIT_X) players[i].pos.x = PLAYER_LIMIT_X;
	else if(players[i].pos.x > PLAYER_LIMIT_W) players[i].pos.x = PLAYER_LIMIT_W;
	if(players[i].pos.y < PLAYER_LIMIT_Y) players[i].pos.y = PLAYER_LIMIT_Y;
	else if(players[i].pos.y > PLAYER_LIMIT_H) players[i].pos.y = PLAYER_LIMIT_H;
}

static void updatePlayerMovement(u8 i){
	if(ctrl.b && !players[i].focus) players[i].focus = TRUE;
	else if(!ctrl.b && players[i].focus) players[i].focus = FALSE;
	if(ctrl.left || ctrl.right || ctrl.up || ctrl.down){
		if(ctrl.left || ctrl.right){
			if(ctrl.up){
				players[i].angle = ctrl.left ? 640 : 896;
			} else if(ctrl.down){
				players[i].angle = ctrl.left ? 384 : 128;
			} else {
				players[i].angle = ctrl.left ? 512 : 0;
			}
			SPR_setFrame(players[i].image, ctrl.left ? 1 : 2);
		} else if(ctrl.up){
			players[i].angle = 768;
		} else if(ctrl.down){
			players[i].angle = 256;
		}
		// PLAYER_SPEED_FOCUS
		players[i].pos.x = fix16Add(players[i].pos.x, fix16Mul(cosFix16(players[i].angle), players[i].focus ? PLAYER_SPEED_FOCUS : PLAYER_SPEED));
		players[i].pos.y = fix16Add(players[i].pos.y, fix16Mul(sinFix16(players[i].angle), players[i].focus ? PLAYER_SPEED_FOCUS : PLAYER_SPEED));
		updatePlayerBounds(i);
	} 
	players[i].moveClock++;
	if(players[i].moveClock >= CLOCK_LIMIT) players[i].moveClock -= CLOCK_LIMIT;
}

static void updatePlayerAnimation(i){
	SPR_setAnim(players[i].image, players[i].focus ? 1 : 0);
}


// shooting

#define SHOT_INTERVAL 10

static void spawnPlayerBullet(u8 i){
	struct bulletSpawner spawner = {
		.x = players[i].pos.x,
		.y = players[i].pos.y,
		.image = players[i].p2 ? &smallYellowBullet : &smallWhiteBullet,
		.speed = FIX16(16),
		.angle = 0,
		.player = TRUE,
		.p2 = players[i].p2
	};
	spawnBullet(spawner, EMPTY);
}

static void updatePlayerShot(u8 i){
	if(players[i].shotClock >= SHOT_INTERVAL && ctrl.a) players[i].shotClock = 0;
	if(players[i].shotClock == 0) spawnPlayerBullet(i);
	players[i].shotClock++;
	if(players[i].shotClock >= CLOCK_LIMIT) players[i].shotClock = SHOT_INTERVAL;
}


// loop

void loadPlayers(){
	for(u8 i = 0; i < 2; i++) players[i].shotClock = SHOT_INTERVAL;
	spawnPlayer(FALSE);
}

#define PLAYER_OFF_X -1
#define PLAYER_OFF_Y 3

void updatePlayers(){
	for(u8 i = 0; i < 2; i++){
		if(players[i].active){
			updatePlayerMovement(i);
			updatePlayerAnimation(i);
			updatePlayerShot(i);
			SPR_setPosition(players[i].image,
				fix16ToInt(fix16Sub(players[i].pos.x, players[i].off.x)) + PLAYER_OFF_X,
				fix16ToInt(fix16Sub(players[i].pos.y, players[i].off.y)) + PLAYER_OFF_Y);
			players[i].clock++;
			if(players[i].clock >= CLOCK_LIMIT) players[i].clock = 0;
		}
	}
}