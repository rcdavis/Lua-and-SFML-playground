
#include "GameStateManager.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include "IGameState.h"
#include "IUpdateable.h"
#include "IRenderable.h"

GameStateManager::GameStateManager() :
	mStates(),
	mUpdateables(),
	mRenderables()
{
}

GameStateManager::~GameStateManager()
{
	ClearStates();
}

void GameStateManager::Set(RefCountedObjectPtr<IGameState> state)
{
	ClearStates();

	Push(state);
}

void GameStateManager::Push(RefCountedObjectPtr<IGameState> state)
{
	mStates.push_back(state);

	IUpdateable* u = dynamic_cast<IUpdateable*>(state.getObject());
	if (u)
		mUpdateables.push_back(u);

	IRenderable* r = dynamic_cast<IRenderable*>(state.getObject());
	if (r)
		mRenderables.push_back(r);

	state->OnEnter();
}

void GameStateManager::Pop()
{
	auto state = Top();
	mStates.pop_back();

	IUpdateable* u = dynamic_cast<IUpdateable*>(state.getObject());
	if (u)
		mUpdateables.pop_back();

	IRenderable* r = dynamic_cast<IRenderable*>(state.getObject());
	if (r)
		mRenderables.pop_back();

	state->OnExit();
}

RefCountedObjectPtr<IGameState> GameStateManager::Top()
{
	return mStates.back();
}

void GameStateManager::ClearStates()
{
	mUpdateables.clear();
	mRenderables.clear();

	for (auto s : mStates)
	{
		s->OnExit();
	}
	mStates.clear();
}

void GameStateManager::UpdateStates()
{
	for (auto updateable : mUpdateables)
	{
		updateable->Update();
	}
}

void GameStateManager::RenderStates(const sf::RenderTarget& rt)
{
	for (auto renderable : mRenderables)
	{
		renderable->Render(rt);
	}
}