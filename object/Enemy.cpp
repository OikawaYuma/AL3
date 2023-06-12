#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("BOSS1.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {0, 5, 50};
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	/*(this->*pMoveTable[0])();*/

	state = new EnemyStateApoorch();
}

void Enemy::Update() { state->Update(this); 

worldTransform_.UpdateMatrix();

Attack();

//// 弾更新
for (EnemyBullet* bullet : bullets_) {
	bullet->Update();
}
// if (bullet_) {
//	bullet_->Update();
// }



ImGui::Begin("Debug2");
	ImGui::Text("t.z : %f\n%f", worldTransform_.translation_.z,velocity_.z);
	ImGui::End();}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
void Enemy::Move() {
	worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, velocity_);
}
//
// void Enemy::MoveApproach() {
//	velocity_.z = -0.2f;
//	if (worldTransform_.translation_.z < 0.0f) {
//		phase_ = Phase::Leave;
//	}
//}
// void Enemy::MoveLeave() {
//	velocity_.z = 0;
//	velocity_.x = -0.2f;
//	velocity_.y = +0.2f;
//
//}

// setter
//  setter
void Enemy::SetVelo(Vector3 velocity) {
	velocity_.x = velocity.x;
	velocity_.y = velocity.y;
	velocity_.z = velocity.z;
}

// staticで宣言したメンバ関数ポインタテーブルの実態
// void (Enemy::*Enemy::pMoveTable[])() = {&Enemy::MoveApproach, &Enemy::MoveLeave};

// state変更用の関数

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }

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

void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelo({-0.2f, 0.2f, -0.2f});
	pEnemy->Move();
}

void Enemy::Attack() { 
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}