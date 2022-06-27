// main

bool started;

// metrics

#define GAME_W 224
#define GAME_H 192
#define GAME_X 16
#define GAME_Y 32
#define WIN_W 320
#define WIN_H 224
#define GAME_TILE_W GAME_W / 8
#define GAME_TILE_H GAME_H / 8
#define WIN_TILE_W WIN_W / 8
#define WIN_TILE_H WIN_H / 8

s16 clock, bossHealth, bossMax;
#define CLOCK_LIMIT 32000

void EMPTY(u8),
	loadGame();

fix16 honeSpeed;
Vect2D_f16 hPos;
Vect2D_f16 hone(Vect2D_f16, Vect2D_f16, fix16, s16);


// shaders

#define LEAST_TILE 2
#define HALF_TILE LEAST_TILE + 1
#define MOST_TILE HALF_TILE + 1
#define FULL_TILE MOST_TILE + 1

bool gameOver, inCutscene, gameLost, gameWon;