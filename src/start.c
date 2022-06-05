// start

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "start.h"
#include "chrome.h"
#include "cutscenes.h"


// intro cutscene

#define INTRO_I 32
#define INTRO_X 0
#define INTRO_Y 6

// static void loadScarletDevilMansion(){
// 	VDP_loadTileSet(scarletDevilMansionTop.tileset, INTRO_I, DMA);
// 	VDP_loadTileSet(scarletDevilMansionBottom.tileset, INTRO_I + 1, DMA);
// 	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, INTRO_I), 0, 0, GAME_TILE_W, INTRO_Y);
// 	VDP_drawImageEx(BG_B, &scarletDevilMansion, TILE_ATTR_FULL(PAL2, 0, 0, 0, INTRO_I + 2), INTRO_X, INTRO_Y, 0, DMA);
// 	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, INTRO_I + 1), 0, INTRO_Y + 16, GAME_TILE_W, INTRO_Y);
// }


// main loop

void loadStart(){
	loadGame();
}

void updateStart(){
	updateChrome();
}