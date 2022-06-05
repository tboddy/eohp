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


// struct bulletSpawner bSpawner, bSpawner2;


// wriggle

// static void wriggleNonOne(s16 i){
// 	if(enemies[i].clock % 120 < 60 && enemies[i].clock % 10 == 0){
// 		if(enemies[i].clock % 120 == 0){
// 			enemies[i].speed = 0;
// 			updateEnemyVel(i);
// 			bSpawner.x = enemies[i].pos.x;
// 			bSpawner.y = enemies[i].pos.y;
// 			bSpawner.image = &smallGreenBullet;
// 			bSpawner.speed = FIX16(3);
// 			bSpawner.ints[0] = 96;
// 			enemies[i].ints[0] = 256 + 102;
// 		}
// 		bSpawner.angle = enemies[i].ints[0];
// 		void updater(s16 j){
// 			if(bullets[j].clock % 40 == 10 || bullets[j].clock % 40 == 0){
// 				bullets[j].angle -= bSpawner.ints[0];
// 				updateBulletVel(j);
// 			} else if(bullets[j].clock % 40 == 20 || bullets[j].clock % 40 == 30){
// 				bullets[j].angle += bSpawner.ints[0];
// 				updateBulletVel(j);
// 			}
// 		}
// 		for(u8 j = 1; j < 5; j++){
// 			bSpawner.angle += 102;
// 			spawnBullet(bSpawner, updater);
// 		}
// 	} else if(enemies[i].clock % 120 >= 60)
// 		transferBoss(i);
// }

// static void wriggleNonTwo(s16 i){
// 	wriggleNonOne(i);
// 	if(enemies[i].clock % 120 < 35 && enemies[i].clock % 6 == 3){
// 		if(enemies[i].clock % 120 == 3){
// 			updateEnemyVel(i);
// 			bSpawner2.x = enemies[i].pos.x;
// 			bSpawner2.y = enemies[i].pos.y;
// 			bSpawner2.image = &bigYellowBullet;
// 			bSpawner2.big = TRUE;
// 			bSpawner2.speed = FIX16(3);
// 			bSpawner2.ints[0] = 512 - 32 + random() % 32;
// 		}
// 		bSpawner2.angle = bSpawner2.ints[0] - 128;
// 		for(u8 j = 1; j < 4; j++){
// 			spawnBullet(bSpawner2, EMPTY);
// 			bSpawner2.angle += 128;
// 		}
// 	}
// }




// cirno

// static void cirnoNonBarf(s16 i){
// 	if(enemies[i].clock % 120 == 0){
// 		bSpawner.x = enemies[i].pos.x;
// 		bSpawner.y = enemies[i].pos.y;
// 		enemies[i].fixes[2] = 0;
// 	}
// 	if(enemies[i].clock % 120 < 90){
// 		if(enemies[i].clock % 2 == 1){
// 			bSpawner.angle = 320 + random() % 384;
// 			bSpawner.image = &smallBlueBullet;
// 			bSpawner.big = FALSE;
// 			bSpawner.speed = FIX16(1.5 + enemies[i].clock % 120 / 20);
// 			void updater(s16 j){
// 				if(bullets[j].speed > FIX16(2) && bullets[j].clock % 10 == 5){
// 					bullets[j].speed = fix16Sub(bullets[j].speed, FIX16(0.25));
// 					updateBulletVel(j);
// 				}
// 			}
// 			spawnBullet(bSpawner, updater);
// 		}
// 	} else if(enemies[i].clock % 120 >= 60) transferBoss(i);
// }

// static void cirnoNonRay(s16 i, bool alt){
// 	if(enemies[i].clock % 120 < 60){
// 		if(enemies[i].clock % (alt ? 6 : 10) == 5){
// 			bSpawner.image = &bigBlueBullet;
// 			bSpawner.big = TRUE;
// 			bSpawner.speed = FIX16(3);
// 			velPos.x = bSpawner.x;
// 			velPos.y = bSpawner.y;
// 			bSpawner.vel = hone(velPos, players[0].pos, bSpawner.speed, 64);
// 			spawnBullet(bSpawner, EMPTY);
// 			enemies[i].fixes[2] = fix16Add(enemies[i].fixes[2], FIX16(0.25));
// 		}
// 	} 
// }

// static void cirnoNonOne(s16 i){
// 	cirnoNonBarf(i);
// 	cirnoNonRay(i, FALSE);
// }

// static void cirnoNonTwo(s16 i){
// 	cirnoNonBarf(i);
// 	// cirnoNonRay(i, TRUE);
// }

// void spawnCirno(){
// 	struct enemySpawner spawner = {
// 		.angle = 512,
// 		.speed = FIX16(1.4),
// 		.y = GAME_H / 2,
// 		.health = 100,
// 		.image = &test,
// 		.offX = 16,
// 		.offY = 24,
// 	};
// 	spawner.x = GAME_W + spawner.offX;
// 	void updater(s16 i){
// 		bossFlyIn(i);
// 		if(enemies[i].bools[1]){
// 			cirnoNonTwo(i);
// 		}
// 	}
// 	spawnEnemy(spawner, updater, EMPTY);
// }


// 


// loop

void loadStage(){
	loadStageOne();
}

void updateStage(){
	updateStageOne();
	waveClock--;
}