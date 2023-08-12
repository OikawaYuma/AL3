#pragma once
#include"Enemy.h"

class EnemyStateLeave;
class EnemyStateApoorch : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};
