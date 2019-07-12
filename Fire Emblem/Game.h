//////////////////////////////////
/// \file Game.h
/// \brief Core of the game.
/// \author Ren Davis
//////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_

#include <SFML\Graphics.hpp>
#include <SFML\Window\Event.hpp>

#include <lua.hpp>
#include <LuaBridge\LuaBridge.h>
#include <LuaBridge\RefCountedObject.h>

#include <vector>
#include <memory>
#include <map>

#include "Camera.h"
#include "Identifier.h"
#include "GameStateManager.h"

class TileMap;

class Game
{
	enum class StateChange
	{
		None,
		Push,
		Pop,
		Set
	};

public:
	Game();
	~Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void Init(int argc, char* argv[]);

	void Main();

private:
	void Update();
	void Render(float lerpBetweenFrame);

	void ProcessInput(const sf::String& str);
	bool IsValidInputText(sf::Uint32 unicode);

	void LoadConfig();

	bool LoadTileMap(const char* file);
	sf::Text* CreateText(const char* fontName, const char* text);

	void PushState(RefCountedObjectPtr<IGameState> state);
	void PopState();
	void SetState(RefCountedObjectPtr<IGameState> state);

	void CreateEntity();

	void SetText(const std::string& str);

	// Event response helper methods
	void OnClosedEvent();
	void OnKeyReleasedEvent(sf::Event::KeyEvent event);
	void OnTextEnteredEvent(sf::Event::TextEvent event);

	std::map<Identifier, std::unique_ptr<sf::Font> > mFonts;
	std::vector<std::unique_ptr<sf::Text> > mTexts;

	sf::RenderWindow mWindow;
	std::shared_ptr<TileMap> mTileMap;
	Camera mCam;

	GameStateManager mStateManager;
	RefCountedObjectPtr<IGameState> mChangingState;
	StateChange mStateChange;

	sf::Font mFont;
	sf::Text mTestText;
	sf::Text mFpsText;
	sf::Text mInputText;
	sf::String mInputString;
	lua_State* mLuaState;
	bool mTextInputActive;
	bool mShowFPS;

	bool mRunning;
};

#endif // !_GAME_H_