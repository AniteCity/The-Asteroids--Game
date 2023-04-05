#include <stdlib.h>
#include "GameUtil.h"
#include "Life.h"
#include "BoundingShape.h"
#include "Bullet.h"
#include "Asteroid.h"

//Make the life object move randomly.
Life::Life(void) : GameObject("Life")
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
Life::~Life(void)
{
}

//Render a sprite in on the life object.
void Life::Render(void)
{

	if (mLifeShape.get() != NULL)
		mLifeShape->Render();

	GameObject::Render();


}

bool Life::CollisionTest(shared_ptr<GameObject> o)
{

	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Life::OnCollision(const GameObjectList& objects)
{
	for (auto& object : objects)
	{

		if (auto bullet = dynamic_cast<Bullet*>(object.get()))
		{
			OnCollisionWithBullet(*bullet);
		}
		
	}
}



void Life::OnCollisionWithBullet(Bullet& bullet)
{
	// handle collision with a bullet object
	mWorld->FlagForRemoval(GetThisPtr());
	mWorld->FlagForRemoval(bullet.GetThisPtr());
}


