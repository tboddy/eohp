// bricks

#define BRICK_COUNT 98
#define BRICK_LABEL_COUNT 16

#define BRICK_W 14
#define BRICK_H 7
#define BRICK_W_PX BRICK_W * 8
#define BRICK_H_PX BRICK_H * 8

#define BRICK_COLLISION_COUNT 12288

u8 bricksCollision[BRICK_COLLISION_COUNT];

struct brick {
	bool active, invincible;
	s16 x, y, type;
};
struct brick bricks[BRICK_COUNT];

struct brickLabel {
	bool active;
	s16 x, y, clock;
};
struct brickLabel brickLabels[BRICK_LABEL_COUNT];

u8 bricksStageOne[BRICK_COUNT];

void loadBricks(),
	killBrick(s16),
	updateBricks();