#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameUtil.h"

#include "GameObject.h"
#include "GameObjectType.h"
#include "IPlayerListener.h"
#include "IGameWorldListener.h"

class Player : public IGameWorldListener
{
public:
	Player() { mLives = 3; }
	virtual ~Player() {}

	void OnWorldUpdated(GameWorld* world) {}

	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}

	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
	{
		//If its a spaceship
		if (object->GetType() == GameObjectType("Spaceship")) {
			//Check if its a normal spaceship
			if (object->GetScale() == 0.1f) {

				mLives -= 1;
				PlayerKilled();
			}
			//Check if its a spaceship with a shield
			else if (object->GetScale() == 0.11f) {

				mLives = mLives;
				PlayerKilled();

			}
		}//Check if the object is life.
		else if (object->GetType() == GameObjectType("Life")) {
			mLives += 1;
			PlayerKillLife();


		}
	}
	void AddListener(shared_ptr<IPlayerListener> listener)
	{
		mListeners.push_back(listener);
	}



	void PlayerKillLife()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnPlayerLifeKilled(mLives);
		}
	}

	void PlayerKilled()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnPlayerKilled(mLives);
		}
	}

private:
	int mLives;

	typedef std::list< shared_ptr<IPlayerListener> > PlayerListenerList;

	PlayerListenerList mListeners;
};

#endif
