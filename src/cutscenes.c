// cutscenes

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "dialogue.h"
#include "cutscenes.h"
#include "stage.h"

u8 currentCutscene, currentDialogue;
bool dialogueActive;

// cutscene one

static void loadIntroCutsceneOne(){
	dialogue(FALSE, "FLANDRE",
		"That was a nice sleep,",
		"I should go see what",
		"everyone is up to!",
		"", 0, 2, FALSE);
}

static void updateIntroCutsceneOne(){
	if(!dialogueActive){
		switch(currentDialogue){
			case 0:
				dialogue(FALSE, "FLANDRE",
					"Sakuyaaaaaaa! Ya got",
					"any pudding?",
					"", "", 0, 1, FALSE);
				break;
			case 1:
				dialogue(FALSE, "FLANDRE",
					"...I couldn't find",
					"anyone in the mansion.",
					"", "", 0, 2, FALSE);
				break;
			case 2:
				dialogue(FALSE, "FLANDRE",
					"They wouldn't just",
					"leave me alone...",
					"Right?",
					"", 0, 1, FALSE);
				break;
			case 3:
				dialogue(FALSE, "FLANDRE",
					"Hmph, I guess I'll",
					"have to go find them,",
					"then.",
					"", 0, 0, TRUE);
				break;
			case 4:
				inCutscene = FALSE;
				break;
		}
		currentDialogue++;
	}
}


// stage one midboss

static void updateStageOneCutsceneOne(){
	if(!dialogueActive){
		switch(currentDialogue){

			case 0: dialogue(TRUE, "WRIGGLE",
				"Hey you, stop right",
				"there!",
				"", "", 1, 2, FALSE); break;

			case 1: dialogue(FALSE, "FLANDRE",
				"...",
				"", "", "", 0, 2, FALSE); break;

			case 2: dialogue(TRUE, "WRIGGLE",
				"Thank you so much!",
				"", "", "", 1, 2, FALSE); break;

			case 3: dialogue(FALSE, "FLANDRE",
				"... What for?",
				"", "", "", 0, 2, FALSE); break;

			case 4: dialogue(TRUE, "WRIGGLE",
				"You're the first one",
				"to enough to look",
				"around and be able to",
				"hear me.", 1, 2, FALSE); break;

			case 5: dialogue(FALSE, "FLANDRE",
				"You've been here",
				"all day?",
				"", "", 0, 2, FALSE); break;

			case 6: dialogue(TRUE, "WRIGGLE",
				"Uh huh!",
				"", "", "", 1, 2, FALSE); break;

			case 7: dialogue(FALSE, "FLANDRE",
				"Did you see a pink",
				"lady, a maid, or a",
				"Chinese lady?",
				"", 0, 2, FALSE); break;

			case 8: dialogue(TRUE, "WRIGGLE",
				"Nuh uh.",
				"", "", "", 1, 2, FALSE); break;

			case 9: dialogue(FALSE, "FLANDRE",
				"Aww shoot.",
				"", "", "", 0, 2, FALSE); break;

			case 10: dialogue(TRUE, "WRIGGLE",
				"I'm sorry.",
				"", "", "", 1, 2, FALSE); break;

			case 11: dialogue(TRUE, "WRIGGLE",
				"WAIT!",
				"", "", "", 1, 2, FALSE); break;

			case 12: dialogue(TRUE, "WRIGGLE",
				"I've been wanting to",
				"look at the moon with",
				"someone here at the",
				"lake all day!", 1, 2, FALSE); break;

			case 13: dialogue(TRUE, "WRIGGLE",
				"You're the only one",
				"who's stopped...", "", "", 1, 2, FALSE); break;

			case 14: dialogue(TRUE, "WRIGGLE",
				"Can you stay here and",
				"wait for them with",
				"me? I've been awful",
				"lonely all day, and", 1, 2, FALSE); break;

			case 15: dialogue(TRUE, "WRIGGLE",
				"my friends are all",
				"busy...",
				"", "", 1, 2, FALSE); break;

			case 16: dialogue(TRUE, "WRIGGLE",
				"PLEASE! I don't want",
				"to waste such a",
				"lovely night all",
				"alone...", 1, 2, FALSE); break;

			case 17: dialogue(FALSE, "FLANDRE",
				"... I really can't.",
				"", "", "", 0, 2, FALSE); break;

			case 18: dialogue(TRUE, "WRIGGLE",
				"... Okay then. I'll",
				"let you go, but under",
				"one condition.",
				"", 1, 2, FALSE); break;

			case 19: dialogue(FALSE, "FLANDRE",
				"Hmm?",
				"", "", "", 0, 2, FALSE); break;

			case 20: dialogue(TRUE, "WRIGGLE",
				"Fight me! If I win,",
				"You stay here. If you",
				"win, I'll let you by.",
				"", 1, 2, FALSE); break;

			case 21: dialogue(FALSE, "FLANDRE",
				"You're on!",
				"", "", "", 0, 2, FALSE); break;

			case 22: dialogue(TRUE, "WRIGGLE",
				"(... Uh oh, I didn't",
				"think she'd say yes",
				"so easily...)",
				"", 1, 2, FALSE); break;

			case 23:
				inCutscene = FALSE;
				// spawnWriggle();
				waveClock = 1;
				break;
		}
		currentDialogue++;
	}
}


// main loop

void loadCutscene(u8 index){
	currentDialogue = 0;
	currentCutscene = index;
	inCutscene = TRUE;
}

void updateCutscenes(){
	if(inCutscene){
		switch(currentCutscene){
			case 0: updateStageOneCutsceneOne(); break;
		}
	}
}