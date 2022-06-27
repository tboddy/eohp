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
#include "boss.h"
#include "bricks.h"


// moving

// #define BOSS_LIMIT_X FIX16(GAME_W / 4 * 3)
// #define BOSS_LIMIT_W FIX16(GAME_W - 32)
// #define BOSS_LIMIT_Y FIX16(64)
// #define BOSS_LIMIT_H FIX16(GAME_H - 48)

// void bossMove(s16 i){
// 	if(enemies[i].pos.y > BOSS_LIMIT_H || enemies[i].pos.y < BOSS_LIMIT_Y){
// 		enemies[i].pos.y = enemies[i].pos.y > BOSS_LIMIT_H ? BOSS_LIMIT_H : BOSS_LIMIT_Y;
// 		enemies[i].angle = 1024 - enemies[i].angle;
// 		updateEnemyVel(i);
// 	}
// 	if(enemies[i].pos.x > BOSS_LIMIT_W || enemies[i].pos.x < BOSS_LIMIT_X){
// 		enemies[i].pos.x = enemies[i].pos.x > BOSS_LIMIT_W  ? BOSS_LIMIT_W : BOSS_LIMIT_X;
// 		enemies[i].angle = 512 - enemies[i].angle;
// 		updateEnemyVel(i);
// 	}
// }

// void transferBoss(s16 i){
// 	if(enemies[i].clock % 120 == 60){
// 		enemies[i].speed = FIX16(1);
// 		updateEnemyVel(i);
// 	} else if(enemies[i].clock % 120 == 70){
// 		enemies[i].speed = FIX16(1.5);
// 		updateEnemyVel(i);
// 	} else if(enemies[i].clock % 120 == 90){
// 		enemies[i].speed = FIX16(1);
// 		updateEnemyVel(i);
// 	} else if(enemies[i].clock % 120 == 100){
// 		enemies[i].speed = FIX16(0.5);
// 		updateEnemyVel(i);
// 	} else if(enemies[i].clock % 120 == 110){
// 		enemies[i].speed = FIX16(0);
// 		updateEnemyVel(i);
// 	}
// 	bossMove(i);
// }



// void spawnBoss(s16 health, void(*bossUpdater)){
// 	struct enemySpawner spawner = {
// 		.angle = 256,
// 		.speed = FIX16(1.3),
// 		.y = -16,
// 		.health = health,
// 		.image = &cirno,
// 		.offX = 16,
// 		.offY = 16,
// 		.x = GAME_W / 2,
// 		.seal = TRUE
// 	};
// 	bossMax = spawner.health;
// 	void updater(s16 i){
// 		bossFlyIn(i);
// 		if(enemies[i].bools[1]){
// 			if(bossHealth != enemies[i].health) bossHealth = enemies[i].health;
// 			enemies[i].bossUpdater(i);
// 		}
// 	}
// 	void suicide(s16 i){
// 		killBullets = TRUE;
// 		waveClock = 60;
// 	}
// 	spawnEnemy(spawner, updater, suicide, bossUpdater);
// }


// loop

void loadStage(){
	loadBoss();
	// loadBricks();
}

void updateStage(){
	updateBoss();
	updateBricks();

	// waveClock--;
}