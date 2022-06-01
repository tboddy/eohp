// start

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "start.h"
#include "chrome.h"


// intro cutscene

#define INTRO_I 32
#define INTRO_X 0
#define INTRO_Y 6

static void loadScarletDevilMansion(){
	VDP_loadTileSet(scarletDevilMansionTop.tileset, INTRO_I, DMA);
	VDP_loadTileSet(scarletDevilMansionBottom.tileset, INTRO_I + 1, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, INTRO_I), 0, 0, GAME_TILE_W, INTRO_Y);
	VDP_drawImageEx(BG_B, &scarletDevilMansion, TILE_ATTR_FULL(PAL2, 0, 0, 0, INTRO_I + 2), INTRO_X, INTRO_Y, 0, DMA);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, INTRO_I + 1), 0, INTRO_Y + 16, GAME_TILE_W, INTRO_Y);
	dialogue(FALSE, "FLANDRE",
		"That was a nice sleep,",
		"I should go see what",
		"everyone is up to!",
		"", 0, 0);
}

u8 currentIntroDialogue;

static void updateIntroDialogue(){
	if(!dialogueActive){
		switch(currentIntroDialogue){
			case 0:
				dialogue(TRUE, "FLANDRE",
					"Sakuyaaaaaaa! Ya got",
					"any pudding?",
					"", "", 0, 1);
				break;
			case 1:
				dialogue(FALSE, "FLANDRE",
					"...I couldn't find",
					"anyone in the mansion.",
					"", "", 0, 2);
				break;
			case 2:
				dialogue(TRUE, "FLANDRE",
					"They wouldn't just",
					"leave me alone...",
					"Right?",
					"", 0, 1);
				break;
			case 3:
				dialogue(FALSE, "FLANDRE",
					"Hmph, I guess I'll",
					"have to go find them,",
					"then.",
					"", 0, 0);
				break;
			case 4:
				loadGame();
				break;
		}
		currentIntroDialogue++;
	}
}


// main loop

void loadStart(){
	loadScarletDevilMansion();
}

void updateStart(){
	updateChrome();
	updateIntroDialogue();
}