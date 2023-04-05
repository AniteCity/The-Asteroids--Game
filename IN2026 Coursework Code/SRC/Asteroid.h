#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"
#include "Life.h"
#include "Bullet.h"

class Asteroid : public GameObject
{
public:
	Asteroid(void);
	~Asteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

	void OnCollisionWithAsteroid(Asteroid& asteroid);

	void OnCollisionWithLife(Life& life);

	void OnCollisionWithBullet(Bullet& bullet);





};

#endif
