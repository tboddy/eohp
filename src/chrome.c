// chrome

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "chrome.h"
#include "controls.h"
#include "players.h"


#define CHROME_I 16

// frame

#define FRAME_I CHROME_I + 1
#define FRAME_W WIN_W / 8
#define FRAME_H WIN_H / 8
#define SIDEBAR_H GAME_H / 8
#define SIDEBAR_X GAME_W / 8
#define SIDEBAR_W WIN_W / 8 - SIDEBAR_X

static void loadFrame(){
	// sides
	for(u8 y = 0; y < 6; y++){
		VDP_drawImageEx(BG_A, &frame5, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), 0, 4 + y * 4, 0, DMA);
		VDP_drawImageEx(BG_A, &frame5, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), 30, 4 + y * 4, 0, DMA);
	}

	// top
	for(u8 x = 0; x < 32; x++)
		VDP_drawImageEx(BG_B, &frame1, TILE_ATTR_FULL(PAL2, 1, 0, 0, FRAME_I + 8), x, 0, 0, DMA);

	// VDP_loadTileSet(frame1.tileset, FRAME_I + 8, DMA);
	// VDP_loadTileSet(frame2.tileset, FRAME_I + 9, DMA);
	// VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I + 8), 0, 0, WIN_W / 8, 3);
	// VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I + 9), 0, 3, WIN_W / 8, 1);

	// VDP_loadTileSet(frame2.tileset, FRAME_I + 1, DMA);
	// VDP_loadTileSet(frame3.tileset, FRAME_I + 2, DMA);
	// VDP_loadTileSet(frame4.tileset, FRAME_I + 3, DMA);

	// left
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), 0, 1, 1, SIDEBAR_H);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I + 2), 1, 1, 1, SIDEBAR_H);

	// top
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), 0, 0, 1, 1);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I + 3), 1, 0, 1, 1);
	// VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I + 1), 2, 0, FRAME_W - 16, 1);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 1, FRAME_I + 3), SIDEBAR_X, 0, 1, 1);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), SIDEBAR_X + 1, 0, 13, 1);

	// bottom
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), 0, FRAME_H - 1, 1, 1);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 1, 0, FRAME_I + 3), 1, FRAME_H - 1, 1, 1);
	// VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL1, 1, 1, 0, FRAME_I + 1), 2, FRAME_H - 1, FRAME_W - 16, 1);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 1, 1, FRAME_I + 3), SIDEBAR_X, FRAME_H - 1, 1, 1);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), SIDEBAR_X + 1, FRAME_H - 1, 13, 1);

	// right
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), SIDEBAR_X, 0, SIDEBAR_W, SIDEBAR_H);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 1, FRAME_I + 2), SIDEBAR_X, 1, 1, SIDEBAR_H);
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), SIDEBAR_X + 1, 1, SIDEBAR_W - 1, SIDEBAR_H);
}


// score

#define SCORE_LENGTH 8
#define SCORE_X 1
#define SCORE_Y 1
#define SCORE_I CHROME_I + 16

s32 lastScore;
char scoreStr[SCORE_LENGTH];

static void loadScore(){
	VDP_drawText("SC", SCORE_X, SCORE_Y);
	VDP_drawText("00000000", SCORE_X + 3, SCORE_Y);
}

static void updateScore(){
	if(lastScore != players[0].score){
		intToStr(players[0].score, scoreStr, SCORE_LENGTH);
		VDP_drawText(scoreStr, SCORE_X + 3, SCORE_Y);
		lastScore = players[0].score;
	}
}


// time

#define MINUTES_LENGTH 1
#define SECONDS_LENGTH 2
#define TIME_X 14
#define TIME_Y 1

Image* numImage;
u8 numOff;

s8 secondsLeft, minutesLeft,
	lastSecondsLeft, lastMinutesLeft;

char minutesStr[MINUTES_LENGTH];
char lastMinutesStr[MINUTES_LENGTH];
char secondsStr[SECONDS_LENGTH];
char lastSecondsStr[SECONDS_LENGTH];

static void drawTime(){
	if(lastMinutesLeft != minutesLeft){
		intToStr(minutesLeft, minutesStr, MINUTES_LENGTH);
		switch(minutesStr[0]){
			case '0': numImage = &num0; numOff = 0; break;
			case '1': numImage = &num1; numOff = 1; break;
			case '2': numImage = &num2; numOff = 2; break;
			case '3': numImage = &num3; numOff = 3; break;
			case '4': numImage = &num4; numOff = 4; break;
			case '5': numImage = &num5; numOff = 5; break;
			case '6': numImage = &num6; numOff = 6; break;
			case '7': numImage = &num7; numOff = 7; break;
			case '8': numImage = &num8; numOff = 8; break;
			case '9': numImage = &num9; numOff = 9; break;
		}
		VDP_drawImageEx(BG_A, numImage, TILE_ATTR_FULL(PAL1, 1, 0, 0, SCORE_I + numOff * 2), TIME_X, TIME_Y, 0, DMA);
	}
	intToStr(secondsLeft, secondsStr, SECONDS_LENGTH);
	for(u8 x = 0; x < SECONDS_LENGTH; x++){
		switch(secondsStr[x]){
			case '0': numImage = &num0; numOff = 0; break;
			case '1': numImage = &num1; numOff = 1; break;
			case '2': numImage = &num2; numOff = 2; break;
			case '3': numImage = &num3; numOff = 3; break;
			case '4': numImage = &num4; numOff = 4; break;
			case '5': numImage = &num5; numOff = 5; break;
			case '6': numImage = &num6; numOff = 6; break;
			case '7': numImage = &num7; numOff = 7; break;
			case '8': numImage = &num8; numOff = 8; break;
			case '9': numImage = &num9; numOff = 9; break;
		}
		VDP_drawImageEx(BG_A, numImage, TILE_ATTR_FULL(PAL1, 1, 0, 0, SCORE_I + numOff * 2), TIME_X + x + 2, TIME_Y, 0, DMA);
	}
	lastSecondsLeft = secondsLeft;
	lastMinutesLeft = minutesLeft;
}

static void loadTime(){
	secondsLeft = 5;
	minutesLeft = 1;
	VDP_drawImageEx(BG_A, &numColon, TILE_ATTR_FULL(PAL1, 1, 0, 0, SCORE_I + 10 * 2), TIME_X + 1, TIME_Y, 0, DMA);
	drawTime();
}

static void updateTime(){
	secondsLeft--;
	if(secondsLeft <= -1){
		if(minutesLeft == 0){

		} else {
			secondsLeft = 59;
			minutesLeft--;
		}
	}
	drawTime();
}


// world

#define WORLD_X 20
#define WORLD_Y 1

static void loadWorld(){
	VDP_drawText("WORLD", WORLD_X, WORLD_Y);
	VDP_drawText("1:1", WORLD_X + 1, WORLD_Y + 1);
}

#define LIVES_X 28
#define LIVES_Y 1

static void loadLives(){
	VDP_drawText("#x2", LIVES_X, LIVES_Y);
}


// boss

s16 lastBoss, bossBarW, bossBarOver;
bool bossActive;

#define BOSS_BAR_I CHROME_I
#define BOSS_BAR_X 3
#define BOSS_BAR_Y 5
#define BOSS_BAR_W FIX16(26)
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
	} else if(bossHealth == 0 && bossActive){
		bossActive = FALSE;
		VDP_clearTileMapRect(BG_A, BOSS_BAR_X, BOSS_BAR_Y, 26, 1);
	}
}


// loop
s16 chromeClock;

void loadChrome(){
	loadFrame();
	VDP_loadTileSet(boss.tileset, BOSS_BAR_I, DMA);
	loadScore();
	loadTime();
	loadWorld();
	loadLives();
	// drawDialogue();
}

void updateChrome(){
	updateScore();
	if(chromeClock % 60 == 0) updateTime();
	chromeClock++;
	if(chromeClock >= CLOCK_LIMIT) chromeClock -= CLOCK_LIMIT;
	updateBoss();
	// VDP_drawText(debugStr1, 0, 0);
}