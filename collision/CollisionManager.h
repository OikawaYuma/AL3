#pragma once
#include "Collider.h"
class CollisionManager {
public:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	void CheckAllCollision();

private:
};
