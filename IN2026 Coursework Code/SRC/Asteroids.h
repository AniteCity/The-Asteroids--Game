#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "ScoreKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"

class GameObject;
class Spaceship;
class GUILabel;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IScoreListener, public IPlayerListener
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

	void OnScoreChanged(int score);

	// Declaration of the IPlayerLister interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	void OnPlayerLifeKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	void SpaceShipSprite();

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);

private:
	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<Spaceship> mShieldSpaceship;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mGameStartLabel;
	shared_ptr<GUILabel> mLivesLabel;
	shared_ptr<GUILabel> mLivesPopUpLabel;
	shared_ptr<GUILabel> mGameOverLabel;
	//shared_ptr<Sprite> shield_spaceship_sprite;

	uint mLevel;
	uint mAsteroidCount;
	uint mStartBoolean;
	uint mStartTrigger;

	void ResetSpaceship();
	shared_ptr<GameObject> CreateSpaceship();
	shared_ptr<GameObject> CreateShieldSpaceship();
	void CreateLifePopUpGUI();
	void CreateGUI();
	void CreateStartGUI();
	void CreateAsteroids(const uint num_asteroids);
	void CreateLife(const uint num_life);
	void CreateShield(const uint num_life);
	bool CollisionTest(shared_ptr<GameObject> o);
	shared_ptr<GameObject> CreateExplosion();
	
	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;
	const static uint SHOW_LIFE_POP_UP = 3;
	const static uint HIDE_LIFE_POP_UP = 4;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;
};

#endif