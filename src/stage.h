// stage

s32 waveClock;
s16 currentWave;

void bossMove(s16),
	transferBoss(s16),
	spawnBoss(s16, void(*bossUpdater));

void loadStage(),
	updateStage();