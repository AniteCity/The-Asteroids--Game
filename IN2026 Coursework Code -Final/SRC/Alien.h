#pragma once
#ifndef _ALIEN_H_
#define _ALIEN_H_

#include "GameObject.h"
#include "Bullet.h"
#include "Spaceship.h"



class Alien : public GameObject
{
public:
	Alien();
	Alien(GLVector3f p, GLVector3f v, GLfloat h, GLfloat r);
	Alien(const Alien& e);
	virtual ~Alien(void);
	void Render(void);

	virtual void Update(int t);
	virtual void Shoot(void);

	void SetSpaceshipShape(shared_ptr<Shape> spaceship_shape) { mSpaceshipShape = spaceship_shape; }
	void SetThrusterShape(shared_ptr<Shape> thruster_shape) { mThrusterShape = thruster_shape; }
	void SetBulletShape(shared_ptr<Shape> bullet_shape) { mBulletShape = bullet_shape; }


	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

	void OnCollisionWithSpaceship(Spaceship& spaceship);

	void OnCollisionWithBullet(Bullet& bullet);


private:
	float mThrust;

	shared_ptr<Shape> mSpaceshipShape;
	shared_ptr<Shape> mThrusterShape;
	shared_ptr<Shape> mBulletShape;
};

#endif