// bricks

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "enemies.h"
#include "bricks.h"
#include "players.h"
#include "stage.h"
#include "chrome.h"
#include "powerups.h"

#define BRICK_I 380

#define BRICK_OFF_X 2
#define BRICK_OFF_Y 4

u8 bricksStageOne[BRICK_COUNT] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 0, 0, 0, 0, 4, 4, 4, 4, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 2, 2, 2, 2, 0, 0, 0, 0, 5, 5, 5, 5, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 3, 3, 3, 3, 0, 0, 0, 0, 6, 6, 6, 6, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


// label

#define LABEL_I BRICK_I + 59

static void spawnBrickLabel(s16 h){
	s16 i = -1;
	for(s16 j = 0; j < BRICK_LABEL_COUNT; j++) if(!brickLabels[j].active && i == -1) i = j;
	if(i > -1){
		brickLabels[i].active = TRUE;
		brickLabels[i].x = bricks[h].x + 2;
		brickLabels[i].y = bricks[h].y + 4;
		brickLabels[i].clock = 50;
		// VDP_drawText("x2", brickLabels[i].x, brickLabels[i].y);
		// brickLabelx2
		VDP_drawImageEx(BG_A, &brickLabelx2, TILE_ATTR_FULL(PAL1, 0, 0, 0, LABEL_I), brickLabels[i].x, brickLabels[i].y, 0, DMA);
	}
}

static void updateBrickLabel(s16 i){
	brickLabels[i].clock--;
	if(brickLabels[i].clock <= 0){
		brickLabels[i].active = FALSE;
		VDP_clearTileMapRect(BG_A, brickLabels[i].x, brickLabels[i].y, 2, 2);
	}
}


// combo

#define COMBO_LIMIT 15
#define COMBO_X 1
#define COMBO_Y 2

bool inCombo;
s16 comboClock;

static void triggerCombo(){
	VDP_drawImageEx(BG_A, &comboLabel, TILE_ATTR_FULL(PAL1, 1, 0, 0, BRICK_I + 56), COMBO_X, COMBO_Y, 0, DMA);
	VDP_drawText("x4", COMBO_X + 3, COMBO_Y);
	VDP_drawText("+100", COMBO_X + 7, COMBO_Y);
};

static void killCombo(){
	inCombo = FALSE;
};

static void updateCombo(){
	if(comboClock > 0){
		comboClock--;
		if(comboClock <= 0) killCombo();
	}
};


// lifecycle

Image* brickImage;
static void spawnBrick(s16 x, s16 y, u8 type, s16 i){
	bricks[i].active = TRUE;
	bricks[i].x = x * 2;
	bricks[i].y = y * 2;
	switch(type){
		case 1: brickImage = &brickRed; break;
		case 2: brickImage = &brickOrange; break;
		case 3: brickImage = &brickYellow; break;
		case 4: brickImage = &brickGreen; break;
		case 5: brickImage = &brickBlue; break;
		case 6: brickImage = &brickPurple; break;
	}
	// bricks[i].invincible = type == 4;
	VDP_drawImageEx(BG_A, brickImage, TILE_ATTR_FULL(PAL1, 0, 0, 0, BRICK_I + (type - 1) * 4), bricks[i].x + BRICK_OFF_X, bricks[i].y + BRICK_OFF_Y, 0, DMA);
}

s16 brickKillCount;
void killBrick(s16 i){
	spawnExplosion(FIX16(bricks[i].x * 8 + 8), FIX16(bricks[i].y * 8 + 8));
	players[0].score += players[0].clock2x > 0 ? 200 : 100;
	bricks[i].active = FALSE;
	if(players[0].clock2x > 0) spawnBrickLabel(i);
	else VDP_clearTileMapRect(BG_A, bricks[i].x + BRICK_OFF_X, bricks[i].y + BRICK_OFF_Y, 2, 2);
	if(brickKillCount % 16 == 0)
		spawnPowerup(FIX16(bricks[i].x * 8 + 8), FIX16(bricks[i].y * 8 + 8), 0);
	brickKillCount++;
}

s16 mapX, mapY, brickI;
bool blocked;
static void loadMap(){
	mapX = 0;
	mapY = 0;
	brickKillCount = 0;
	for(s16 i = 0; i < BRICK_COUNT; i++){
		if(bricksStageOne[i] > 0) spawnBrick(mapX, mapY, bricksStageOne[i], i);
		mapX++;
		if(mapX >= BRICK_W){
			mapX = 0;
			mapY++;
		}
	}
}


// loop

void loadBricks(){
	triggerCombo();
	loadMap();
	spawnPowerup(FIX16(GAME_W / 2), FIX16(40), 2);
}

void updateBricks(){
	for(s16 i = 0; i < BRICK_LABEL_COUNT; i++) if(brickLabels[i].active) updateBrickLabel(i);
	updateCombo();
}