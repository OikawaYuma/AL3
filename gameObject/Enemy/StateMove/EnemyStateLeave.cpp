#include "EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelo({-0.2f, 0.2f, -0.2f});
	pEnemy->Move();
}
