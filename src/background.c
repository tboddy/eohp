// background

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "background.h"


// scrolling

#define BG_I 64
#define SCROLL_LIMIT -64

s16 bgScrollsA[GAME_TILE_H];
s16 bgScrollsB[GAME_TILE_H];
s16 bgClock;

#define FLOOR_Y 20
#define MID_Y 9
#define MID_H 11
#define TOP_Y 5

#define SCROLL_HEIGHT 12
#define SCROLL_START 4


#define BG_W GAME_TILE_W + 8
#define BG_I_MID BG_I + 64 + 5
#define BG_I_MID_2 BG_I + 64 + 5 + 1 + 16

static void drawMist(){
	for(s16 x = 0; x < BG_W; x++) for(s16 y = 0; y < GAME_TILE_H; y++){
		if(y == MID_Y){
			if(random() % 2 < 1) VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I_MID_2 + 1), x, y, 1, 1);
			else VDP_clearTileMapRect(BG_B, x, y, 1, 1);
		} else if(y > MID_Y && y < MID_Y + MID_H)
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I_MID_2 + (random() % 3 == 0 ? 0 : 1)), x, y, 1, 1);
	}
}

static void loadBackgroundTiles(){
	VDP_loadTileSet(stageOneMountainsBottom.tileset, BG_I_MID, DMA);
	VDP_loadTileSet(stageOneMistOne.tileset, BG_I_MID_2, DMA);
	VDP_loadTileSet(stageOneMistTwo.tileset, BG_I_MID_2 + 1, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I_MID), 0, 0, GAME_TILE_W, GAME_TILE_H);
	for(s16 x = 0; x < BG_W; x++) for(s16 y = 0; y < GAME_TILE_H; y++){
		if(y == TOP_Y && x % 8 == 0) VDP_drawImageEx(BG_B, &stageOneMountains, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64 + 5 + 1), x, y, 0, DMA);
		if(y == FLOOR_Y && x % 8 == 0) VDP_drawImageEx(BG_B, &stageOneGround, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I), x, y, 0, DMA);
		if(y == FLOOR_Y) VDP_drawImageEx(BG_A, &stageOneGroundOverlay, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64), x, y, 0, DMA);
	}
}

static void updateScrolling(){
	// if(bgClock % 5 == 0) drawMist();
	for(u8 y = 0; y < GAME_TILE_H; y++){
		if((y == TOP_Y || y == TOP_Y + 1) && bgClock % 90 == 0) bgScrollsB[y]--;

		if(y >= MID_Y && y < MID_Y + MID_H){
			// bgScrollsB[y]--;
		}

		else if(y >= FLOOR_Y){
			bgScrollsB[y] -= 3;
			if(y > FLOOR_Y) bgScrollsB[y]--;
			if(y > FLOOR_Y + 1) bgScrollsB[y]--;
			if(y > FLOOR_Y + 3) bgScrollsB[y]--;
			if(y > FLOOR_Y + 5) bgScrollsB[y]--;
		}
		if(bgScrollsB[y] <= SCROLL_LIMIT) bgScrollsB[y] -= SCROLL_LIMIT;
		if(bgScrollsA[y] <= SCROLL_LIMIT) bgScrollsA[y] -= SCROLL_LIMIT;
	}
	VDP_setHorizontalScrollTile(BG_A, 0, bgScrollsA, GAME_TILE_H, DMA);
	VDP_setHorizontalScrollTile(BG_B, 0, bgScrollsB, GAME_TILE_H, DMA);
}


// loop

void loadBackground(){
	VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);
	loadBackgroundTiles();
}

void updateBackground(){
	updateScrolling();
	bgClock++;
	if(bgClock >= CLOCK_LIMIT) bgClock -= CLOCK_LIMIT;
}