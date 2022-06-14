// chrome

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "chrome.h"
#include "controls.h"


// score

#define SCORE_LENGTH 8
#define SCORE_Y 1
#define SCORE_X 4
#define HIGH_X GAME_TILE_W - 1 - SCORE_LENGTH

static void loadScore(){
	VDP_drawText("SC", SCORE_X - 3, SCORE_Y);
	VDP_drawText("00000000", SCORE_X, SCORE_Y);
	VDP_drawText("HI", HIGH_X - 3, SCORE_Y);
	VDP_drawText("00000000", HIGH_X, SCORE_Y);
}


// lives

#define LIVES_X 1
#define LIVES_Y 2

static void loadLives(){
	for(u8 x = 0; x < 3; x++)
	VDP_drawText("#x2", LIVES_X, LIVES_Y);
}


// bombs

#define BOMBS_X 5
#define BOMBS_Y LIVES_Y

static void loadBombs(){
	VDP_drawText("*x3", BOMBS_X, BOMBS_Y);
}


// p2 message

#define P2_MESSAGE_X GAME_TILE_W - 15
#define P2_MESSAGE_Y 2

static void loadP2(){
	VDP_drawText("Press 2P Start", P2_MESSAGE_X, P2_MESSAGE_Y);
}


// boss

s16 lastBoss, bossBarW, bossBarOver;
bool bossActive;

#define BOSS_BAR_I 16
#define BOSS_BAR_X 1
#define BOSS_BAR_Y 3
#define BOSS_BAR_W FIX16(30)
#define BOSS_BAR_W_I fix16ToInt(BOSS_BAR_W)

static void updateBoss(){
	if(bossHealth != lastBoss && bossHealth > 0){
		lastBoss = bossHealth;
		bossBarW = fix16ToInt(fix16Mul(fix16Div(FIX16(bossHealth), FIX16(bossMax)), BOSS_BAR_W));
		bossBarOver = BOSS_BAR_W_I - bossBarW;
		if(bossActive) VDP_clearTileMapRect(BG_A, BOSS_BAR_X + bossBarW, BOSS_BAR_Y, bossBarOver, 1);
		else {
			bossActive = TRUE;
			VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, BOSS_BAR_I), BOSS_BAR_X, BOSS_BAR_Y, bossBarW, 1);
		}
	} else if(bossHealth == 0 && bossActive) bossActive = FALSE;
}



// loop

void loadChrome(){
	VDP_loadTileSet(boss.tileset, BOSS_BAR_I, DMA);
	loadScore();
	loadLives();
	loadBombs();
	loadP2();
	// drawDialogue();
}

void updateChrome(){
	updateBoss();
	// VDP_drawText(debugStr1, 0, 0);
}