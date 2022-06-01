// chrome

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "chrome.h"
#include "controls.h"
// #include "dialogue.h"


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


// dialogue

#define DIALOGUE_X_LEFT 8
#define DIALOGUE_X_RIGHT 1
#define DIALOGUE_Y 23
#define DIALOGUE_END_X_LEFT 30
#define DIALOGUE_END_X_RIGHT 23

s16 dialogueX;
s16 dialogueEndX;

char dialogueName[12];
char dialogueRow1[DIALOGUE_LENGTH];
char dialogueRow2[DIALOGUE_LENGTH];
char dialogueRow3[DIALOGUE_LENGTH];
char dialogueRow4[DIALOGUE_LENGTH];

#define DIALOGUE_IMAGE_X_LEFT 0
#define DIALOGUE_IMAGE_X_RIGHT GAME_W - 56
#define DIALOGUE_IMAGE_Y GAME_H - 64

#define DIALOGUE_IMAGE_FACE_Y DIALOGUE_IMAGE_Y + 13

s16 dialogueImageX;

Image* dialogueGirl;
Image* dialogueGirlFace;

Sprite* dialogueImage;
Sprite* dialogueImageFace;

void dialogue(bool right, char *speaker, char *line1, char *line2, char *line3, char *line4, u8 girl, u8 face){
	dialogueX = right ? DIALOGUE_X_RIGHT : DIALOGUE_X_LEFT;
	dialogueEndX = right ? DIALOGUE_END_X_RIGHT : DIALOGUE_END_X_LEFT;
	dialogueImageX = right ? DIALOGUE_IMAGE_X_RIGHT : DIALOGUE_IMAGE_X_LEFT;
	strcpy(dialogueName, speaker);
	strcpy(dialogueRow1, line1);
	strcpy(dialogueRow2, line2);
	strcpy(dialogueRow3, line3);
	strcpy(dialogueRow4, line4);
	VDP_drawText(dialogueName, dialogueX, DIALOGUE_Y - 2);
	VDP_drawText(dialogueRow1, dialogueX, DIALOGUE_Y);
	VDP_drawText(dialogueRow2, dialogueX, DIALOGUE_Y + 1);
	VDP_drawText(dialogueRow3, dialogueX, DIALOGUE_Y + 2);
	VDP_drawText(dialogueRow4, dialogueX, DIALOGUE_Y + 3);
	VDP_drawText("^", dialogueEndX, DIALOGUE_Y + 3);

	switch(girl){
		case 0:
			dialogueGirl = &dialogueWriggle;
			break;
		case 1:
			dialogueGirl = &dialogueWriggle;
			break;
	}

	switch(face){
		case 0:
			dialogueGirlFace = &dialogueFaceHappy1;
			break;
		case 1:
			dialogueGirlFace = &dialogueFaceHappy2;
			break;
		case 2:
			dialogueGirlFace = &dialogueFaceSurprised;
			break;
	}

	dialogueImage = SPR_addSprite(dialogueGirl, dialogueImageX, DIALOGUE_IMAGE_Y, TILE_ATTR(PAL1, 0, 0, !right));
	dialogueImageFace = SPR_addSprite(dialogueGirlFace, dialogueImageX + (right ? 11 : 13), DIALOGUE_IMAGE_FACE_Y, TILE_ATTR(PAL1, 0, 0, !right));

	SPR_setDepth(dialogueImage, 2);
	SPR_setDepth(dialogueImageFace, 1);
	dialogueActive = TRUE;
	dialoguePressed = TRUE;
}

static void clearDialogue(){
	SPR_releaseSprite(dialogueImage);
	SPR_releaseSprite(dialogueImageFace);
	VDP_clearTextArea(0, DIALOGUE_Y - 2, GAME_TILE_W, 6);
	dialogueActive = FALSE;
}

static void updateDialogue(){
	if(!ctrl.a && !ctrl.b && !ctrl.c && !ctrl.start && dialoguePressed) dialoguePressed = FALSE;
	else if((ctrl.a || ctrl.b || ctrl.c || ctrl.start) && !dialoguePressed) clearDialogue();
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
	if(dialogueActive) updateDialogue();
	// VDP_drawText(debugStr1, 0, 0);
}