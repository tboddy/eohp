// dialogue

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "dialogue.h"


#define DIALOGUE_X_LEFT 10
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
#define DIALOGUE_IMAGE_Y GAME_H - 72

#define DIALOGUE_IMAGE_FACE_Y DIALOGUE_IMAGE_Y + 13 + 8

s16 dialogueImageX, dialogueImageY, dialogueFaceY, dialogueFaceX;

Image* dialogueGirl;
Image* dialogueGirlFace;

Sprite* dialogueImage;
Sprite* dialogueImageFace;

void dialogue(bool right, char *speaker, char *line1, char *line2, char *line3, char *line4, u8 girl, u8 face, bool last){
	dialogueX = right ? DIALOGUE_X_RIGHT : DIALOGUE_X_LEFT;
	dialogueEndX = right ? DIALOGUE_END_X_RIGHT : DIALOGUE_END_X_LEFT;
	dialogueImageX = right ? DIALOGUE_IMAGE_X_RIGHT : DIALOGUE_IMAGE_X_LEFT;
	dialogueImageY = DIALOGUE_IMAGE_Y;
	dialogueFaceX = dialogueImageX + (right ? 10 : 13);
	dialogueFaceY = DIALOGUE_IMAGE_FACE_Y;
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
	VDP_drawText(last ? "_" : "^", dialogueEndX, DIALOGUE_Y + 3);

	switch(girl){
		case 0:
			dialogueGirl = &dialogueFlandre;
			// dialogueImageY -= 8;
			dialogueFaceX += 12;
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
			dialogueGirlFace = &dialogueFaceHappy3;
			break;
		case 3:
			dialogueGirlFace = &dialogueFaceSurprised;
			break;
		case 4:
			dialogueGirlFace = &dialogueFaceShy;
			break;
		case 5:
			dialogueGirlFace = &dialogueFaceIndifferent;
			break;
	}

	dialogueImage = SPR_addSprite(dialogueGirl, dialogueImageX, dialogueImageY, TILE_ATTR(PAL1, 1, 0, !right));
	dialogueImageFace = SPR_addSprite(dialogueGirlFace, dialogueFaceX, dialogueFaceY, TILE_ATTR(PAL1, 1, 0, !right));

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

void updateDialogue(){
	if(dialogueActive){
		if(!ctrl.a && !ctrl.b && !ctrl.c && !ctrl.start && dialoguePressed) dialoguePressed = FALSE;
		else if((ctrl.a || ctrl.b || ctrl.c || ctrl.start) && !dialoguePressed) clearDialogue();
	}
}