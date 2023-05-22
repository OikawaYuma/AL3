﻿#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include <input.h>

class Player {
public:
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

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;


};
