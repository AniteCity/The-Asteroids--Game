#include <stdlib.h>
#include "GameUtil.h"
#include "Life.h"
#include "BoundingShape.h"
#include "Bullet.h"
#include "Asteroid.h"


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
void Life::Render(void)
{
	// Disable lighting for solid colour lines
	glDisable(GL_LIGHTING);
	// Start drawing lines
	glBegin(GL_LINE_LOOP);
	// Set colour to grey
	glColor3f(0.6, 0.6, 0.6);
	// Add vertices to draw an octagon
	glVertex3f(-7, -7, 0.0);
	glVertex3f(-10, 0, 0.0);
	glVertex3f(-7, 7, 0.0);
	glVertex3f(0, 10, 0.0);
	glVertex3f(7, 7, 0.0);
	glVertex3f(10, 0, 0.0);
	glVertex3f(7, -7, 0.0);
	glVertex3f(0, -10, 0.0);
	// Finish drawing lines
	glEnd();
	// Enable lighting
	glEnable(GL_LIGHTING);
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

		// add other cases for other types of game objects as needed
	}
}

 

void Life::OnCollisionWithBullet(Bullet& bullet)
{
	// handle collision with a life object
	mWorld->FlagForRemoval(GetThisPtr());
	mWorld->FlagForRemoval(bullet.GetThisPtr());

}

