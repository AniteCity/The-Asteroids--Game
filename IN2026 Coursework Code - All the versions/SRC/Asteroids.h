#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "IShieldListener.h" 

#include "ScoreKeeper.h"
#include "ShieldKeeper.h"
#include "ShieldKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"

class GameObject;
class Spaceship;
class GUILabel;
 

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IShieldListener, public IScoreListener, public IPlayerListener 
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IScoreListener interface //////////////////////////////////
	void OnShieldChanged(bool shield);
	void OnScoreChanged(int score);
 

	void PlayerKilled(int lives_left);
 

	// Declaration of the IPlayerLister interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);
 
private:
	shared_ptr<Spaceship> mSpaceship;
	 
 
	
	 
	 
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mGameStartLabel;
	shared_ptr<GUILabel> mLivesLabel;
	//
	shared_ptr<GUILabel> mGameOverLabel;
	shared_ptr<GUILabel> mGameNameLabel;
	 
	 

	uint mLevel;
	uint mAsteroidCount;
	uint mStartBoolean;
	uint mStartTrigger;

	void ResetSpaceship();
	shared_ptr<GameObject> CreateSpaceship();
	void CreateShield(const uint num_shield);
	void CreateGUI();
	void CreateLifePopUpMessage();
	
	void CreateStartGUI();
	void CreateAsteroids(const uint num_asteroids);
	void CreateLife(const uint num_life);
	void CreateShieldGUI();
	void OnPlayerLifeKilled(int lives_left);
	bool CollisionTest(shared_ptr<GameObject> o);
	shared_ptr<GameObject> CreateExplosion();
	
	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;
const static uint SHOW_LIFE_POP_UP = 3;
	const static uint HIDE_LIFE_POP_UP = 4;
	const static uint BLINK_START_MESSAGE = 9;

	ScoreKeeper mScoreKeeper;
	ShieldKeeper mShieldKeeper;
	Player mPlayer;
};

#endif