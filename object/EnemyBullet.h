#pragma once
#include <Vector3.h>
#include"Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

class EnemyBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection viewProjection);

	// getter
	bool GetIsDead() const { return isDead_; }

	Vector3 GetWorldPosition() const override;
	int GetRadius() { return radius_; }
	// 衝突を検出したらコールバック関数
	void OnCollision() override;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// モデル
	Model* model_ = nullptr;

	int radius_ = 2;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 2;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

};
