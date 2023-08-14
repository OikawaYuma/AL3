#pragma once
#include"Vector3.h"
class Collider {
public:


	// Getter
	// 半径を取得
	float GetRadius() { return radius_; }

	// Setter
	void SetRadius(float radius) { radius_ = radius; }

	virtual void OnCollision() = 0;

	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() const = 0;

private:
	// 衝突半径
	float radius_ = 2;
};
