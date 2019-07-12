
#ifndef _GAMESTATEWRAPPER_H_
#define _GAMESTATEWRAPPER_H_

#include <LuaBridge\LuaBridge.h>

#include "IGameState.h"

class GameStateWrapper : public IGameState
{
public:
	GameStateWrapper(luabridge::LuaRef selfRef, luabridge::LuaRef onEnterFunc, luabridge::LuaRef onExitFunc) :
		mSelf(selfRef),
		mOnEnter(onEnterFunc),
		mOnExit(onExitFunc)
	{
	}

	void OnEnter() override
	{
		mOnEnter(mSelf);
	}

	void OnExit() override
	{
		mOnExit(mSelf);
	}

private:
	luabridge::LuaRef mSelf;

	luabridge::LuaRef mOnEnter;
	luabridge::LuaRef mOnExit;
};

#endif // !_GAMESTATEWRAPPER_H_