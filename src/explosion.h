// explosion

#define EXPLOSION_LIMIT 16

struct explosion {
	bool active;
	s16 clock;
	Sprite* image;
};

struct explosion explosions[EXPLOSION_LIMIT];

void spawnExplosion(fix16, fix16),
	updateExplosions();