#include "PlayerBullet.h"
#include"function.h"
#include<cassert>

#include"CollisionConfig.h"

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

	// 衝突属性を設定
	SetCollisonAttribute(0);

	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(1);
	

	
}
void PlayerBullet::Update() {

	worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, velocity_);
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

Vector3 PlayerBullet::GetWorldPosition() const {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


void PlayerBullet::OnCollision(){ 
	isDead_ = true;
};