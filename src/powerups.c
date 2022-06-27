// background

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "powerups.h"
#include "players.h"


// lifecycle

#define POWERUP_OFF FIX16(8)

Image* powerupImage;

void spawnPowerup(fix16 x, fix16 y, u8 type){
	s16 i = -1;
	for(s16 j = 0; j < POWERUP_COUNT; j++) if(!powerups[j].active && i == -1) i = j;
	if(i > -1){
		powerups[i].active = TRUE;
		powerups[i].pos.x = x;
		powerups[i].pos.y = y;
		powerups[i].start = x;
		powerups[i].count = FIX16(4);
		powerups[i].type = type;
		switch(type){
			case 0: powerupImage = &powerupx2; break;
			case 1: powerupImage = &powerupMulti; break;
			case 2: powerupImage = &powerupTime; break;
		}
		powerups[i].image = SPR_addSprite(powerupImage,
			fix16ToInt(fix16Sub(powerups[i].pos.x, POWERUP_OFF)) + GAME_X,
			fix16ToInt(fix16Sub(powerups[i].pos.y, POWERUP_OFF)) + GAME_Y,
			TILE_ATTR(PAL1, 0, 0, 0));
	}
}

void killPowerup(s16 i){
	powerups[i].active = FALSE;
	SPR_releaseSprite(powerups[i].image);
}


// collect

#define COLLECT_LABEL_X 26
#define COLLECT_LABEL_Y 2
#define COLLECT_LABEL_W 5
#define COLLECT_LABEL_LIMIT 60 * 3

s16 collectLabelClock;
char* collectLabelStr[COLLECT_LABEL_W];

static void spawnCollectLabel(u8 type){
	switch(type){
		case 0: strcpy(collectLabelStr, "x2 SC"); break;
		case 1: strcpy(collectLabelStr, "MULTI"); break;
		case 2: strcpy(collectLabelStr, "+30 S"); break;
	}
	VDP_drawText(collectLabelStr, COLLECT_LABEL_X, COLLECT_LABEL_Y);
	collectLabelClock = COLLECT_LABEL_LIMIT;
}

static void killCollectLabel(){
	VDP_clearTextArea(COLLECT_LABEL_X, COLLECT_LABEL_Y, COLLECT_LABEL_W, 1);
}

#define LIMIT_2X 60 * 10
static void collect2x(){
	players[0].clock2x = LIMIT_2X;
	spawnCollectLabel(0);
}

static void collectMulti(){

}

static void collectTime(){

}


#define POWERUP_DIST FIX32(16)
fix32 powerupDist;

static void collidePowerupWithPlayer(s16 i){
	powerupDist = getApproximatedDistance(
		fix16ToFix32(fix16Sub(players[0].pos.x, powerups[i].pos.x)),
		fix16ToFix32(fix16Sub(players[0].pos.y, powerups[i].pos.y)));
	if(powerupDist <= POWERUP_DIST){
		switch(powerups[i].type){
			case 0: collect2x(); break;
			case 1: collectMulti(); break;
			case 2: collectTime(); break;
		}
		killPowerup(i);
	}
}

#define POWERUP_SPEED_Y FIX16(0.5)
#define POWERUP_MOD_X FIX16(24)
#define POWERUP_MOD_C FIX16(0.15)

static void updatePowerup(s16 i){
	if(powerups[i].type == 2){

	} else {
		if(powerups[i].pos.y >= fix16Add(FIX16(GAME_H), POWERUP_OFF)){
			killPowerup(i);
		} else {
			powerups[i].pos.y = fix16Add(powerups[i].pos.y, POWERUP_SPEED_Y);
			powerups[i].pos.x = fix16Sub(powerups[i].start,
				fix16Mul(cosFix16(powerups[i].count), POWERUP_MOD_X));
			powerups[i].count = fix16Add(powerups[i].count, POWERUP_MOD_C);
			SPR_setPosition(powerups[i].image,
				fix16ToInt(fix16Sub(powerups[i].pos.x, POWERUP_OFF)) + GAME_X,
				fix16ToInt(fix16Sub(powerups[i].pos.y, POWERUP_OFF)) + GAME_Y);
			collidePowerupWithPlayer(i);
		}
	}
}

// loop

void updatePowerups(){
	for(s16 i = 0; i < POWERUP_COUNT; i++) if(powerups[i].active) updatePowerup(i);
	if(collectLabelClock > 0){
		collectLabelClock--;
		if(collectLabelClock <= 0) killCollectLabel();
	}
}