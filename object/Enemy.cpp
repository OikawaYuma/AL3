#include "Enemy.h"


void Enemy::Initialize(Model* model, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("BOSS1.png");


	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = {0, 5, 50};
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;



}

void Enemy::Update() {
	
	Move();
	
	

}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);


}

void Enemy::Move() { 
	switch (phase_) { 
	case Phase::Approach:
	default:
		velocity_.z = -0.2f;
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		
		break;
	case Phase::Leave:
		velocity_.z = 0.2f;
		break;
	}
	Transform_Move(worldTransform_.translation_, velocity_);
	
	/// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();


}
