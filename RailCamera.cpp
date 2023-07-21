#include "RailCamera.h"
#include<Input.h>
#include"function.h"
#include<ImGuiManager.h>

void RailCamera::Initialize(Vector3 pos, Vector3 rot) {	
	// 引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_ = pos;
	// 引数で回転角[ラジアンを受け取ってワールドトランスフォームに設定
	worldTransform_.rotation_ = rot;

	worldTransform_.UpdateMatrix();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

};

void RailCamera::Update(){
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

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

	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::Text(
	    "%f  %f  %f", worldTransform_.matWorld_.m[1][3], worldTransform_.matWorld_.m[2][3],
	    worldTransform_.matWorld_.m[0][3]);
	ImGui::End();


};