
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <lua.hpp>
#include <LuaBridge\LuaBridge.h>
#include <LuaBridge\RefCountedObject.h>

#include <string>

class Entity : public RefCountedObject
{
public:
	Entity(luabridge::LuaRef ref);

	void Say(const std::string& say);

private:
	luabridge::LuaRef mRef;
};

#endif // !_ENTITY_H_