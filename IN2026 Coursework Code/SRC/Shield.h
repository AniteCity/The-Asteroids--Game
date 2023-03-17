#pragma once
#ifndef _SHIELD_H_
#define _SHIELD_H_

#include "GameObject.h"
#include "Bullet.h"



class Shield : public GameObject
{
public:
	Shield(void);
	~Shield(void);
	void Render(void);


	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);


	void OnCollisionWithBullet(Bullet& bullet);
};

#endif