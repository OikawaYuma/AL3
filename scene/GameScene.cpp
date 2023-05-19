#include "GameScene.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include"ImGuiManager.h"

GameScene::GameScene() { }

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTh_ = TextureManager::Load("BOUFLY1.png");
	sprite_ = Sprite::Create(playerTh_, {100, 50});

	//3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,playerTh_);


	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

}

void GameScene::Update() {
// 自キャラの更新あ
	player_->Update();
	if (input_->TriggerKey(DIK_Q) && isDebugCameraActive_ ==false) {
		isDebugCameraActive_ = true;
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

#endif // 
	// カメラの処理
	if (isDebugCameraActive_) {
	
			// デバックカメラの更新
		debugCamera_->Update();
	//デバッグカメラの更新
		viewProjection_.matView = Inverse(viewProjection_.matView);
		viewProjection_.matProjection = MakePerspectiveFovMatrix(0.45f,float(1280)/float(720),0.1f,100.0f);
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	ImGui::Begin("Debug2");
	ImGui::Text(  "%d",isDebugCameraActive_);
	ImGui::End();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
