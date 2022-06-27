// bosses

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "bullets.h"
#include "stage.h"
#include "players.h"
#include "enemies.h"
#include "cutscenes.h"
#include "boss.h"


#define BOSS_MOVE_INTERVAL 240
#define BOSS_MOVE_LIMIT 240 - 30
#define BOSS_MOVE_MAX 960
#define BOSS_MOVE_INTERVAL_Y 480

u8 currentBoss = 0;


// cirno

static void cirnoOne(s16 i){
	if(enemies[i].clock % 90 == 0){
		struct bulletSpawner spawner = {
			.angle = 0,
			.speed = FIX16(2),
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.image = &bigBlueBullet,
			.big = TRUE
		};
		spawner.ints[0] = enemies[i].clock % 180 == 0 ? 12 : -12;
		void updater(s16 j){
			if(bullets[j].clock > 0 && bullets[j].clock % 5 == 0 && bullets[j].clock < 60){
				bullets[j].angle += bullets[j].ints[0];
				updateBulletVel(j);
			}
		}
		for(u8 j = 0; j < 5; j++){
			spawner.angle += 85;
			spawnBullet(spawner, updater);
		}
	}
}

static void cirnoTwo(s16 i){
	if(enemies[i].clock % 30 == 0){
		struct bulletSpawner spawner = {
			.angle = random() % 256,
			.speed = FIX16(2),
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.big = TRUE
		};
		if(enemies[i].clock % BOSS_MOVE_INTERVAL < 120){
			enemies[i].ints[3] = 80;
			spawner.ints[0] = 20 + random() % 20;
			spawner.ints[1] = spawner.ints[0] + 30;
			switch(enemies[i].clock % 120){
				case 0: spawner.image = &bigRedBullet; break;
				case 30: spawner.image = &bigBlueBullet; break;
				case 60: spawner.image = &bigGreenBullet; break;
				case 90: spawner.image = &bigYellowBullet; break;
			}
			if(enemies[i].clock % 60 == 0) spawner.angle += 256;
			void updater(s16 j){
				if(bullets[j].clock == bullets[j].ints[0]){
					SPR_setDefinition(bullets[j].image, &bigWhiteBullet);
					bullets[j].speed = 0;
					updateBulletVel(j);
				} else if(bullets[j].clock >= bullets[j].ints[1]){
					if(bullets[j].clock == bullets[j].ints[1]){
						bullets[j].angle = random() % 512;
						bullets[j].speed = FIX16(0.5);
						updateBulletVel(j);
					} else if(bullets[j].clock % 15 == 0 && bullets[j].speed < FIX16(2)){
						bullets[j].speed = fix16Add(bullets[j].speed, FIX16(0.5));
						updateBulletVel(j);
					}
				}
			}
			spawnBullet(spawner, updater);
		} else {
			spawner.image = &bigBlueBullet;
			enemies[i].ints[3] += 88;
			spawner.angle = enemies[i].ints[3];
			spawnBullet(spawner, EMPTY);
		}
	}
}


// shoot

static void bossShoot(s16 i){
	switch(currentBoss){
		case 0:
			enemies[i].health <= 8 ? cirnoTwo(i) : cirnoOne(i);
			break;
	}
}


// movement

static void bossFlyIn(s16 i){
	if(!enemies[i].bools[0] && enemies[i].clock % 10 == 5){
		enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.15));
		if(enemies[i].speed <= 0){
			enemies[i].clock = 0;
			enemies[i].speed = 0;
			enemies[i].bools[0] = TRUE;
		}
		updateEnemyVel(i);
	}
	if(enemies[i].bools[0] && !enemies[i].bools[1]){
		if(enemies[i].clock >= 10){
			enemies[i].clock = -1;
			enemies[i].bools[1] = TRUE;
		}
	}
}

#define BOSS_SPEED_X FIX16(3)
#define BOSS_SPEED_Y FIX16(0.25)
fix16 bossSpeedMul;

static void bossMove(s16 i){
	if(enemies[i].clock % BOSS_MOVE_INTERVAL >= BOSS_MOVE_LIMIT){
		if(enemies[i].clock % BOSS_MOVE_INTERVAL < BOSS_MOVE_LIMIT + 10 || enemies[i].clock % BOSS_MOVE_INTERVAL > BOSS_MOVE_LIMIT + 20)
			bossSpeedMul = FIX16(0.5);
		else bossSpeedMul = FIX16(1);
		enemies[i].pos.x = enemies[i].clock % BOSS_MOVE_MAX < BOSS_MOVE_INTERVAL || (enemies[i].clock % BOSS_MOVE_MAX >= BOSS_MOVE_INTERVAL * 3) ?
			fix16Add(enemies[i].pos.x, fix16Mul(BOSS_SPEED_X, bossSpeedMul)) : fix16Sub(enemies[i].pos.x, fix16Mul(BOSS_SPEED_X, bossSpeedMul));
		enemies[i].pos.y = enemies[i].clock % BOSS_MOVE_MAX < BOSS_MOVE_INTERVAL_Y ?
			fix16Add(enemies[i].pos.y, fix16Mul(BOSS_SPEED_Y, bossSpeedMul)) : fix16Sub(enemies[i].pos.y, fix16Mul(BOSS_SPEED_Y, bossSpeedMul));
	} else bossShoot(i);
}


// spawn

static void spawnBoss(){
	struct enemySpawner spawner = {
		.angle = 256,
		.speed = FIX16(1),
		.y = 0,
		.boss = TRUE,
		.health = 16,
		.image = &cirno,
		.offX = 16,
		.offY = 16,
		.x = GAME_W / 2
		// .seal = TRUE
	};
	bossMax = spawner.health;
	void updater(s16 i){
		enemies[i].bools[1] ? bossMove(i) : bossFlyIn(i);
	}
	void suicide(s16 i){
		killBullets = TRUE;
		currentBoss++;
		bossHealth = 0;
	}
	spawnEnemy(spawner, updater, suicide, EMPTY);
}


// loop

void loadBoss(){
	spawnBoss();
}

void updateBoss(){}


// enemy types

// static void smallOne(s16 y, bool alt){
// 	struct enemySpawner spawner = {
// 		.angle = 512,
// 		.speed = FIX16(1),
// 		.y = y,
// 		.health = 2,
// 		.image = &fairy,
// 		.offX = 12,
// 		.offY = 12,
// 		.frame = random() % 3,
// 		.anim = 2
// 	};
// 	spawner.x = GAME_W + spawner.offX;
// 	spawner.fixes[0] = FIX16(spawner.y);
// 	spawner.fixes[1] = FIX16(20);
// 	spawner.fixes[2] = 0;
// 	spawner.bools[5] = alt;
// 	void updater(s16 i){
// 		enemies[i].pos.y = fix16Sub(enemies[i].fixes[0],
// 			fix16Mul(sinFix16(enemies[i].fixes[2]), enemies[i].fixes[1]));
// 		enemies[i].fixes[2] = fix16Add(enemies[i].fixes[2], FIX16(0.1));
// 		if(enemies[i].clock == 45){
// 			struct bulletSpawner spawner = {
// 				.angle = 512,
// 				.speed = FIX16(enemies[i].bools[5] ? 1.5 : 1.25),
// 				.x = enemies[i].pos.x,
// 				.y = enemies[i].pos.y,
// 				.image = &smallBlueBullet
// 			};
// 			velPos.x = spawner.x;
// 			velPos.y = spawner.y;
// 			spawner.vel = hone(velPos, players[0].pos, spawner.speed, 0);
// 			spawnBullet(spawner, EMPTY);
// 		}
// 	}
// 	spawnEnemy(spawner, updater, EMPTY, EMPTY);
// }

// static void smallTwo(bool bottom){
// 	struct enemySpawner spawner = {
// 		.angle = bottom ? 768 : 256,
// 		.speed = FIX16(1),
// 		.health = 2,
// 		.image = &spirit,
// 		.offX = 12,
// 		.offY = 12,
// 		.x = GAME_W / 5 * 4,
// 		.frame = random() % 4,
// 		.anim = 3,
// 	};
// 	spawner.y = bottom ? (GAME_H + spawner.offY) : -spawner.offY;
// 	spawner.bools[0] = bottom;
// 	void updater(s16 i){
// 		if(enemies[i].clock % 5 == 0 && enemies[i].clock >= 75 && enemies[i].angle != 512){
// 			enemies[i].angle += enemies[i].bools[0] ? -32 : 32;
// 			updateEnemyVel(i);
// 		}
// 		if(enemies[i].health <= 1) SPR_setAnim(enemies[i].image, 1);
// 	}
// 	spawnEnemy(spawner, updater, EMPTY, EMPTY);
// }

// static void smallThree(s16 y){
// 	struct enemySpawner spawner = {
// 		.angle = 512,
// 		.speed = FIX16(2.25),
// 		.y = y,
// 		.health = 2,
// 		.image = &fairy,
// 		.offX = 12,
// 		.offY = 12,
// 		.frame = random() % 3,
// 		.anim = 3
// 	};
// 	spawner.x = GAME_W + spawner.offX;
// 	void updater(s16 i){
// 		if(enemies[i].speed > FIX16(0.5) && enemies[i].clock % 5 == 0 && !enemies[i].bools[0] && !enemies[i].bools[1]){
// 			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.25));
// 			enemies[i].vel = hone(enemies[i].pos, players[0].pos, enemies[i].speed, 0);
// 		} else if(enemies[i].speed <= FIX16(0.5) && !enemies[i].bools[1]){
// 			if(!enemies[i].bools[0]){
// 				enemies[i].bools[0] = TRUE;
// 				enemies[i].clock = 0;
// 				enemies[i].fixes[0] = players[0].pos.x;
// 				enemies[i].fixes[1] = players[0].pos.y;
// 				enemies[i].fixes[2] = enemies[i].pos.x;
// 				enemies[i].fixes[3] = enemies[i].pos.y;
// 			}
// 			if(enemies[i].clock % 8 == 0 && enemies[i].clock <= 24){
// 				struct bulletSpawner spawner = {
// 					.angle = 512,
// 					.x = enemies[i].fixes[2],
// 					.y = enemies[i].fixes[3],
// 					.speed = FIX16(2),
// 					.image = &smallBlueBullet
// 				};
// 				velPos.x = spawner.x;
// 				velPos.y = spawner.y;
// 				velPos2.x = enemies[i].fixes[0];
// 				velPos2.y = enemies[i].fixes[1];
// 				spawner.vel = hone(velPos, velPos2, spawner.speed, 0);
// 				spawnBullet(spawner, EMPTY);
// 			} else if(enemies[i].clock >= 180){
// 				enemies[i].bools[1] = TRUE;
// 				enemies[i].clock = 0;
// 			}
// 		}
// 		if(enemies[i].bools[1] && enemies[i].clock % 30 == 0 && enemies[i].speed < FIX16(2.5)){
// 			enemies[i].angle = 512;
// 			enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.25));
// 			updateEnemyVel(i);
// 		}
// 	}
// 	spawnEnemy(spawner, updater, EMPTY, EMPTY);
// }

// static void mediumOne(){
// 	struct enemySpawner spawner = {
// 		.angle = 512,
// 		.speed = FIX16(1.2),
// 		.health = 20,
// 		.image = &fairy,
// 		.offX = 12,
// 		.offY = 12,
// 		.x = GAME_W + 12,
// 		.y = GAME_H / 2,
// 		.seal = TRUE
// 	};
// 	void updater(s16 i){
// 		if(enemies[i].bools[0]){
// 			if(enemies[i].clock >= 360 && enemies[i].clock % 10 == 0){
// 				enemies[i].angle = 0;
// 				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
// 				updateEnemyVel(i);
// 			} else if(enemies[i].clock % 90 == 0 || enemies[i].clock % 90 == 15 && enemies[i].clock < 360){
// 				if(enemies[i].clock % 90 == 0){
// 					enemies[i].ints[4] = 320 - 32 + random() % 64;
// 					enemies[i].fixes[4] = FIX16(1.75);
// 				}
// 				struct bulletSpawner spawner = {
// 					.angle = enemies[i].ints[4],
// 					.speed =enemies[i].fixes[4],
// 					.x = enemies[i].pos.x,
// 					.y = enemies[i].pos.y,
// 					.image = &bigRedBullet,
// 					.big = TRUE
// 				};
// 				if(enemies[i].clock % 90 == 0){
// 					spawner.image = &smallRedBullet;
// 					spawner.big = FALSE;
// 				}
// 				enemies[i].fixes[4] = fix16Sub(enemies[i].fixes[4], FIX16(0.5));
// 				for(u8 i = 0; i < 5; i++){
// 					spawner.angle += 64;
// 					spawnBullet(spawner, EMPTY);
// 				}
// 			}
// 		} else if(enemies[i].clock >= 20 && enemies[i].clock % 10 == 0 && enemies[i].speed > 0){
// 			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.2));
// 			if(enemies[i].speed <= 0){
// 				enemies[i].bools[0] = TRUE;
// 				enemies[i].speed = 0;
// 				enemies[i].clock = -1;
// 			}
// 			updateEnemyVel(i);
// 		}
// 	}
// 	void suicide(s16 i){
// 		waveClock = 15;
// 		killBullets = TRUE;
// 	}
// 	spawnEnemy(spawner, updater, suicide, EMPTY);
// }

// static void mediumTwo(s16 y){
// 	struct enemySpawner spawner = {
// 		.angle = 512,
// 		.speed = FIX16(1),
// 		.health = 6,
// 		.image = &fairy,
// 		.offX = 12,
// 		.offY = 12,
// 		.x = GAME_W + 12,
// 		.y = y,
// 		.anim = 1,
// 		.seal = TRUE
// 	};
// 	void updater(s16 i){
// 		if(enemies[i].bools[0]){
// 			if(enemies[i].clock >= 210 && enemies[i].clock % 10 == 0){
// 				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
// 				updateEnemyVel(i);
// 			} else if(enemies[i].clock <= 60 && enemies[i].clock % 15 == 0){
// 				struct bulletSpawner spawner = {
// 					.speed = FIX16(1.75),
// 					.x = enemies[i].pos.x,
// 					.y = fix16Sub(enemies[i].pos.y, FIX16(enemies[i].clock % 30 ? -32 : 32)),
// 					.image = &smallGreenBullet
// 				};
// 				velPos.x = spawner.x;
// 				velPos.y = spawner.y;
// 				spawner.vel = hone(velPos, players[0].pos, spawner.speed, 0);
// 				spawnBullet(spawner, EMPTY);
// 				spawnExplosion(spawner.x, spawner.y);
// 			}
// 		} else if(enemies[i].clock >= 20 && enemies[i].clock % 20 == 0 && enemies[i].speed > FIX16(0.5)){
// 			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.25));
// 			if(enemies[i].speed <= FIX16(0.5)){
// 				enemies[i].bools[0] = TRUE;
// 				enemies[i].speed = FIX16(0.5);
// 				enemies[i].clock = -1;
// 			}
// 			updateEnemyVel(i);
// 		}
// 	}
// 	spawnEnemy(spawner, updater, EMPTY, EMPTY);
// }

// static void largeOne(bool bottom){
// 	struct enemySpawner spawner = {
// 		.angle = bottom ? 576 : 448,
// 		.speed = FIX16(1),
// 		.health = 32,
// 		.image = &fairy,
// 		.offX = 12,
// 		.offY = 12,
// 		.x = GAME_W + 12,
// 		.y = bottom ? GAME_H / 4 * 3 : GAME_H / 4,
// 		.anim = 1,
// 		.seal = TRUE
// 	};
// 	if(bottom) spawner.bools[3] = TRUE;
// 	void updater(s16 i){
// 		if(enemies[i].bools[0] && enemies[i].clock >= 0){
// 			if(enemies[i].clock >= 600 && enemies[i].clock % 10 == 0){
// 				enemies[i].angle = 0;
// 				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
// 				enemies[i].ints[3] = 0;
// 				updateEnemyVel(i);
// 			} else if(enemies[i].clock % 120 == 0){
// 				struct bulletSpawner spawner = {
// 					.angle = (enemies[i].bools[3] ? 608 : 544) + 16 - random() % 32,
// 					.speed = FIX16(1.5),
// 					.x = enemies[i].pos.x,
// 					.y = enemies[i].pos.y,
// 					.image = &bigRedBullet,
// 					.big = TRUE
// 				};
// 				for(u8 i = 0; i < 4; i++){
// 					spawnBullet(spawner, EMPTY);
// 					spawner.angle -= 56;
// 				}
// 			}
// 			else if(enemies[i].clock % 120 >= 60 && enemies[i].clock % 120 < 110 && enemies[i].clock % 20 == 0) {
// 				struct bulletSpawner spawner = {
// 					.angle = (enemies[i].bools[3] ? 608 : 544) + 8 - random() % 16,
// 					.speed = FIX16(enemies[i].clock % 120 == 80 ? 2 : 1.5),
// 					.x = enemies[i].pos.x,
// 					.y = enemies[i].pos.y + FIX16(enemies[i].clock % 40 < 20 ? -32 : 32),
// 					.image = &smallBlueBullet
// 				};
// 				for(u8 i = 0; i < 3; i++){
// 					spawnBullet(spawner, EMPTY);
// 					spawner.angle -= 64;
// 				}
// 				spawnExplosion(spawner.x, spawner.y);
// 			}
// 		} else if(enemies[i].clock >= 20 && enemies[i].clock % 10 == 0 && enemies[i].speed > 0 && !enemies[i].bools[0]){
// 			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.15));
// 			if(enemies[i].speed <= 0){
// 				enemies[i].bools[0] = TRUE;
// 				enemies[i].speed = 0;
// 				enemies[i].clock = -30;
// 			}
// 			updateEnemyVel(i);
// 		}
// 	}
// 	void suicide(s16 i){
// 		killBullets = TRUE;
// 		waveClock = 15;
// 	}
// 	spawnEnemy(spawner, updater, suicide, EMPTY);
// }

// static void largeTwo(){
// 	struct enemySpawner spawner = {
// 		.angle = 512,
// 		.speed = FIX16(1.2),
// 		.health = 26,
// 		.image = &fairy,
// 		.offX = 12,
// 		.offY = 12,
// 		.x = GAME_W + 12,
// 		.y = GAME_H / 2,
// 		.seal = TRUE
// 	};
// 	void updater(s16 i){
// 		if(enemies[i].bools[0]){
// 			if(enemies[i].clock >= 600 && enemies[i].clock % 10 == 0){
// 				enemies[i].angle = 0;
// 				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
// 				enemies[i].ints[3] = 0;
// 				updateEnemyVel(i);
// 			} else if(enemies[i].clock % 25 == 0 && enemies[i].clock < 600){
// 				struct bulletSpawner spawner = {
// 					.angle = enemies[i].ints[3],
// 					.speed = FIX16(2),
// 					.x = enemies[i].pos.x,
// 					.y = enemies[i].pos.y,
// 					.image = &bigRedBullet,
// 					.big = TRUE
// 				};
// 				enemies[i].ints[3] += 27;
// 				if(enemies[i].ints[3] > 1024) enemies[i].ints[3] -= 1024;
// 				if(enemies[i].clock % 50 == 0){
// 					spawner.image = &smallRedBullet;
// 					spawner.big = FALSE;
// 				}
// 				for(u8 i = 0; i < 16; i++){
// 					if(spawner.angle % 1024 > 320 && spawner.angle % 1024 < 704) spawnBullet(spawner, EMPTY);
// 					spawner.angle += 64;
// 				}
// 			}
// 		} else if(enemies[i].clock >= 20 && enemies[i].clock % 10 == 0 && enemies[i].speed > 0){
// 			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.2));
// 			if(enemies[i].speed <= 0){
// 				enemies[i].bools[0] = TRUE;
// 				enemies[i].speed = 0;
// 				enemies[i].clock = -1;
// 			}
// 			updateEnemyVel(i);
// 		}
// 	}
// 	void suicide(s16 i){
// 		killBullets = TRUE;
// 		waveClock = 10;
// 	}
// 	spawnEnemy(spawner, updater, suicide, EMPTY);
// }


// // midboss

// static void wriggleOne(s16 i){
// 	if(enemies[i].clock % 480 < 240){
// 		if(enemies[i].clock % 120 < 80 && enemies[i].clock % 20 == 0){
// 			if(enemies[i].speed != 0){
// 				enemies[i].speed = 0;
// 				updateEnemyVel(i);
// 			}
// 			if(enemies[i].clock % 120 == 0) enemies[i].ints[3] = 0;
// 			struct bulletSpawner spawner = {
// 				.angle = 64,
// 				.speed = FIX16(1.6),
// 				.x = enemies[i].pos.x,
// 				.y = enemies[i].pos.y,
// 				.image = &smallYellowBullet
// 			};
// 			if(enemies[i].clock % 240 >= 120){
// 				spawner.image = &smallGreenBullet;
// 				spawner.angle += 512 - enemies[i].ints[3];
// 				spawner.bools[4] = TRUE;
// 			} else spawner.angle += enemies[i].ints[3];
// 			enemies[i].ints[3] += 16;
// 			void updater(s16 j){
// 				if(bullets[j].bools[0]){
// 					if(bullets[j].clock == 30) bullets[j].speed = FIX16(1.75);
// 					if(bullets[j].clock >= 30 && bullets[j].clock < 50 && bullets[j].clock % 2 == 0){
// 						bullets[j].angle += bullets[j].bools[4] ? -32 : 32;
// 						updateBulletVel(j);
// 					} else if(bullets[j].clock >= 100 && bullets[j].clock < 130 && bullets[j].clock % 5 == 0){
// 						bullets[j].angle += bullets[j].bools[4] ? 32 : -32;
// 						updateBulletVel(j);
// 					}
// 				}	else if(!bullets[j].bools[0]) {
// 					if(bullets[j].clock > 0 && bullets[j].clock % 5 == 0){
// 						bullets[j].speed = fix16Sub(bullets[j].speed, FIX16(0.2));
// 						if(bullets[j].speed <= 0){
// 							bullets[j].bools[0] = TRUE;
// 							bullets[j].speed = 0;
// 							bullets[j].clock = -1;
// 						}
// 						updateBulletVel(j);
// 					}
// 				}
// 			}
// 			for(u8 j = 0; j < 5; j++){
// 				spawner.angle += 64;
// 				spawnBullet(spawner, updater);
// 			}
// 		}
// 		else if(enemies[i].clock % 240 == 200){
// 			enemies[i].angle = 288;
// 			enemies[i].speed = FIX16(1);
// 			updateEnemyVel(i);
// 		}
// 	} else {
// 		if(enemies[i].clock % 240 < 180){
// 			if(enemies[i].speed != 0){
// 				enemies[i].speed = 0;
// 				updateEnemyVel(i);
// 			}
// 			if(enemies[i].clock % 6 == 0 && enemies[i].clock % 240 >= 60){
// 				struct bulletSpawner spawner = {
// 					.speed = FIX16(enemies[i].clock % 10 == 0 ? 1.25 : 1.75),
// 					.x = enemies[i].pos.x,
// 					.y = enemies[i].pos.y,
// 					.image = enemies[i].clock % 10 == 0 ? &smallGreenBullet : &bigGreenBullet
// 				};
// 				velPos.x = spawner.x;
// 				velPos.y = spawner.y;
// 				spawner.vel = hone(velPos, players[0].pos, spawner.speed, 80);
// 				spawnBullet(spawner, EMPTY);
// 			}
// 		} else if(enemies[i].clock % 240 == 200){
// 			enemies[i].angle = 800;
// 			enemies[i].speed = FIX16(1);
// 			updateEnemyVel(i);
// 		}
// 	}
// }

// static void wriggleTwo(s16 i){
// 	if(enemies[i].speed > 0){
// 		enemies[i].speed = 0;
// 		updateEnemyVel(i);
// 		enemies[i].clock = -1;
// 	}
// 	// if(enemies[i].pos.y > FIX16(GAME_H / 2 + 1)){
// 	// 	enemies[i].pos.y = fix16Sub(enemies[i].pos.y, FIX16(1));
// 	// 	enemies[i].clock = -1;
// 	// } else if(enemies[i].pos.y < FIX16(GAME_H / 2 - 1)){
// 	// 	enemies[i].pos.y = fix16Add(enemies[i].pos.y, FIX16(1));
// 	// 	enemies[i].clock = -1;
// 	// } else {
// 		if(enemies[i].clock % 240 < 80 && enemies[i].clock % 15 == 0){
// 			if(enemies[i].clock % 240 == 0) enemies[i].ints[2] = 48 + random() % 32;
// 			struct bulletSpawner spawner = {
// 				.angle = enemies[i].ints[2],
// 				.image = &smallGreenBullet
// 			};
// 			void updater(j){
// 				if(bullets[j].clock >= 60){
// 					if(bullets[j].clock == 60){
// 						bullets[j].angle = random() % 512;
// 					}
// 					if(bullets[j].clock % 15 == 0 && bullets[j].speed < FIX16(1.6)){
// 						bullets[j].speed = fix16Add(bullets[j].speed, FIX16(0.2));
// 						updateBulletVel(j);
// 					}
// 				}
// 			}
// 			spawner.angle += 48;
// 			for(u8 j = 0; j < 4; j++){
// 				spawner.x = fix16Add(enemies[i].pos.x, fix16Mul(cosFix16(spawner.angle), FIX16(28 * (enemies[i].clock % 120 / 15 + 1))));
// 				spawner.y = fix16Add(enemies[i].pos.y, fix16Mul(sinFix16(spawner.angle), FIX16(28 * (enemies[i].clock % 120 / 15 + 1))));
// 				spawnBullet(spawner, updater);
// 				spawner.angle += 96;
// 			}
// 		}
// 		else if(enemies[i].clock % 120 >= 80 && enemies[i].clock % 120 <= 100 && enemies[i].clock % 10 == 0){
// 			struct bulletSpawner spawner = {
// 				.angle = 64,
// 				.speed = FIX16(2 - (enemies[i].clock % 120 - 80) / 10 * 0.33),
// 				.x = enemies[i].pos.x,
// 				.y = enemies[i].pos.y,
// 				.image = &bigBlueBullet,
// 				.big = TRUE
// 			};
// 			if(enemies[i].clock % 120 != 90){
// 				spawner.big = FALSE;
// 				spawner.image = &smallBlueBullet;
// 			}
// 			for(u8 j = 0; j < 5; j++){
// 				spawner.angle += 64;
// 				spawnBullet(spawner, EMPTY);
// 			}
// 		}
// 	// }
// }

// static void spawnWriggle(){
// 	void updater(s16 i){
// 		if(enemies[i].health > 30) wriggleTwo(i);
// 		else {
// 			if(!enemies[i].bools[7]){
// 				enemies[i].bools[7] = TRUE;
// 				killBullets = TRUE;
// 			}
// 			wriggleTwo(i);
// 		}
// 	}
// 	spawnBoss(60, updater);
// }


// // boss

// static void spawnCirno(){
// 	void updater(s16 i){

// 		if(enemies[i].health >= 100){
// 			if(enemies[i].clock % 15 == 0){
// 				if(enemies[i].clock % 240 == 0){
// 					enemies[i].ints[4] = 512 - 128;
// 					enemies[i].ints[5] = 80;
// 				}
// 				struct bulletSpawner spawner = {
// 					.angle = enemies[i].ints[5],
// 					.speed = FIX16(2 - enemies[i].clock % 60 / 15 * .25),
// 					.x = enemies[i].pos.x,
// 					.y = enemies[i].pos.y,
// 					.image = &smallBlueBullet
// 				};
// 				spawner.bools[1] = enemies[i].clock % 480 < 240;
// 				spawner.bools[2] = enemies[i].clock % 30 == 0;
// 				spawner.ints[4] = enemies[i].ints[4];
// 				enemies[i].ints[5] += 32;
// 				enemies[i].ints[4] += 32;
// 				void updater(s16 j){
// 					if(bullets[j].bools[0]){ if(bullets[j].clock == 5){
// 						// bullets[j].vel = hone(bullets[j].pos, players[0].pos, FIX16(bullets[j].bools[2] ? 2.5 : 2), 80);
// 						bullets[j].speed = FIX16(bullets[j].bools[2] ? 1.5 : 2.5);
// 						bullets[j].angle = bullets[j].ints[4];
// 						updateBulletVel(j);
// 					} } else {
// 						if(bullets[j].clock % 15 == 0 && bullets[j].clock > 0){
// 							bullets[j].speed = fix16Sub(bullets[j].speed, FIX16(0.35));
// 							updateBulletVel(j);
// 							if(bullets[j].speed <= 0){
// 								bullets[j].speed = 0;
// 								updateBulletVel(j);
// 								bullets[j].bools[0] = TRUE;
// 								bullets[j].clock = 1;
// 							}
// 						}
// 					}
// 				}
// 				for(u8 j = 0; j < 4; j++){
// 					spawner.angle += 128;
// 					spawnBullet(spawner, updater);
// 				}
// 			}
// 		}

// 		else if(enemies[i].health >= 50){
// 			if(!enemies[i].bools[3]){
// 				killBullets = TRUE;
// 				enemies[i].bools[3] = TRUE;
// 				enemies[i].clock = -60;
// 			}
// 			if(enemies[i].clock % 480 < 240){
// 				if(enemies[i].clock % 5 == 0){
// 					struct bulletSpawner spawner = {
// 						.angle = 352 + random() % 320,
// 						.speed = FIX16(enemies[i].clock % 10 == 0 ? 1 : 1.5),
// 						.x = enemies[i].pos.x,
// 						.y = enemies[i].pos.y,
// 						.big = TRUE
// 					};
// 					spawner.ints[4] = i;
// 					spawner.ints[3] = enemies[i].clock % 20 / 5;
// 					spawner.ints[2] = enemies[i].clock % 10 == 0;
// 					switch(spawner.ints[3]){
// 						case 3: spawner.image = &bigBlueBullet; break;
// 						case 0: spawner.image = &bigRedBullet; break;
// 						case 1: spawner.image = &bigGreenBullet; break;
// 						case 2: spawner.image = &bigYellowBullet; break;
// 					}
// 					void updater(s16 j){
// 						if(enemies[bullets[j].ints[4]].clock % 480 == 120){
// 							bullets[j].speed = 0;
// 							updateBulletVel(j);
// 							SPR_setDefinition(bullets[j].image, &bigWhiteBullet);
// 						} else if(enemies[bullets[j].ints[4]].clock % 480 == 210){
// 							switch(bullets[j].ints[3]){
// 								case 3: bullets[j].angle = 416; break;
// 								case 0: bullets[j].angle = 480; break;
// 								case 1: bullets[j].angle = 544; break;
// 								case 2: bullets[j].angle = 608; break;
// 							}
// 							bullets[j].speed = FIX16(bullets[j].ints[2] ? 1 : 2);
// 							updateBulletVel(j);
// 						}
// 					}
// 					spawnBullet(spawner, updater);
// 				}
// 			} else {
// 				if(enemies[i].clock % 6 == 0){
// 					struct bulletSpawner spawner = {
// 						.speed = FIX16(enemies[i].clock % 12 == 0 ? 1.75 : 2.25),
// 						.x = enemies[i].pos.x,
// 						.y = FIX16(32 + random() % 160),
// 						.big = TRUE,
// 						.image = &bigBlueBullet
// 					};
// 					velPos.x = spawner.x;
// 					velPos.y = spawner.y;
// 					spawner.vel = hone(velPos, players[0].pos, spawner.speed, 32);
// 					spawnExplosion(spawner.x, spawner.y);
// 					spawnBullet(spawner, EMPTY);
// 				}
// 			}
// 		}

// 		else {
// 			if(!enemies[i].bools[4]){
// 				killBullets = TRUE;
// 				enemies[i].bools[4] = TRUE;
// 				enemies[i].clock = -1;
// 			}
// 			if(enemies[i].clock % 6 == 0){
// 				struct bulletSpawner spawner = {
// 					.x = FIX16(GAME_W - 96 + random() % 64),
// 					.y = FIX16(32 + random() % 160),
// 					.angle = 256 - 32 + random() % 64
// 				};
// 				spawner.bools[1] = enemies[i].clock % 12 == 0;
// 				if(spawner.bools[1]){
// 					spawner.image = &smallBlueBullet;
// 					spawner.speed = FIX16(2.5);
// 				} else {
// 					spawner.big = TRUE;
// 					spawner.image = &bigBlueBullet;
// 					spawner.speed = FIX16(2);
// 				}
// 				spawnExplosion(spawner.x, spawner.y);
// 				for(u8 j = 0; j < 3; j++){
// 					spawner.angle += 128;
// 					spawnBullet(spawner, EMPTY);
// 				}
// 			}
// 		}

// 	}
// 	spawnBoss(150, updater);
// }


// // loop

// void loadStageOne(){
// 	// waveClock = 60;
// 	currentWave = 79;
// }

// void updateStageOne(){
// 	if(waveClock == 0){
// 		switch(currentWave){

// 			case 0: smallOne(GAME_H / 4, FALSE); waveClock = 30; break;
// 			case 1: smallOne(GAME_H / 4, TRUE); waveClock = 30; break;
// 			case 2: smallOne(GAME_H / 4, FALSE); waveClock = 30; break;
// 			case 3: smallOne(GAME_H / 4, TRUE); waveClock = 45; break;

// 			case 4: smallOne(GAME_H / 4 * 3, FALSE); waveClock = 30; break;
// 			case 5: smallOne(GAME_H / 4 * 3, TRUE); waveClock = 30; break;
// 			case 6: smallOne(GAME_H / 4 * 3, FALSE); waveClock = 30; break;
// 			case 7: smallOne(GAME_H / 4 * 3, TRUE); waveClock = 45; break;

// 			case 8: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 9: smallOne(GAME_H / 2, TRUE); waveClock = 30; break;
// 			case 10: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 11: smallOne(GAME_H / 2, TRUE); waveClock = 15; break;

// 			case 12: smallTwo(FALSE); waveClock = 30; break;
// 			case 13: smallTwo(FALSE); waveClock = 30; break;
// 			case 14: smallTwo(FALSE); waveClock = 30; break;
// 			case 15: smallTwo(FALSE); waveClock = 1; break;

// 			case 16: smallTwo(TRUE); waveClock = 30; break;
// 			case 17: smallTwo(TRUE); waveClock = 30; break;
// 			case 18: smallTwo(TRUE); waveClock = 30; break;
// 			case 19: smallTwo(TRUE); waveClock = 45; break;

// 			case 20: smallOne(GAME_H / 3, FALSE); waveClock = 30; break;
// 			case 21: smallOne(GAME_H / 3, TRUE); waveClock = 30; break;
// 			case 22: smallOne(GAME_H / 3, FALSE); waveClock = 30; break;
// 			case 23: smallOne(GAME_H / 3, TRUE); waveClock = 45; break;

// 			case 24: smallOne(GAME_H / 3 * 2, FALSE); waveClock = 30; break;
// 			case 25: smallOne(GAME_H / 3 * 2, TRUE); waveClock = 30; break;
// 			case 26: smallOne(GAME_H / 3 * 2, FALSE); waveClock = 30; break;
// 			case 27: smallOne(GAME_H / 3 * 2, TRUE); waveClock = 75; break;

// 			case 28: mediumOne(); break;

// 			case 29: smallTwo(TRUE); waveClock = 30; break;
// 			case 30: smallTwo(TRUE); waveClock = 30; break;
// 			case 31: smallTwo(TRUE); waveClock = 30; break;
// 			case 32: smallTwo(TRUE); waveClock = 1; break;

// 			case 33: smallTwo(FALSE); waveClock = 30; break;
// 			case 34: smallTwo(FALSE); waveClock = 30; break;
// 			case 35: smallTwo(FALSE); waveClock = 30; break;
// 			case 36: smallTwo(FALSE); waveClock = 45; break;

// 			case 37: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 38: smallOne(GAME_H / 2, TRUE); waveClock = 30; break;
// 			case 39: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 40: smallOne(GAME_H / 2, TRUE); waveClock = 75; break;

// 			case 41: mediumOne(); break;

// 			case 42: largeOne(TRUE); break;

// 			case 43: smallTwo(FALSE); waveClock = 30; break;
// 			case 44: smallTwo(FALSE); waveClock = 1; break;

// 			case 45: smallTwo(TRUE); waveClock = 30; break;
// 			case 46: smallTwo(TRUE); waveClock = 45; break;

// 			case 47: smallOne(GAME_H / 4, FALSE); waveClock = 30; break;
// 			case 48: smallOne(GAME_H / 4, TRUE); waveClock = 30; break;
// 			case 49: smallOne(GAME_H / 4, FALSE); waveClock = 30; break;
// 			case 50: smallOne(GAME_H / 4, TRUE); waveClock = 45; break;

// 			case 51: smallOne(GAME_H / 4 * 3, FALSE); waveClock = 30; break;
// 			case 52: smallOne(GAME_H / 4 * 3, TRUE); waveClock = 30; break;
// 			case 53: smallOne(GAME_H / 4 * 3, FALSE); waveClock = 30; break;
// 			case 54: smallOne(GAME_H / 4 * 3, TRUE); waveClock = 90; break;

// 			case 55: mediumTwo(GAME_H / 2); waveClock = 90; break;
// 			case 56: mediumTwo(GAME_H / 4); waveClock = 90; break;
// 			case 57: mediumTwo(GAME_H / 4 * 3); waveClock = 90; break;

// 			case 58: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 59: smallOne(GAME_H / 2, TRUE); waveClock = 30; break;
// 			case 60: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 61: smallOne(GAME_H / 2, TRUE); waveClock = 45; break;

// 			case 62: smallOne(GAME_H / 4, FALSE); waveClock = 30; break;
// 			case 63: smallOne(GAME_H / 4, TRUE); waveClock = 30; break;
// 			case 64: smallOne(GAME_H / 4, FALSE); waveClock = 30; break;
// 			case 65: smallOne(GAME_H / 4, TRUE); waveClock = 45; break;

// 			case 66: smallOne(GAME_H / 4 * 3, FALSE); waveClock = 30; break;
// 			case 67: smallOne(GAME_H / 4 * 3, TRUE); waveClock = 30; break;
// 			case 68: smallOne(GAME_H / 4 * 3, FALSE); waveClock = 30; break;
// 			case 69: smallOne(GAME_H / 4 * 3, TRUE); waveClock = 90; break;

// 			case 70: mediumTwo(GAME_H / 2); waveClock = 90; break;
// 			case 71: mediumTwo(GAME_H / 4); waveClock = 90; break;
// 			case 72: mediumTwo(GAME_H / 4 * 3); waveClock = 90; break;
// 			case 73: mediumTwo(GAME_H / 2); waveClock = 90; break;

// 			case 74: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 75: smallOne(GAME_H / 2, TRUE); waveClock = 30; break;
// 			case 76: smallOne(GAME_H / 2, FALSE); waveClock = 30; break;
// 			case 77: smallOne(GAME_H / 2, TRUE); waveClock = 240; break;

// 			case 78: killBullets = TRUE; loadCutscene(0); break;

// 			case 79: spawnWriggle(); break;

// 			// case  61: loadCutscene(1); break;
		
// 			// // 1:16
// 			// case  62: smallTwo(TRUE); waveClock = 30; break;
// 			// case  63: smallTwo(TRUE); waveClock = 10; break;
// 			// case  64: smallTwo(FALSE); waveClock = 30; break;
// 			// case  65: smallTwo(FALSE); waveClock = 30; break;

// 			// case  66: smallThree(GAME_H / 3 * 2); waveClock = 30; break;
// 			// case  67: smallThree(GAME_H / 3 * 2 - 32); waveClock = 60; break;
// 			// case  68: smallThree(GAME_H / 3); waveClock = 30; break;
// 			// case  69: smallThree(GAME_H / 3 + 32); waveClock = 60; break;
// 			// case  70: smallThree(GAME_H / 2); waveClock = 30; break;
// 			// case  71: smallThree(GAME_H / 2 - 64); waveClock = 30; break;
// 			// case  72: smallThree(GAME_H / 2 + 64); waveClock = 30; break;
// 			// case  73: smallThree(GAME_H / 2); waveClock = 60; break;

// 			// case  74: largeOne(TRUE); break;

// 			// case  75: smallTwo(FALSE); waveClock = 30; break;
// 			// case  76: smallTwo(FALSE); waveClock = 10; break;
// 			// case  77: smallTwo(TRUE); waveClock = 30; break;
// 			// case  78: smallTwo(TRUE); waveClock = 30; break;

// 			// case  79: smallOne(GAME_H / 2); waveClock = 30; break;
// 			// case  80: smallOne(GAME_H / 2 - 64); waveClock = 30; break;
// 			// case  81: smallOne(GAME_H / 2 + 32); waveClock = 30; break;
// 			// case  82: smallOne(GAME_H / 2); waveClock = 30; break;
// 			// case  83: smallOne(GAME_H / 2 + 64); waveClock = 30; break;
// 			// case  84: smallOne(GAME_H / 2 - 32); waveClock = 60; break;

// 			// case  85: smallThree(GAME_H / 2); waveClock = 60; break;
// 			// case  86: smallThree(GAME_H / 3); waveClock = 30; break;
// 			// case  87: smallThree(GAME_H / 3 + 32); waveClock = 30; break;
// 			// case  88: smallThree(GAME_H / 3 * 2); waveClock = 30; break;
// 			// case  89: smallThree(GAME_H / 3 * 2 - 32); waveClock = 30; break;
// 			// case  90: smallThree(GAME_H / 2); waveClock = 60; break;

// 			// case  91: largeTwo(); break;

// 			// case  92: cirno(); break;

// 		}
// 		currentWave++;
// 	}
// }