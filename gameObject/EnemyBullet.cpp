#include "EnemyBullet.h"
#include "function.h"
#include <cassert>

#include<ImGuiManager.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("BOUFLY1.png");

	// WorldTransformの初期化
	worldTransform_.Initialize();
	// ViewProjectionの初期化
	viewProjection_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}
void EnemyBullet::Update() {

	worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, velocity_);
	ImGui::Begin("Debug3");
	ImGui::Text("bullet : %d\n",deathTimer_);

	ImGui::End();
	// 行列を更新
	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {

		isDead_ = true;
		
	}


}

void EnemyBullet::Draw(ViewProjection viewProjectiom) {
	model_->Draw(worldTransform_, viewProjectiom, textureHandle_);
};

Vector3 EnemyBullet::GetWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision(){ isDead_ = true; };