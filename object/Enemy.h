#pragma once
#include "EnemyBullet.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "function.h"
#include <Input.h>
#include <cassert>

class Enemy;

class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
	
};

class EnemyStateApoorch : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class Enemy {
public:
	/*Enemy();
	~Enemy();*/
	void Initialize(Model* model, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Move();
	// void MoveApproach();
	// void MoveLeave();
	// getter
	Vector3 GetVelo() { return velocity_; }

	Vector3 GetTranslation() { return worldTransform_.translation_; }

	// setter
	void SetVelo(Vector3 velocity);
	void SetShotInterval(int32_t shotIntervalTimer);

	// state

	void ChangeState(BaseEnemyState* newState);

	// 攻撃
	void Fire();


	// 弾の発射間隔
	static const int kFireInterval = 60*3;
	

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	// ViewProjection viewProjection_;

	// モデルのポインタ
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 行動フェーズ
	// enum class Phase {
	//	Approach, // 接近する
	//	Leave     // 離脱する
	//};

	// フェーズ
	/*Phase phase_ = Phase::Approach;*/

	// メンバポインタ関数のテーブル
	/*static void (Enemy::*pMoveTable[])();*/
	// キーボード入力
	Input* input_ = nullptr;
	// state
	BaseEnemyState* state;

	// 弾
	std::list<EnemyBullet*> bullets_;
	// EnemyBullet* bullet_ = nullptr;

	// 発射タイマー
	int32_t shotIntervalTimer_;
};
