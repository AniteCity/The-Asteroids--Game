#include "GameWorld.h"
#include "BoundingSphere.h"
#include "Alien.h"
#include "BulletAlien.h"
#include "Bullet.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. */

Alien::Alien(void) : GameObject("Alien")
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

void Alien::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}


/** Shoot a bullet. */
void Alien::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet
	(new BulletAlien(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);

}

/** Copy constructor. */
Alien::Alien(const Alien& e) : GameObject(e) {}

/** Destructor. */
Alien::~Alien(void) {}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update explosion, removing it from game world if necessary. */
void Alien::Update(int t)
{
	// Update position/velocity
	GameObject::Update(t);

	if (mSprite.get() == NULL) return;

	// If sprite has finished animation remove explosion from world
	if (!mSprite->IsAnimating()) {
		if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
	}
}



bool Alien::CollisionTest(shared_ptr<GameObject> o)
{

	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Alien::OnCollision(const GameObjectList& objects)
{
	for (auto& object : objects)
	{
		 
		if (auto spaceship = dynamic_cast<Spaceship*>(object.get()))
		{
			OnCollisionWithSpaceship(*spaceship);
		}else if (auto bullet = dynamic_cast<Bullet*>(object.get()))
		{
			OnCollisionWithBullet(*bullet);
		}
		// add other cases for other types of game objects as needed
	}
}


void Alien::OnCollisionWithSpaceship(Spaceship& Spaceship)
{
	 
	//mWorld->FlagForRemoval(GetThisPtr());
	mWorld->FlagForRemoval(Spaceship.GetThisPtr());
	
}

void Alien::OnCollisionWithBullet(Bullet& bullet)
{
	// handle collision with a life object
	mWorld->FlagForRemoval(GetThisPtr());
	mWorld->FlagForRemoval(bullet.GetThisPtr());

}
