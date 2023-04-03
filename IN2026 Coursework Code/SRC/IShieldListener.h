#ifndef _ISHIELDLISTENER_H_
#define _ISHIELDLISTENER_H_

class IShieldListener
{
public:
	virtual void OnShieldChanged(bool shield) = 0;
};
#endif