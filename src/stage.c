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
		enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.15));
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

void spawnBoss(s16 health, void(*bossUpdater)){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.25),
		.y = GAME_H / 2,
		.health = health,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.x = GAME_W + 12,
		.seal = TRUE
	};
	bossMax = spawner.health;
	void updater(s16 i){
		bossFlyIn(i);
		if(enemies[i].bools[1]){
			if(bossHealth != enemies[i].health) bossHealth = enemies[i].health;
			enemies[i].bossUpdater(i);
			// if((enemies[i].clock >= 0 && enemies[i].clock % 240 < 120) || enemies[i].clock % 240 < 0){
			// 	if(enemies[i].clock % 120 == 0){
			// 		enemies[i].speed = 0;
			// 		updateEnemyVel(i);
			// 	}
			// 	if(enemies[i].clock >= 0) enemies[i].bossUpdater(i);
			// } else {
			// 	if(enemies[i].clock % 240 == 120){
			// 		enemies[i].speed = FIX16(0.8);
			// 		enemies[i].angle = random() % 256 + 128;
			// 		if(enemies[i].clock % 480 == 120) enemies[i].angle += 512;
			// 		updateEnemyVel(i);
			// 	} else if(enemies[i].clock % 240 == 150){
			// 		enemies[i].speed = FIX16(0.6);
			// 		updateEnemyVel(i);
			// 	} else if(enemies[i].clock % 240 == 180){
			// 		enemies[i].speed = FIX16(0.4);
			// 		updateEnemyVel(i);
			// 	} else if(enemies[i].clock % 240 == 210){
			// 		enemies[i].speed = FIX16(0.2);
			// 		updateEnemyVel(i);
			// 	}
			// 	bossMove(i);
			// }
		}
	}
	void suicide(s16 i){
		killBullets = TRUE;
		waveClock = 60;
	}
	spawnEnemy(spawner, updater, suicide, bossUpdater);
}


// loop

void loadStage(){
	loadStageOne();
}

void updateStage(){
	updateStageOne();
	waveClock--;
}