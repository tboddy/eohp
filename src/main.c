// main

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "background.h"
#include "bullets.h"
#include "stage.h"
#include "enemies.h"
#include "players.h"
#include "chrome.h"
#include "start.h"
#include "explosion.h"
#include "powerups.h"

// resources

static void loadResources(){
	VDP_loadFont(font.tileset, DMA);
	VDP_setPalette(PAL1, half.palette -> data);
	VDP_setPalette(PAL2, least.palette -> data);
	VDP_setTextPalette(1);
	VDP_loadTileSet(least.tileset, LEAST_TILE, DMA);
	VDP_loadTileSet(half.tileset, HALF_TILE, DMA);
	VDP_loadTileSet(most.tileset, MOST_TILE, DMA);
	VDP_loadTileSet(full.tileset, FULL_TILE, DMA);
}

void EMPTY(u8 i){}

Vect2D_f16 hone(Vect2D_f16 pos, Vect2D_f16 target, fix16 speed, s16 lerp){
	hPos.x = target.x;
	hPos.y = target.y;
	if(lerp > 0){
		hPos.x = fix16Add(fix16Sub(hPos.x, FIX16(lerp)), FIX16(random() % (lerp * 2)));
		hPos.y = fix16Add(fix16Sub(hPos.y, FIX16(lerp)), FIX16(random() % (lerp * 2)));
	}
	honeSpeed = fix32ToFix16(getApproximatedDistance(
		fix32Sub(fix16ToFix32(pos.x), fix16ToFix32(hPos.x)),
		fix32Sub(fix16ToFix32(pos.y), fix16ToFix32(hPos.y))));

	hPos.x = fix16Div(fix16Sub(hPos.x, pos.x), honeSpeed);
	hPos.y = fix16Div(fix16Sub(hPos.y, pos.y), honeSpeed);

	// wtf
	// hPos.x *= -1;

	hPos.x = fix16Mul(hPos.x, speed);
	hPos.y = fix16Mul(hPos.y, speed);

	return hPos;
}

// game loop

static void loadInternals(){
	JOY_init();
	JOY_setEventHandler(&updateControls);
	loadResources();
	SPR_init(127, 0, 0);
	VDP_setScreenWidth256();
}

void loadGame(){
	started = TRUE;
	VDP_clearPlane(BG_A, TRUE);
	VDP_clearPlane(BG_B, TRUE);
	loadChrome();
	loadBackground();
	loadPlayers();
	loadStage();
}

static void updateGame(){
	updateBackground();
	updateStage();
	updateEnemies();
	updatePlayers();
	updateBullets();
	updateChrome();	
	updateDialogue();
	updateCutscenes();
	updateExplosions();
	updatePowerups();
	clock++;
	if(clock >= CLOCK_LIMIT) clock -= CLOCK_LIMIT;
}


// main loop

int main(){
	loadInternals();
	// loadGame();
	loadStart();
	while(1){
		started ? updateGame() : updateStart();
		SPR_update();
		SYS_doVBlankProcess();
	}
	return(0);
}