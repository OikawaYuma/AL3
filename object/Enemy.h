#pragma once
#include"Vector3.h"
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include<cassert>
#include"function.h"
class Enemy {
public:
	~Enemy();
	void Initialize(Model *model,const Vector3& velocity);
	void Update();
	void Draw(ViewProjection viewProjection);
	void MoveApproach();
	void MoveLeave();
	//getter
	

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	//ViewProjection viewProjection_;
	
	//モデルのポインタ
	Model *model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave     // 離脱する
	};

	// フェーズ
	Phase phase_ = Phase::Approach;

	// メンバポインタ関数のテーブル
	static void (Enemy::*pMoveTable[])();

};
