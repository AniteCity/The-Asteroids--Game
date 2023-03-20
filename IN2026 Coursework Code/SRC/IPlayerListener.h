#ifndef __IPLAYERLISTENER_H__
#define __IPLAYERLISTENER_H__

class IPlayerListener
{
public:
	virtual void OnPlayerKilled(int lives_left) = 0;
	virtual void OnPlayerLifeKilled(int mLives) = 0;
	virtual void isTimeToShoot() = 0;
};

#endif
