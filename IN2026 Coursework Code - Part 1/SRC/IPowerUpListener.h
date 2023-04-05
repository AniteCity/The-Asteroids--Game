#ifndef __IPOWERUPLISTENER_H__
#define __IPOWERUPLISTENER_H__

class IPowerUpListener
{
public:
	virtual void OnShieldKilled(int no_shield) = 0;
};

#endif
