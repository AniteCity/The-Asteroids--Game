#ifndef _SCOREKEEPER_H_
#define _SCOREKEEPER_H_
#include "GameUtil.h"
#include "GameObject.h"
#include "GameObjectType.h"
#include "IShieldListener.h"
#include "IGameWorldListener.h"
class ShieldKeeper : public IGameWorldListener
{
public:
	ShieldKeeper() { mShield = false; }
	virtual ~ShieldKeeper() {}
	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
	{
		if (object->GetType() == GameObjectType("Shield")) {
			mShield = true;
			FireScoreChanged();
		}if (object->GetType() == GameObjectType("Spaceship")) {
			mShield = false;
			FireScoreChanged();
		}
	}
	void AddListener(shared_ptr<IShieldListener> listener)
	{
		mListeners.push_back(listener);
	}
	void FireScoreChanged()
	{
		// Send message to all listeners
		for (ShieldListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnShieldChanged(mShield);
		}
	}
private:
	bool mShield;
	typedef std::list< shared_ptr<IShieldListener> > ShieldListenerList;
	ShieldListenerList mListeners;
};
#endif