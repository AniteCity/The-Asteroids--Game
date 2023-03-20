#include "GameWorld.h"
#include "BulletAlien.h"
#include "BoundingSphere.h"
#include "Spaceship.h"
 

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Bullets live for 2s by default. */
BulletAlien::BulletAlien()
	: GameObject("BulletAlien"), mTimeToLive(2000)
{
}

/** Construct a new bullet with given position, velocity, acceleration, angle, rotation and lifespan. */
BulletAlien::BulletAlien(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl)
	: GameObject("BulletAlien", p, v, a, h, r), mTimeToLive(ttl)
{ 
}






/** Copy constructor. */
BulletAlien::BulletAlien(const BulletAlien& b)
	: GameObject(b),
	mTimeToLive(b.mTimeToLive)
{
}

/** Destructor. */
BulletAlien::~BulletAlien(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update bullet, removing it from game world if necessary. */
void BulletAlien::Update(int t)
{
	// Update position/velocity
	GameObject::Update(t);
	// Reduce time to live
	mTimeToLive = mTimeToLive - t;
	// Ensure time to live isn't negative
	if (mTimeToLive < 0) { mTimeToLive = 0; }
	// If time to live is zero then remove bullet from world
	if (mTimeToLive == 0) {
		if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
	}

}

bool BulletAlien::CollisionTest(shared_ptr<GameObject> o)
{

	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void BulletAlien::OnCollision(const GameObjectList& objects)
{
	for (auto& object : objects)
	{

		if (auto spaceship = dynamic_cast<Spaceship*>(object.get()))
		{
			OnCollisionWithSpaceship(*spaceship);
		}

		// add other cases for other types of game objects as needed
	}
}



void BulletAlien::OnCollisionWithSpaceship(Spaceship& spaceship)
{
	// handle collision with a life object
	mWorld->FlagForRemoval(GetThisPtr());
	mWorld->FlagForRemoval(spaceship.GetThisPtr());

}