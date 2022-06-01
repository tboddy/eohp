// enemies

#define ENEMY_COUNT 32
#define E_COUNT_INT 8

struct enemySpawner {
	fix16 speed;
	s16 angle, offX, offY, x, y, anim, health, frame;
	Vect2D_f16 vel;
	Sprite* image;
	bool boss;
	bool bools[E_COUNT_INT];
	s16 ints[E_COUNT_INT];
	fix16 fixes[E_COUNT_INT];
};

struct enemy {
	bool active, boss, seen;
	fix16 speed;
	fix32 dist;
	Vect2D_f16 pos, vel, off;
	s16 angle, clock, health, shotClock;
	Sprite* image;
	void (*updater)(s16);
	void (*suicide)(s16);
	bool bools[E_COUNT_INT];
	s16 ints[E_COUNT_INT];
	fix16 fixes[E_COUNT_INT];
};
struct enemy enemies[ENEMY_COUNT];

Vect2D_f16 velPos, velPos2;

void killEnemy(s16),
	updateEnemyVel(s16),
	updateEnemies();