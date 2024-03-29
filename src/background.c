// background

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "background.h"


// scrolling

#define BG_I 64
#define BG_X 2
#define BG_W GAME_TILE_W

s16 bgScrollsA[GAME_TILE_H];
s16 bgScrollsB[GAME_TILE_H + 4];
s16 bgClock;


// stage one

#define STAGE_ONE_SKY_H 4
#define STAGE_ONE_SKY_1_Y 4
#define STAGE_ONE_SKY_2_Y STAGE_ONE_SKY_1_Y + STAGE_ONE_SKY_H
#define STAGE_ONE_SKY_4_Y STAGE_ONE_SKY_2_Y + 2
#define STAGE_ONE_SKY_5_Y STAGE_ONE_SKY_4_Y + STAGE_ONE_SKY_H
#define STAGE_ONE_SKY_6_Y STAGE_ONE_SKY_5_Y + 2

#define STAGE_ONE_WATER_Y 19
#define STAGE_ONE_WATER_W 14
#define STAGE_ONE_WATER_LIMIT STAGE_ONE_WATER_W * -8

#define STAGE_ONE_CLOUDS_Y 7
#define STAGE_ONE_CLOUDS_W 32
#define STAGE_ONE_CLOUDS_LIMIT STAGE_ONE_CLOUDS_W * -8

static void loadStageOne(){

	// sky
	VDP_loadTileSet(stageOneSky1.tileset, BG_I, DMA);
	VDP_loadTileSet(stageOneSky2.tileset, BG_I + 1, DMA);
	VDP_loadTileSet(stageOneSky3.tileset, BG_I + 2, DMA);
	VDP_loadTileSet(stageOneSky4.tileset, BG_I + 3, DMA);
	VDP_loadTileSet(stageOneSky5.tileset, BG_I + 4, DMA);
	VDP_loadTileSet(stageOneSky6.tileset, BG_I + 5, DMA);
	VDP_loadTileSet(stageOneSky7.tileset, BG_I + 6, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I), BG_X, STAGE_ONE_SKY_1_Y, BG_W, STAGE_ONE_SKY_H);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 1), BG_X, STAGE_ONE_SKY_2_Y, BG_W, 1);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 2), BG_X, STAGE_ONE_SKY_2_Y + 1, BG_W, 1);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 3), BG_X, STAGE_ONE_SKY_4_Y, BG_W, STAGE_ONE_SKY_H);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 4), BG_X, STAGE_ONE_SKY_5_Y, BG_W, 1);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 5), BG_X, STAGE_ONE_SKY_5_Y + 1, BG_W, 1);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 6), BG_X, STAGE_ONE_SKY_6_Y, BG_W, STAGE_ONE_SKY_H);

	// water
	VDP_loadTileSet(stageOneWater2.tileset, BG_I + 7, DMA);
	for(u8 x = 0; x < 4; x++)
		VDP_drawImageEx(BG_B, &stageOneWater, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 8), x * STAGE_ONE_WATER_W, STAGE_ONE_WATER_Y, 0, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 7), BG_X, STAGE_ONE_WATER_Y + 6, BG_W, 3);


	// clouds
	// for(u8 x = 0; x < 2; x++)
	// 	VDP_drawImageEx(BG_A, &stageOneClouds, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 84 + 352 + 2), x * STAGE_ONE_CLOUDS_W, STAGE_ONE_CLOUDS_Y, 0, DMA);
}

static void updateStageOne(){
	for(u8 y = 0; y < GAME_TILE_H + 4; y++){
		// water
		if(y >= STAGE_ONE_WATER_Y && y < STAGE_ONE_WATER_Y + 6){
			bgScrollsB[y]--;
			if(y >= STAGE_ONE_WATER_Y + 1) bgScrollsB[y]--;
			if(y >= STAGE_ONE_WATER_Y + 2) bgScrollsB[y]--;
			if(y >= STAGE_ONE_WATER_Y + 4) bgScrollsB[y] -= 2;
			if(bgScrollsB[y] <= STAGE_ONE_WATER_LIMIT) bgScrollsB[y] -= STAGE_ONE_WATER_LIMIT;
		}
		// clouds
		// if(y >= STAGE_ONE_CLOUDS_Y && y < STAGE_ONE_WATER_Y && bgClock % 5 == 0){
		// 	bgScrollsA[y] -= 2;
		// 	if(bgScrollsA[y] <= STAGE_ONE_CLOUDS_LIMIT) bgScrollsA[y] -= STAGE_ONE_CLOUDS_LIMIT;
		// }
	}
	// VDP_setHorizontalScrollTile(BG_A, 0, bgScrollsA, GAME_TILE_H, DMA);
	VDP_setHorizontalScrollTile(BG_B, 0, bgScrollsB, GAME_TILE_H + 4, DMA);
}


// stage two

#define STAGE_TWO_TREES_Y 7
#define STAGE_TWO_TREES_W 32
#define STAGE_TWO_TREES_H 16
#define STAGE_TWO_TREES_LIMIT STAGE_TWO_TREES_W * -8

#define STAGE_TWO_TREES2_Y 10
#define STAGE_TWO_TREES2_W 18
#define STAGE_TWO_TREES2_H 8
#define STAGE_TWO_TREES2_LIMIT STAGE_TWO_TREES2_W * -8

static void loadStageTwo(){

	// foreground
	VDP_loadTileSet(stageTwoTrees2.tileset, BG_I, DMA);
	VDP_loadTileSet(stageTwoTrees3.tileset, BG_I + 1, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I), 0, 0, GAME_TILE_W, STAGE_TWO_TREES_Y);
	for(u8 x = 0; x < 2; x++)
		VDP_drawImageEx(BG_B, &stageTwoTrees, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I + 2), x * STAGE_TWO_TREES_W, STAGE_TWO_TREES_Y, 0, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I + 1), 0, STAGE_TWO_TREES_Y + STAGE_TWO_TREES_H, GAME_TILE_W, STAGE_TWO_TREES_Y);

	// background
	VDP_loadTileSet(stageTwoTrees5.tileset, BG_I + 2 + 512 + 144, DMA);
	VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 2 + 512 + 144), 0, STAGE_TWO_TREES2_Y - 3, GAME_TILE_W, 3);
	for(u8 x = 0; x < 3; x++)
		VDP_drawImageEx(BG_A, &stageTwoTrees4, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 2 + 512), x * STAGE_TWO_TREES2_W, STAGE_TWO_TREES2_Y, 0, DMA);
	VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 2 + 512 + 144), 0, STAGE_TWO_TREES2_Y + STAGE_TWO_TREES2_H, GAME_TILE_W, 3);
}

static void updateStageTwo(){
	for(u8 y = 0; y < GAME_TILE_H; y++){

		// foreground
		if(y >= STAGE_TWO_TREES_Y && y < STAGE_TWO_TREES_Y + STAGE_TWO_TREES_H){
			bgScrollsB[y]--;
			if(bgScrollsB[y] <= STAGE_TWO_TREES2_LIMIT) bgScrollsB[y] -= STAGE_TWO_TREES_LIMIT;
		}

		// background
		if(y >= STAGE_TWO_TREES2_Y && y < STAGE_TWO_TREES2_Y + STAGE_TWO_TREES2_H && bgClock % 10 == 0){
			bgScrollsA[y]--;
			if(bgScrollsA[y] <= STAGE_TWO_TREES2_LIMIT) bgScrollsA[y] -= STAGE_TWO_TREES2_LIMIT;
		}
	}

	VDP_setHorizontalScrollTile(BG_A, 0, bgScrollsA, GAME_TILE_H, DMA);
	VDP_setHorizontalScrollTile(BG_B, 0, bgScrollsB, GAME_TILE_H, DMA);
}


// stage three

#define STAGE_THREE_SKY_Y 4
#define STAGE_THREE_SKY_H 11
#define STAGE_THREE_SKY_W 32
#define STAGE_THREE_SKY_LIMIT STAGE_THREE_SKY_W * -8

#define STAGE_THREE_MIDDLE_Y STAGE_THREE_SKY_Y + STAGE_THREE_SKY_H - 3
#define STAGE_THREE_MIDDLE_H 6
#define STAGE_THREE_MIDDLE_W 13
#define STAGE_THREE_MIDDLE_LIMIT STAGE_THREE_MIDDLE_W * -8

static void loadStageThree(){

	// sky
	VDP_loadTileSet(stageThreeSky2.tileset, BG_I, DMA);
	VDP_loadTileSet(stageThreeSky3.tileset, BG_I + 1, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I), 0, 0, GAME_TILE_W, STAGE_THREE_SKY_Y);
	for(u8 x = 0; x < 2; x++) VDP_drawImageEx(BG_B, &stageThreeSky, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I + 2), x * STAGE_THREE_SKY_W, STAGE_THREE_SKY_Y, 0, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I + 1), 0, STAGE_THREE_SKY_Y + STAGE_THREE_SKY_H, GAME_TILE_W, 2);

	// middle
	for(u8 x = 0; x < 4; x++) VDP_drawImageEx(BG_A, &stageThreeMiddle, TILE_ATTR_FULL(PAL2, 1, 0, 0, BG_I + 2 + 352), x * STAGE_THREE_MIDDLE_W, STAGE_THREE_MIDDLE_Y, 0, DMA);
}

static void updateStageThree(){
	for(u8 y = 0; y < GAME_TILE_H; y++){

		// sky
		if(y >= STAGE_THREE_SKY_Y && y < STAGE_THREE_SKY_Y + STAGE_THREE_SKY_H && bgClock % 5 == 0){
			bgScrollsB[y]--;
			if(bgScrollsB[y] <= STAGE_THREE_SKY_LIMIT) bgScrollsB[y] -= STAGE_THREE_SKY_LIMIT;
		}

		// middle
		if(y >= STAGE_THREE_MIDDLE_Y && y < STAGE_THREE_MIDDLE_Y + STAGE_THREE_MIDDLE_H){
			bgScrollsA[y]--;
			if(bgScrollsA[y] <= STAGE_THREE_MIDDLE_LIMIT) bgScrollsA[y] -= STAGE_THREE_MIDDLE_LIMIT;
		}
	}
	VDP_setHorizontalScrollTile(BG_A, 0, bgScrollsA, GAME_TILE_H, DMA);
	VDP_setHorizontalScrollTile(BG_B, 0, bgScrollsB, GAME_TILE_H, DMA);
}


// loop

void loadBackground(){
	VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);
	loadStageOne();
	// loadStageTwo();
	// loadStageThree();
}

void updateBackground(){
	if(bgClock % 2 == 1) updateStageOne();
	// updateStageTwo();
	// updateStageThree();
	bgClock++;
	if(bgClock >= CLOCK_LIMIT) bgClock -= CLOCK_LIMIT;
}