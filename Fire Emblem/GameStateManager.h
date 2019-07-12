
#ifndef _GAMESTATEMANAGER_H_
#define _GAMESTATEMANAGER_H_

#include <LuaBridge\RefCountedObject.h>

#include <vector>

class IGameState;
class IUpdateable;
class IRenderable;

namespace sf
{
	class RenderTarget;
}

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	GameStateManager(const GameStateManager&) = delete;
	GameStateManager& operator=(const GameStateManager&) = delete;

	void Set(RefCountedObjectPtr<IGameState> state);

	void Push(RefCountedObjectPtr<IGameState> state);

	void Pop();

	RefCountedObjectPtr<IGameState> Top();

	void ClearStates();

	void UpdateStates();
	void RenderStates(const sf::RenderTarget& rt);

private:
	std::vector<RefCountedObjectPtr<IGameState> > mStates;
	std::vector<IUpdateable*> mUpdateables;
	std::vector<IRenderable*> mRenderables;
};

#endif // !_GAMESTATEMANAGER_H_