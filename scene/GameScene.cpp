#include "GameScene.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include"ImGuiManager.h"
#include"AxisIndicator.h"


GameScene::GameScene() { }

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTh_ = TextureManager::Load("BOUFLY1.png");
	sprite_ = Sprite::Create(playerTh_, {100, 50});

	//3Dモデルの生成
	model_ = Model::Create();

	
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,playerTh_);

	//敵キャラの生成
	enemy_ = new Enemy;
	//敵キャラの初期化
	enemy_->Initialize(model_, {0,0,-0.2f});

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球の生成
	skydome_ = new Skydome;
	
	// レールカメラの生成
	railCamera_ = new RailCamera;
	// レールカメラの初期化
	railCamera_->Initialize();

	

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// farZの変更
	viewProjection_.farZ = 1000.0f;
	viewProjection_.Initialize();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);


	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	

}

void GameScene::Update() {
	skydome_->Update();
	    // 自キャラの更新あ
	player_->Update();
	
	//敵キャラの更新
	enemy_->Update();
	CheckAllCollision();
	
	railCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {

		// デバックカメラの更新
		debugCamera_->Update();
		// デバッグカメラの更新
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	ImGui::Begin("Debug2");
	ImGui::Text("%d", enemy_);
	ImGui::End();

 // 
	

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
	skydome_->Draw(debugCamera_->GetViewProjection());
	// 自キャラの描画
	player_->Draw(debugCamera_->GetViewProjection());

	// 敵キャラの描画
	enemy_->Draw(debugCamera_->GetViewProjection());

	

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

void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	int radiusA, radiusB;
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->Getbullet();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->Getbullet();

	
	#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	radiusA = player_->GetRadius();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	// 敵弾の座標
		posB = bullet->GetWorldPosition();
		float p2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		radiusB = bullet->GetRadius();
		int r2r = (radiusA + radiusB) * (radiusA + radiusB);

		if (p2b <= r2r) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 自キャラの衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定

	// 自キャラの座標
	posA = enemy_->GetWorldPosition();
	radiusA = enemy_->GetRadius();
	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		float e2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		            (posB.z - posA.z) * (posB.z - posA.z);
		radiusB = bullet->GetRadius();
		int r2r = (radiusA + radiusB) * (radiusA + radiusB);

		if (e2b <= r2r) {
			// 自キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自キャラの衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}


	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定

	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets) {
			// 自弾の座標
			posA = bulletA->GetWorldPosition();
			radiusA = bulletA->GetRadius();
			// 敵弾の座標
			posB = bulletB->GetWorldPosition();
			radiusB = bulletB->GetRadius();
			float a2b = (posB.x - posA.x) * (posB.x - posA.x) +
			            (posB.y - posA.y) * (posB.y - posA.y) +
			            (posB.z - posA.z) * (posB.z - posA.z);

			int r2r = (radiusA + radiusB) * (radiusA + radiusB);

			if (a2b <= r2r) {
				// 自弾の衝突時コールバックを呼び出す
				bulletA->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				bulletB->OnCollision();
			}
		}
	}

	#pragma endregion

}
