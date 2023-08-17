#include "Player.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include "function.h"
#include"ImGuiManager.h"
#include<Input.h>

#include"CollisionConfig.h"


Player::~Player() { for (PlayerBullet* bullet : bullets_){
		delete bullet;
	}; }

void Player::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();

	
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	// 衝突属性を設定
	SetCollisonAttribute(kCollisionAttributePlayer);

	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	



}

Vector3 Player::Rotate(Vector3 rot){

	
	return rot;

};

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {

		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {

		move.y -= kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_= Transform_Move(worldTransform_.translation_,move);

	//キーボード入力による移動処理

	//移動限界座標
	const float kMoveLimitX = 50;
	const float kMoveLimitY = 30;


	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}


	worldTransform_.UpdateMatrix();

	ImGui::Begin("Debug1");
	ImGui::Text(
	    "x:%f y:%f z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();

	// キャラクター攻撃処理
	Attack();


	
	//// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	//if (bullet_) {
	//	bullet_->Update();
	//}


}
void Player::Draw(ViewProjection viewProjection_) { 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
	/*if (bullet_) {
	bullet_->Draw(viewProjection_);
	}*/
}
	

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾があれば破棄する
	/*if (bullet_) {
		delete bullet_;
		bullet_ = nullptr;
	}*/
		//弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

	}
}

Vector3 Player::GetWorldPosition() const {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::OnCollision(){

};