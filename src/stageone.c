// stage one
// https://www.youtube.com/watch?v=dEzH4yxjFx0

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "bullets.h"
#include "stage.h"
#include "players.h"
#include "enemies.h"
#include "cutscenes.h"
#include "stageone.h"


// enemy types

static void smallOne(s16 y){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(2.25),
		.y = y,
		.health = 2,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.frame = random() % 3,
		.anim = 2
	};
	spawner.x = GAME_W + spawner.offX;
	void updater(s16 i){
		if(enemies[i].speed > FIX16(0.5) && enemies[i].clock > 0 && enemies[i].clock % 5 == 0 && !enemies[i].bools[0]){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.25));
			enemies[i].vel = hone(enemies[i].pos, players[0].pos, enemies[i].speed, 0);
		} else if(enemies[i].speed <= FIX16(0.5) && !enemies[i].bools[0]){
			enemies[i].bools[0] = TRUE;
			struct bulletSpawner spawner = {
				.angle = 512,
				.speed = FIX16(1.5),
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &smallBlueBullet
			};
			velPos.x = spawner.x;
			velPos.y = spawner.y;
			spawner.vel = hone(velPos, players[0].pos, spawner.speed, 0);
			spawnBullet(spawner, EMPTY);
			enemies[i].clock = -1;
		} else if(enemies[i].bools[0] && enemies[i].clock >= 180 && enemies[i].clock % 30 == 0 && enemies[i].speed < FIX16(2.5)){
			enemies[i].angle = 512;
			enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.25));
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void smallTwo(bool bottom){
	struct enemySpawner spawner = {
		.angle = bottom ? 768 : 256,
		.speed = FIX16(1),
		.health = 2,
		.image = &spirit,
		.offX = 12,
		.offY = 12,
		.x = GAME_W / 5 * 4,
		.frame = random() % 4,
		.anim = 3,
	};
	spawner.y = bottom ? (GAME_H + spawner.offY) : -spawner.offY;
	spawner.bools[0] = bottom;
	void updater(s16 i){
		if(enemies[i].clock % 5 == 0 && enemies[i].clock >= 60 && enemies[i].angle != 512){
			enemies[i].angle += enemies[i].bools[0] ? -32 : 32;
			updateEnemyVel(i);
		}
		if(enemies[i].health <= 1) SPR_setAnim(enemies[i].image, 1);
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void smallThree(s16 y){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(2.25),
		.y = y,
		.health = 2,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.frame = random() % 3,
		.anim = 3
	};
	spawner.x = GAME_W + spawner.offX;
	void updater(s16 i){
		if(enemies[i].speed > FIX16(0.5) && enemies[i].clock % 5 == 0 && !enemies[i].bools[0] && !enemies[i].bools[1]){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.25));
			enemies[i].vel = hone(enemies[i].pos, players[0].pos, enemies[i].speed, 0);
		} else if(enemies[i].speed <= FIX16(0.5) && !enemies[i].bools[1]){
			if(!enemies[i].bools[0]){
				enemies[i].bools[0] = TRUE;
				enemies[i].clock = 0;
				enemies[i].fixes[0] = players[0].pos.x;
				enemies[i].fixes[1] = players[0].pos.y;
				enemies[i].fixes[2] = enemies[i].pos.x;
				enemies[i].fixes[3] = enemies[i].pos.y;
			}
			if(enemies[i].clock % 8 == 0 && enemies[i].clock <= 24){
				struct bulletSpawner spawner = {
					.angle = 512,
					.x = enemies[i].fixes[2],
					.y = enemies[i].fixes[3],
					.speed = FIX16(2),
					.image = &smallBlueBullet
				};
				velPos.x = spawner.x;
				velPos.y = spawner.y;
				velPos2.x = enemies[i].fixes[0];
				velPos2.y = enemies[i].fixes[1];
				spawner.vel = hone(velPos, velPos2, spawner.speed, 0);
				spawnBullet(spawner, EMPTY);
			} else if(enemies[i].clock >= 180){
				enemies[i].bools[1] = TRUE;
				enemies[i].clock = 0;
			}
		}
		if(enemies[i].bools[1] && enemies[i].clock % 30 == 0 && enemies[i].speed < FIX16(2.5)){
			enemies[i].angle = 512;
			enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.25));
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void mediumOne(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.2),
		.health = 15,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.x = GAME_W + 12,
		.y = GAME_H / 2,
		.seal = TRUE
	};
	void updater(s16 i){
		if(enemies[i].bools[0]){
			if(enemies[i].clock >= 360 && enemies[i].clock % 10 == 0){
				enemies[i].angle = 0;
				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
				updateEnemyVel(i);
			} else if(enemies[i].clock % 90 == 0 || enemies[i].clock % 90 == 15 && enemies[i].clock < 360){
				if(enemies[i].clock % 90 == 0){
					enemies[i].ints[4] = 320 - 32 + random() % 64;
					enemies[i].fixes[4] = FIX16(1.75);
				}
				struct bulletSpawner spawner = {
					.angle = enemies[i].ints[4],
					.speed =enemies[i].fixes[4],
					.x = enemies[i].pos.x,
					.y = enemies[i].pos.y,
					.image = &bigRedBullet,
					.big = TRUE
				};
				if(enemies[i].clock % 90 == 0){
					spawner.image = &smallRedBullet;
					spawner.big = FALSE;
				}
				enemies[i].fixes[4] = fix16Sub(enemies[i].fixes[4], FIX16(0.25));
				for(u8 i = 0; i < 5; i++){
					spawner.angle += 64;
					spawnBullet(spawner, EMPTY);
				}
			}
		} else if(enemies[i].clock >= 20 && enemies[i].clock % 10 == 0 && enemies[i].speed > 0){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.2));
			if(enemies[i].speed <= 0){
				enemies[i].bools[0] = TRUE;
				enemies[i].speed = 0;
				enemies[i].clock = -1;
			}
			updateEnemyVel(i);
		}
	}
	void suicide(s16 i){
		waveClock = 60;
	}
	spawnEnemy(spawner, updater, suicide);
}

static void mediumTwo(s16 y){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1),
		.health = 4,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.x = GAME_W + 12,
		.y = y,
		.anim = 1,
		.seal = TRUE
	};
	void updater(s16 i){
		if(enemies[i].bools[0]){
			if(enemies[i].clock >= 210 && enemies[i].clock % 10 == 0){
				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
				updateEnemyVel(i);
			} else if(enemies[i].clock < 60 && enemies[i].clock % 10 == 0){
				struct bulletSpawner spawner = {
					.speed = FIX16(2),
					.x = enemies[i].pos.x,
					.y = fix16Sub(enemies[i].pos.y, FIX16(enemies[i].clock % 20 ? -40 : 40)),
					.image = &smallGreenBullet
				};
				velPos.x = spawner.x;
				velPos.y = spawner.y;
				spawner.vel = hone(velPos, players[0].pos, spawner.speed, 0);
				spawnBullet(spawner, EMPTY);
				spawnExplosion(spawner.x, spawner.y);
			}
		} else if(enemies[i].clock >= 20 && enemies[i].clock % 20 == 0 && enemies[i].speed > FIX16(0.5)){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.25));
			if(enemies[i].speed <= FIX16(0.5)){
				enemies[i].bools[0] = TRUE;
				enemies[i].speed = FIX16(0.5);
				enemies[i].clock = -1;
			}
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void largeOne(bool bottom){
	struct enemySpawner spawner = {
		.angle = bottom ? 576 : 448,
		.speed = FIX16(1),
		.health = 26,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.x = GAME_W + 12,
		.y = bottom ? GAME_H / 4 * 3 : GAME_H / 4,
		.anim = 1,
		.seal = TRUE
	};
	if(bottom) spawner.bools[3] = TRUE;
	void updater(s16 i){
		if(enemies[i].bools[0] && enemies[i].clock >= 0){
			if(enemies[i].clock >= 600 && enemies[i].clock % 10 == 0){
				enemies[i].angle = 0;
				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
				enemies[i].ints[3] = 0;
				updateEnemyVel(i);
			} else if(enemies[i].clock % 120 <= 15 && enemies[i].clock % 15 == 0){
				struct bulletSpawner spawner = {
					.angle = (enemies[i].bools[3] ? 608 : 544) + 8 - random() % 16,
					.speed = FIX16(1.75),
					.x = enemies[i].pos.x,
					.y = enemies[i].pos.y,
					.image = &bigRedBullet,
					.big = TRUE
				};
				for(u8 i = 0; i < 3; i++){
					spawnBullet(spawner, EMPTY);
					spawner.angle -= 64;
				}
			}
			else if(enemies[i].clock % 120 >= 60 && enemies[i].clock % 120 < 110 && enemies[i].clock % 20 == 0) {
				struct bulletSpawner spawner = {
					.angle = (enemies[i].bools[3] ? 608 : 544) + 8 - random() % 16,
					.speed = FIX16(2),
					.x = enemies[i].pos.x,
					.y = enemies[i].pos.y + FIX16(enemies[i].clock % 40 < 20 ? -32 : 32),
					.image = &smallBlueBullet
				};
				for(u8 i = 0; i < 3; i++){
					spawnBullet(spawner, EMPTY);
					spawner.angle -= 64;
				}
				spawnExplosion(spawner.x, spawner.y);
			}
		} else if(enemies[i].clock >= 20 && enemies[i].clock % 10 == 0 && enemies[i].speed > 0 && !enemies[i].bools[0]){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.15));
			if(enemies[i].speed <= 0){
				enemies[i].bools[0] = TRUE;
				enemies[i].speed = 0;
				enemies[i].clock = -30;
			}
			updateEnemyVel(i);
		}
	}
	void suicide(s16 i){
		killBullets = TRUE;
		waveClock = 10;
	}
	spawnEnemy(spawner, updater, suicide);
}

static void largeTwo(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.2),
		.health = 26,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.x = GAME_W + 12,
		.y = GAME_H / 2,
		.seal = TRUE
	};
	void updater(s16 i){
		if(enemies[i].bools[0]){
			if(enemies[i].clock >= 600 && enemies[i].clock % 10 == 0){
				enemies[i].angle = 0;
				enemies[i].speed = fix16Add(enemies[i].speed, FIX16(0.1));
				enemies[i].ints[3] = 0;
				updateEnemyVel(i);
			} else if(enemies[i].clock % 25 == 0 && enemies[i].clock < 600){
				struct bulletSpawner spawner = {
					.angle = enemies[i].ints[3],
					.speed = FIX16(2),
					.x = enemies[i].pos.x,
					.y = enemies[i].pos.y,
					.image = &bigRedBullet,
					.big = TRUE
				};
				enemies[i].ints[3] += 27;
				if(enemies[i].ints[3] > 1024) enemies[i].ints[3] -= 1024;
				if(enemies[i].clock % 50 == 0){
					spawner.image = &smallRedBullet;
					spawner.big = FALSE;
				}
				for(u8 i = 0; i < 16; i++){
					if(spawner.angle % 1024 > 320 && spawner.angle % 1024 < 704) spawnBullet(spawner, EMPTY);
					spawner.angle += 64;
				}
			}
		} else if(enemies[i].clock >= 20 && enemies[i].clock % 10 == 0 && enemies[i].speed > 0){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.2));
			if(enemies[i].speed <= 0){
				enemies[i].bools[0] = TRUE;
				enemies[i].speed = 0;
				enemies[i].clock = -1;
			}
			updateEnemyVel(i);
		}
	}
	void suicide(s16 i){
		killBullets = TRUE;
		waveClock = 10;
	}
	spawnEnemy(spawner, updater, suicide);
}


// midboss

void wriggle(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.25),
		.y = GAME_H / 2,
		.health = 50,
		.image = &fairy,
		.offX = 12,
		.offY = 12,
		.x = GAME_W + 12,
		.seal = TRUE
	};
	void updater(s16 i){
		bossFlyIn(i);
		if(enemies[i].bools[1]){
			if(enemies[i].clock % 120 < 60){
				if(enemies[i].clock % 120 == 0){
					enemies[i].speed = 0;
					updateEnemyVel(i);
				}
				// shoot
				if(enemies[i].clock >= 0){
					if(enemies[i].health >= 25){
						if(enemies[i].clock % 15 == 0){
							struct bulletSpawner spawner = {
								.angle = 416,
								.speed = FIX16(1.75),
								.x = enemies[i].pos.x,
								.y = enemies[i].pos.y,
								.image = &smallGreenBullet
							};
							void updater(s16 j){
								if(bullets[j].clock % 2 == 1){
									if(bullets[j].clock % 60 < 15) bullets[j].angle -= 16;
									else if(bullets[j].clock % 60 < 30) bullets[j].angle += 16;
									else if(bullets[j].clock % 60 < 45) bullets[j].angle += 16;
									else bullets[j].angle -= 16;
									updateBulletVel(j);
								}
							}
							for(u8 j = 0; j < 3; j++){
								spawnBullet(spawner, updater);
								spawner.angle += 96;
							}
						}
						if(enemies[i].clock % 240 >= 155 && enemies[i].clock % 240 < 175 && enemies[i].clock % 10 == 5){
							if(enemies[i].clock % 240 == 155) enemies[i].ints[6] = 256 - 32 + random() % 64;
							struct bulletSpawner spawner = {
								.angle = enemies[i].ints[6] + 64 + 16,
								.speed = FIX16(1.5),
								.x = enemies[i].pos.x,
								.y = enemies[i].pos.y,
								.image = &bigYellowBullet,
								.big = TRUE
							};
							if(enemies[i].clock % 240 == 165) spawner.speed = fix16Add(spawner.speed, FIX16(0.75));
							for(u8 j = 0; j < 4; j++){
								spawner.angle += 64;
								spawnBullet(spawner, EMPTY);
							}
						}
					} else {
						if(!enemies[i].bools[7]){
							enemies[i].bools[7] = TRUE;
							killBullets = TRUE;
							enemies[i].clock = -60;
						} else {
							if(enemies[i].clock % 30 == 0){
								if(enemies[i].clock % 120 == 0){
									enemies[i].ints[6] = 256 - 64 + random() % 32;
									enemies[i].bools[6] = enemies[i].clock % 240 == 0;
								}
								struct bulletSpawner spawner = {
									.angle = enemies[i].ints[6],
									.speed = FIX16(2),
									.x = enemies[i].pos.x,
									.y = enemies[i].pos.y,
									.image = &smallYellowBullet
								};
								enemies[i].ints[6] += enemies[i].bools[6] ? -40 : 40;
								void updater(s16 j){
									if(bullets[j].bools[0] && bullets[j].clock == 30){
										bullets[j].vel = hone(bullets[j].pos, players[0].pos, FIX16(1.75), 64);
									} else if(bullets[j].clock > 0 && bullets[j].clock % 5 == 0 && !bullets[j].bools[0]){
										bullets[j].speed = fix16Sub(bullets[j].speed, FIX16(0.25));
										updateBulletVel(j);
										if(bullets[j].speed <= 0){
											bullets[j].bools[0] = TRUE;
											bullets[j].clock = -1;
										}
									}
								}
								for(u8 j = 0; j < 4; j++){
									spawner.angle += 128;
									spawnBullet(spawner, updater);
								}
							}
							if(enemies[i].clock % 120 == 45){
								struct bulletSpawner spawner = {
									.angle = 320 + 43,
									.speed = FIX16(1.25),
									.x = enemies[i].pos.x,
									.y = enemies[i].pos.y,
									.image = &bigGreenBullet,
									.big = TRUE
								};
								void updater(s16 j){
									if(bullets[j].clock > 0 && bullets[j].clock % 15 == 0){
										bullets[j].angle += bullets[j].bools[0] ? 24 : -24;
										updateBulletVel(j);
									}
								}
								for(u8 j = 0; j < 3; j++){
									spawner.bools[0] = FALSE;
									spawner.angle += 85;
									spawnBullet(spawner, updater);
									spawner.bools[0] = TRUE;
									spawnBullet(spawner, updater);
								}
							}
						}
					}
				}
			} else {
				if(enemies[i].clock % 120 == 60){
					enemies[i].speed = FIX16(0.75);
					enemies[i].angle = random() % 1024;
					updateEnemyVel(i);
				}
				bossMove(i);
			}
		}
	}
	void suicide(s16 i){
		killBullets = TRUE;
		waveClock = 60;
	}
	spawnEnemy(spawner, updater, suicide);
}

// loop

void loadStageOne(){
	// waveClock = 30;
	currentWave = 91;
}

void updateStageOne(){
	if(waveClock == 0){
		switch(currentWave){

			case   0: smallOne(GAME_H / 2); waveClock = 60; break;
			case   1: smallOne(GAME_H / 3); waveClock = 30; break;
			case   2: smallOne(GAME_H / 3 * 2); waveClock = 30; break;
			case   3: smallOne(GAME_H / 3 * 2 - 32); waveClock = 30; break;
			case   4: smallOne(GAME_H / 3 * 2 + 32); waveClock = 30; break;

			case   5: smallTwo(FALSE); waveClock = 30; break;
			case   6: smallTwo(FALSE); waveClock = 30; break;
			case   7: smallTwo(FALSE); waveClock = 30; break;
			case   8: smallTwo(FALSE); waveClock = 1; break;
			case   9: smallTwo(TRUE); waveClock = 30; break;
			case  10: smallTwo(TRUE); waveClock = 30; break;
			case  11: smallTwo(TRUE); waveClock = 30; break;
			case  12: smallTwo(TRUE); waveClock = 30; break;

			case  13: smallOne(GAME_H / 2); waveClock = 30; break;
			case  14: smallOne(GAME_H / 2 - 32); waveClock = 30; break;
			case  15: smallOne(GAME_H / 2 - 64); waveClock = 30; break;
			case  16: smallOne(GAME_H / 2); waveClock = 30; break;
			case  17: smallOne(GAME_H / 2 + 32); waveClock = 30; break;
			case  18: smallOne(GAME_H / 2 + 64); waveClock = 60; break;

			case  19: mediumOne(); break;

			case  20: smallTwo(TRUE); waveClock = 30; break;
			case  21: smallTwo(TRUE); waveClock = 30; break;
			case  22: smallTwo(TRUE); waveClock = 30; break;
			case  23: smallTwo(TRUE); waveClock = 1; break;
			case  24: smallTwo(FALSE); waveClock = 30; break;
			case  25: smallTwo(FALSE); waveClock = 30; break;
			case  26: smallTwo(FALSE); waveClock = 30; break;
			case  27: smallTwo(FALSE); waveClock = 30; break;

			case  28: smallOne(GAME_H / 2); waveClock = 30; break;
			case  29: smallOne(GAME_H / 2 + 64); waveClock = 30; break;
			case  30: smallOne(GAME_H / 2 - 32); waveClock = 60; break;

			case  31: mediumOne(); break;

			case  32: largeOne(FALSE); break;

			case  33: smallTwo(TRUE); waveClock = 30; break;
			case  34: smallTwo(TRUE); waveClock = 10; break;
			case  35: smallTwo(FALSE); waveClock = 30; break;
			case  36: smallTwo(FALSE); waveClock = 30; break;

			case  37: smallOne(GAME_H / 2); waveClock = 30; break;
			case  38: smallOne(GAME_H / 2 + 64); waveClock = 30; break;
			case  39: smallOne(GAME_H / 2 - 32); waveClock = 30; break;
			case  40: smallOne(GAME_H / 2); waveClock = 30; break;
			case  41: smallOne(GAME_H / 2 - 64); waveClock = 30; break;
			case  42: smallOne(GAME_H / 2 + 32); waveClock = 60; break;

			case  43: mediumTwo(GAME_H / 2); waveClock = 60; break;
			case  44: mediumTwo(GAME_H / 2 - 32); waveClock = 60; break;
			case  45: mediumTwo(GAME_H / 2 + 32); waveClock = 90; break;

			case  46: smallOne(GAME_H / 2); waveClock = 30; break;
			case  47: smallOne(GAME_H / 2 - 64); waveClock = 30; break;
			case  48: smallOne(GAME_H / 2 + 32); waveClock = 30; break;
			case  49: smallOne(GAME_H / 2); waveClock = 30; break;
			case  50: smallOne(GAME_H / 2 + 64); waveClock = 30; break;
			case  51: smallOne(GAME_H / 2 - 32); waveClock = 90; break;

			case  52: mediumTwo(GAME_H / 2); waveClock = 60; break;
			case  53: mediumTwo(GAME_H / 2 + 32); waveClock = 60; break;
			case  54: mediumTwo(GAME_H / 2 - 32); waveClock = 60; break;
			case  55: mediumTwo(GAME_H / 2); waveClock = 90; break;

			case  56: smallOne(GAME_H / 2); waveClock = 30; break;
			case  57: smallOne(GAME_H / 2 + 32); waveClock = 30; break;
			case  58: smallOne(GAME_H / 2 - 32); waveClock = 120; break;

			case  59: killBullets = TRUE; loadCutscene(0); break;

			case  60: wriggle(); break;

			case  61: loadCutscene(1); break;
		
			// 1:16
			case  62: smallTwo(TRUE); waveClock = 30; break;
			case  63: smallTwo(TRUE); waveClock = 10; break;
			case  64: smallTwo(FALSE); waveClock = 30; break;
			case  65: smallTwo(FALSE); waveClock = 30; break;

			case  66: smallThree(GAME_H / 3 * 2); waveClock = 30; break;
			case  67: smallThree(GAME_H / 3 * 2 - 32); waveClock = 60; break;
			case  68: smallThree(GAME_H / 3); waveClock = 30; break;
			case  69: smallThree(GAME_H / 3 + 32); waveClock = 60; break;
			case  70: smallThree(GAME_H / 2); waveClock = 30; break;
			case  71: smallThree(GAME_H / 2 - 64); waveClock = 30; break;
			case  72: smallThree(GAME_H / 2 + 64); waveClock = 30; break;
			case  73: smallThree(GAME_H / 2); waveClock = 60; break;

			case  74: largeOne(TRUE); break;

			case  75: smallTwo(FALSE); waveClock = 30; break;
			case  76: smallTwo(FALSE); waveClock = 10; break;
			case  77: smallTwo(TRUE); waveClock = 30; break;
			case  78: smallTwo(TRUE); waveClock = 30; break;

			case  79: smallOne(GAME_H / 2); waveClock = 30; break;
			case  80: smallOne(GAME_H / 2 - 64); waveClock = 30; break;
			case  81: smallOne(GAME_H / 2 + 32); waveClock = 30; break;
			case  82: smallOne(GAME_H / 2); waveClock = 30; break;
			case  83: smallOne(GAME_H / 2 + 64); waveClock = 30; break;
			case  84: smallOne(GAME_H / 2 - 32); waveClock = 60; break;

			case  85: smallThree(GAME_H / 2); waveClock = 60; break;
			case  86: smallThree(GAME_H / 3); waveClock = 30; break;
			case  87: smallThree(GAME_H / 3 + 32); waveClock = 30; break;
			case  88: smallThree(GAME_H / 3 * 2); waveClock = 30; break;
			case  89: smallThree(GAME_H / 3 * 2 - 32); waveClock = 30; break;
			case  90: smallThree(GAME_H / 2); waveClock = 60; break;

			case  91: largeTwo(); break;

		}
		currentWave++;
	}
}