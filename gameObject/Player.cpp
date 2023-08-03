#include "Player.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "function.h"
#include <Input.h>
#include <cassert>

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
		delete sprite2DReticle_;
	};
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {

	// NULLポインタチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();

	// 3Dレティクルのワールドトラベル初期化
	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	// スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {500, 500}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

Vector3 Player::Rotate(Vector3 rot) { return rot; };

void Player::Update(ViewProjection viewProjection) {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	worldTransform3DReticle_.TransferMatrix();

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
	worldTransform_.translation_ = Transform_Move(worldTransform_.translation_, move);

	// キーボード入力による移動処理

	// 移動限界座標
	const float kMoveLimitX = 50;
	const float kMoveLimitY = 30;

	// 範囲を超えない処理
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

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		// 自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		// 自機のワールド行列の回転を繁栄
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = Normalize(offset);
		offset.x *= kDistancePlayerTo3DReticle;
		offset.y *= kDistancePlayerTo3DReticle;
		offset.z *= kDistancePlayerTo3DReticle;
		// 3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = Add(offset, worldTransform_.translation_);

		worldTransform3DReticle_.UpdateMatrix();
	}

	// 3Dレティクルのワールド座標から2レティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = GetReticleWorldPosition();

		// ビューポート
		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matVireProjectionViewport =
		    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

		// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
		positionReticle = Transform(positionReticle, matVireProjectionViewport);

		// スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
		// レティクル用テクスチャ取得
		uint32_t textureReticle = TextureManager::Load("Reticle.png");
		// スプライト生成
		sprite2DReticle_ = Sprite::Create(
		    textureReticle, {positionReticle.x, positionReticle.y}, {1.0f, 1.0f, 1.0f, 1.0f},
		    {0.5f, 0.5f});
	}

	ImGui::Begin("Debug1");
	ImGui::Text("%f", worldTransform_.matWorld_.m[3][2]);
	ImGui::End();

	// キャラクター攻撃処理
	Attack();

	//// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	// if (bullet_) {
	//	bullet_->Update();
	// }
}
void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);
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
		// 弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;

		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetReticleWorldPosition() {
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision(){

};

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

void Player::DrawUI() {

	// スプライト生成
	sprite2DReticle_->Draw();
}