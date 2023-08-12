#include "EnemyStateApoorch.h"
#include"EnemyStateLeave.h"
#include"Enemy.h"
void EnemyStateApoorch::Update(Enemy* pEnemy) {

	pEnemy->SetVelo({0, 0, -0.2f});
	pEnemy->Move();

	if (pEnemy->GetTranslation().z <= 0) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
	/*
	switch (phase_) {
	case Phase::Approach:
	default:
	    MoveApproach();
	    break;
	case Phase::Leave:
	    MoveLeave();
	    break;
	}*/
}
