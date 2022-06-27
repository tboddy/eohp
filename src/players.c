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
#define PLAYER_INIT_X FIX16(GAME_W / 2)
#define PLAYER_INIT_Y FIX16(GAME_H - 32)
#define PLAYER_INIT_Y_P2 FIX16(GAME_H / 2 + PLAYER_INIT_DIFF)

#define FLIPPER_W FIX16(16)
#define FLIPPER_OFF_Y FIX16(32)

void spawnPlayer(bool p2){
	u8 i = p2 ? 1 : 0;
	players[i].active = TRUE;
	players[i].pos.x = PLAYER_INIT_X;
	players[i].pos.y = p2 ? PLAYER_INIT_Y_P2 : PLAYER_INIT_Y;
	players[i].off.x = FIX16(16);
	players[i].off.y = FIX16(16);
	players[i].dist = FIX32(1);
	players[i].clock2x = 0;
	players[i].dashClock = 0;
	players[i].score = 0;
	players[i].p2 = p2;
	players[i].image = SPR_addSprite(
		p2 ? &remilia : &flandre,
		fix16ToInt(fix16Sub(players[i].pos.x, players[i].off.x)),
		fix16ToInt(fix16Sub(players[i].pos.y, players[i].off.y)),
		TILE_ATTR(PAL1, 0, 0, 0));
	players[i].flipperLeft = SPR_addSprite(&flipper,
		fix16ToInt(fix16Sub(players[i].pos.x, FLIPPER_W)),
		fix16ToInt(fix16Sub(players[i].pos.y, FLIPPER_OFF_Y)),
		TILE_ATTR(PAL1, 0, 0, 0));
	players[i].flipperRight = SPR_addSprite(&flipper,
		fix16ToInt(players[i].pos.x),
		fix16ToInt(fix16Sub(players[i].pos.y, FLIPPER_OFF_Y)),
		TILE_ATTR(PAL1, 0, 0, 1));
	SPR_setDepth(players[i].image, 5);
	SPR_setDepth(players[i].flipperLeft, 5);
	SPR_setDepth(players[i].flipperRight, 5);
}


// movement

#define PLAYER_LIMIT_X FIX16(4)
#define PLAYER_LIMIT_W FIX16(GAME_W - 3)
#define PLAYER_LIMIT_Y FIX16(4)
#define PLAYER_LIMIT_H FIX16(GAME_H - 3)

#define PLAYER_SPEED FIX16(3)
#define PLAYER_SPEED_DASH FIX16(6)

static void updatePlayerBounds(u8 i){
	if(players[i].pos.x < PLAYER_LIMIT_X) players[i].pos.x = PLAYER_LIMIT_X;
	else if(players[i].pos.x > PLAYER_LIMIT_W) players[i].pos.x = PLAYER_LIMIT_W;
	if(players[i].pos.y < PLAYER_LIMIT_Y) players[i].pos.y = PLAYER_LIMIT_Y;
	else if(players[i].pos.y > PLAYER_LIMIT_H) players[i].pos.y = PLAYER_LIMIT_H;
}

#define DASH_LIMIT 15
#define DASH_MAX DASH_LIMIT - 10

static void updatePlayerMovement(u8 i){
	if(ctrl.b && players[i].dashClock == 0 && !players[i].dashing){
		players[i].dashClock = DASH_LIMIT;
		players[i].dashing = TRUE;
	} else if(!ctrl.b && players[i].dashClock == 0) players[i].dashing = FALSE;
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
		players[i].pos.x = fix16Add(players[i].pos.x, fix16Mul(cosFix16(players[i].angle), players[i].dashClock >= DASH_MAX ? PLAYER_SPEED_DASH : PLAYER_SPEED));
		players[i].pos.y = fix16Add(players[i].pos.y, fix16Mul(sinFix16(players[i].angle), players[i].dashClock >= DASH_MAX ? PLAYER_SPEED_DASH : PLAYER_SPEED));
		updatePlayerBounds(i);
	} 
	players[i].moveClock++;
	if(players[i].moveClock >= CLOCK_LIMIT) players[i].moveClock -= CLOCK_LIMIT;
	if(players[i].dashClock > 0) players[i].dashClock--;
}

static void updatePlayerAnimation(i){
	// SPR_setAnim(players[i].image, players[i].focus ? 1 : 0);
}


// powerups

static void updatePlayerPowerups(u8 i){
	if(players[0].clock2x > 0){
		players[0].clock2x--;
	}
}


// shooting

#define SHOT_INTERVAL 30
#define PLAYER_SHOT_X FIX16(GAME_W / 2)
#define PLAYER_SHOT_Y FIX16(GAME_H / 3)

static void flipPlayer(u8 i){
 switch(players[i].shotClock){
 	case 1:
 		SPR_setFrame(players[i].flipperLeft, 1);
 		SPR_setFrame(players[i].flipperRight, 1);
	 	break;
 	case 5:
 		SPR_setFrame(players[i].flipperLeft, 2);
 		SPR_setFrame(players[i].flipperRight, 2);
	 	break;
 	case 10:
 		SPR_setFrame(players[i].flipperLeft, 1);
 		SPR_setFrame(players[i].flipperRight, 1);
	 	break;
 	case 15:
 		SPR_setFrame(players[i].flipperLeft, 0);
 		SPR_setFrame(players[i].flipperRight, 0);
	 	break;
 }
}

static void spawnPlayerBullet(u8 i){
	spawnExplosion(PLAYER_SHOT_X, PLAYER_SHOT_Y);
	struct bulletSpawner spawner = {
		.x = PLAYER_SHOT_X,
		.y = PLAYER_SHOT_Y,
		.image = &smallPinkBullet,
		.speed = FIX16(2),
		.angle = 256,
		.player = TRUE,
		.p2 = players[i].p2
	};
	spawnBullet(spawner, EMPTY);
}

bool firedPlayer;

static void updatePlayerShot(u8 i){
	if(players[i].clock == 30 && !firedPlayer){
		firedPlayer = TRUE;
		spawnPlayerBullet(i);
	}
	if(players[i].shotClock >= SHOT_INTERVAL && ctrl.a) players[i].shotClock = 0;
	else if(players[i].shotClock < SHOT_INTERVAL) flipPlayer(i);
	players[i].shotClock++;
	if(players[i].shotClock >= CLOCK_LIMIT) players[i].shotClock = SHOT_INTERVAL;
}


// loop

void loadPlayers(){
	for(u8 i = 0; i < 2; i++) players[i].shotClock = SHOT_INTERVAL;
	spawnPlayer(FALSE);
}

#define PLAYER_OFF_X -1 + GAME_X
#define PLAYER_OFF_Y 3 + GAME_Y

void updatePlayers(){
	for(u8 i = 0; i < 2; i++){
		if(players[i].active){
			updatePlayerMovement(i);
			updatePlayerAnimation(i);
			updatePlayerPowerups(i);
			updatePlayerShot(i);
			SPR_setPosition(players[i].image,
				fix16ToInt(fix16Sub(players[i].pos.x, players[i].off.x)) + PLAYER_OFF_X,
				fix16ToInt(fix16Sub(players[i].pos.y, players[i].off.y)) + PLAYER_OFF_Y);
			SPR_setPosition(players[i].flipperLeft,
				fix16ToInt(fix16Sub(players[i].pos.x, FLIPPER_W)) + GAME_X,
				fix16ToInt(fix16Sub(players[i].pos.y, FLIPPER_OFF_Y)) + GAME_Y);
			SPR_setPosition(players[i].flipperRight,
				fix16ToInt(players[i].pos.x) + GAME_X,
				fix16ToInt(fix16Sub(players[i].pos.y, FLIPPER_OFF_Y)) + GAME_Y);
			players[i].clock++;
			if(players[i].clock >= CLOCK_LIMIT) players[i].clock -= CLOCK_LIMIT;
		}
	}
}