
#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

#include <LuaBridge\RefCountedObject.h>

class IGameState : public RefCountedObject
{
public:
	virtual ~IGameState() {}

	virtual void OnEnter() = 0;

	virtual void OnExit() = 0;
};

#endif // !_IGAMESTATE_H_