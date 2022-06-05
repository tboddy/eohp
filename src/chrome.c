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
	// for(u8 x = 0; x < 3; x++)
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




// loop

void loadChrome(){
	loadScore();
	loadLives();
	loadBombs();
	loadP2();
	// drawDialogue();
}

void updateChrome(){
	// VDP_drawText(debugStr1, 0, 0);
}