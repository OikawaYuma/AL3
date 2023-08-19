#include "CollisionManager.h"

void CollisionManager::CheckAllCollision() {

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->Getbullet();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->Getbullet();

	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);

	for (PlayerBullet* bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	for (EnemyBullet* bullet : enemyBullets) {
		colliders_.push_back(bullet);
	}

	// std::list<Collider*> colliders;
	//  リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		// イテレータBはイテレータ―Aの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {

			// ペアの当たり判定
			CheckCollisionPair(*itrA, *itrB);
		}
	}
