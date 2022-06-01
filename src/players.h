// players

#define INVINCIBLE_LIMIT 180
#define INVINCIBLE_MID INVINCIBLE_LIMIT / 2

struct player {
	Vect2D_f16 pos, vel, off;
	s16 angle, moveClock, shotClock, clock, invincibleClock, lives, bombs;
	fix16 moveSpeed, shotSpeed;
	fix32 dist, enemyDist, bulletDist;
	bool invincible, p2, shooting, active, focus;
	Sprite* image;
};
struct player players[2];

void spawnPlayer(bool),
	loadPlayers(),
	updatePlayers();