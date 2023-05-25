#include "PlayerBullet.h"
#include<cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
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

	
}

void PlayerBullet::Draw(ViewProjection viewProjectiom) {

	model_->Draw(worldTransform_, viewProjectiom, textureHandle_);

};