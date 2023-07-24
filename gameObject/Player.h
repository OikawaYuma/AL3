#pragma once
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <cassert>
#include <input.h>
#include <list>

class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Initialize(Model* model, uint32_t textureHandle);

	Vector3 Rotate(Vector3 rot);

	/// <summary>
	/// 初期化
	/// </ summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </ summary>
	void Draw(ViewProjection);
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	Vector3 GetWorldPosition();
	int GetRadius() { return radius_; }
	// 衝突を検出したらコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& Getbullet() const { return bullets_; }

	/// <summary>
	/// 親となるワールドトランスフォームセット
	/// </summary>
	///	<parma name="parent">親となるワールドトランスフォーム<.param>
	void SetParent(const WorldTransform* parent);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	int radius_ = 2;

	// キーボード入力
	Input* input_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;
};
