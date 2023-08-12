#include "Enemy.h"
#include "ImGuiManager.h"
#include"EnemyStateApoorch.h"

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



