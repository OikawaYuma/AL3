#include "PlayerBullet.h"
#include"function.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("BOUFLY1.png");

	// WorldTransformの初期化
	worldTransform_.Initialize();
	// ViewProjectionの初期化
	viewProjection_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	
}
void PlayerBullet::Update() {

	Transform_Move(worldTransform_.translation_,velocity_);
	//行列を更新
	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {

		isDead_ = true;
	}

}

void PlayerBullet::Draw(ViewProjection viewProjectiom) {

	model_->Draw(worldTransform_, viewProjectiom, textureHandle_);

};