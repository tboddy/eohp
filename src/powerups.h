// powerups

#define POWERUP_COUNT 8

struct powerup {
	bool active;
	Vect2D_f16 pos;
	Sprite* image;
	u8 type;
	fix16 start, count;
};
struct powerup powerups[POWERUP_COUNT];

void spawnPowerup(fix16, fix16, u8),
	killPowerup(s16),
	updatePowerups();