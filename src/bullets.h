// bullets

#define BULLET_COUNT 64
#define B_COUNT_INT 8

struct bulletSpawner {
	fix16 x, y, speed;
	Vect2D_f16 vel;
	s16 angle, frame;
	Sprite* image;
	bool big, player, p2;
	bool bools[B_COUNT_INT];
	s16 ints[B_COUNT_INT];
	fix16 fixes[B_COUNT_INT];
};

struct bullet {
	bool active, player, p2, ball, flipped, powerup, dead;
	fix16 speed;
	fix32 dist;
	Vect2D_f16 pos, vel, off;
	s16 angle, clock, nextX, nextY, currentX, currentY;
	Sprite* image;
	void (*updater)(s16);
	bool bools[B_COUNT_INT];
	s16 ints[B_COUNT_INT];
	fix16 fixes[B_COUNT_INT];
};
struct bullet bullets[BULLET_COUNT];

bool killBullets;

void spawnBullet(struct bulletSpawner, void(*updater)),
	killBullet(s16),
	updateBulletVel(s16),
	updateBullets();