// players

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "bullets.h"
#include "stage.h"
#include "players.h"
#include "enemies.h"
#include "chrome.h"
#include "cutscenes.h"
#include "stageone.h"


// moving

#define BOSS_LIMIT_X FIX16(GAME_W / 4 * 3)
#define BOSS_LIMIT_W FIX16(GAME_W - 32)
#define BOSS_LIMIT_Y FIX16(64)
#define BOSS_LIMIT_H FIX16(GAME_H - 48)

void bossMove(s16 i){
	if(enemies[i].pos.y > BOSS_LIMIT_H || enemies[i].pos.y < BOSS_LIMIT_Y){
		enemies[i].pos.y = enemies[i].pos.y > BOSS_LIMIT_H ? BOSS_LIMIT_H : BOSS_LIMIT_Y;
		enemies[i].angle = 1024 - enemies[i].angle;
		updateEnemyVel(i);
	}
	if(enemies[i].pos.x > BOSS_LIMIT_W || enemies[i].pos.x < BOSS_LIMIT_X){
		enemies[i].pos.x = enemies[i].pos.x > BOSS_LIMIT_W  ? BOSS_LIMIT_W : BOSS_LIMIT_X;
		enemies[i].angle = 512 - enemies[i].angle;
		updateEnemyVel(i);
	}
}

void transferBoss(s16 i){
	if(enemies[i].clock % 120 == 60){
		enemies[i].speed = FIX16(1);
		updateEnemyVel(i);
	} else if(enemies[i].clock % 120 == 70){
		enemies[i].speed = FIX16(1.5);
		updateEnemyVel(i);
	} else if(enemies[i].clock % 120 == 90){
		enemies[i].speed = FIX16(1);
		updateEnemyVel(i);
	} else if(enemies[i].clock % 120 == 100){
		enemies[i].speed = FIX16(0.5);
		updateEnemyVel(i);
	} else if(enemies[i].clock % 120 == 110){
		enemies[i].speed = FIX16(0);
		updateEnemyVel(i);
	}
	bossMove(i);
}

void bossFlyIn(s16 i){
	// fly in
	if(!enemies[i].bools[0] && enemies[i].clock % 10 == 5){
		enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.2));
		if(enemies[i].speed <= 0){
			enemies[i].clock = 0;
			enemies[i].speed = 0;
			enemies[i].bools[0] = TRUE;
		}
		updateEnemyVel(i);
	}
	// and wait
	if(enemies[i].bools[0] && !enemies[i].bools[1])
		if(enemies[i].clock >= 10){
			enemies[i].clock = 0;
			enemies[i].angle = 256 + random() % 128;
			enemies[i].bools[1] = TRUE;
		}
}


// loop

void loadStage(){
	loadStageOne();
}

void updateStage(){
	updateStageOne();
	waveClock--;
}