#pragma once

#include <ViewProjection.h>
#include <WorldTransform.h>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 pos, float radian);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};
