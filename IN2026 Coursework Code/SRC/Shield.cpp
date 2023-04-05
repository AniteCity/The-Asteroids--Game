#include <stdlib.h>
#include "GameUtil.h"
#include "Shield.h"
#include "BoundingShape.h"
#include "Bullet.h"
#include "Asteroid.h"


Shield::Shield(void) : GameObject("Shield")
{
	mAngle = rand() % 360;
	mRotation = rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 10.0 * cos(DEG2RAD * mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD * mAngle);
	mVelocity.z = 0.0;
}
Shield::~Shield(void)
{
}
void Shield::Render(void)
{

	if (mShieldShape.get() != NULL)
		mShieldShape->Render();



	GameObject::Render();


}

bool Shield::CollisionTest(shared_ptr<GameObject> o)
{

	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;

	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Shield::OnCollision(const GameObjectList& objects)
{
	for (auto& object : objects)
	{
		//Check if the object colliding is a bullet
		if (auto bullet = dynamic_cast<Bullet*>(object.get()))
		{
			OnCollisionWithBullet(*bullet);
		}

	}
}



void Shield::OnCollisionWithBullet(Bullet& bullet)
{
	// handle collision with a bullet object
	mWorld->FlagForRemoval(GetThisPtr());
	mWorld->FlagForRemoval(bullet.GetThisPtr());

}

