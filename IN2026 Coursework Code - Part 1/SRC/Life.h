#pragma once
#ifndef _LIFE_H_
#define _LIFE_H_
 
#include "GameObject.h"
#include "Bullet.h"



class Life : public GameObject
{
public:
	Life(void);
	~Life(void);
	void Render(void);


	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	void SetLifeShape(shared_ptr<Shape> life_shape) { mLifeShape = life_shape; }


	void OnCollisionWithBullet(Bullet& bullet);
private:
 
	shared_ptr<Shape> mLifeShape;
};

#endif