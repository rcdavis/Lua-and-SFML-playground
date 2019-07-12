
#include "Game.h"

#ifdef SFML_SYSTEM_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	define LogOutput(str) OutputDebugStringA(str)
#else
#	define LogOutput(str) printf(str)
#endif // SFML_SYSTEM_WINDOWS

#include <iostream>

#include "IGameState.h"
#include "GameStateWrapper.h"

#include "TileMap.h"
#include "Stats.h"
#include "Entity.h"

static void printLog(const char* message);
static RefCountedObjectPtr<IGameState> CreateGameState(luabridge::LuaRef selfRef,
	luabridge::LuaRef onEnterRef, luabridge::LuaRef onExitRef);

class TempObj
{
public:
	TempObj(lua_State* ls);

private:
	luabridge::LuaRef mRef;
};

Game::Game() :
	mFonts(),
	mTexts(),
	mWindow(),
	mTileMap(),
	mCam(),
	mStateManager(),
	mChangingState(),
	mStateChange(StateChange::None),
	mFont(),
	mTestText(),
	mFpsText(),
	mInputText(),
	mInputString(),
	mLuaState(luaL_newstate()),
	mTextInputActive(false),
	mShowFPS(false),
	mRunning(true)
{
	luaL_openlibs(mLuaState);
}

Game::~Game()
{
	mStateManager.ClearStates();

	mWindow.close();

	lua_close(mLuaState);
}

void Game::Init(int argc, char* argv[])
{
	luabridge::getGlobalNamespace(mLuaState)
		.addFunction("print", printLog)
		.beginClass<IGameState>("IGameState")
			.addFunction("OnEnter", &IGameState::OnEnter)
			.addFunction("OnExit", &IGameState::OnExit)
		.endClass()
		.deriveClass<GameStateWrapper, IGameState>("GameState")
			.addConstructor<void(*)(luabridge::LuaRef, luabridge::LuaRef, luabridge::LuaRef), RefCountedObjectPtr<GameStateWrapper> >()
		.endClass()
		.beginClass<Entity>("Entity")
			.addFunction("Say", &Entity::Say)
		.endClass()
		.beginClass<Game>("Game")
			.addData("showFps", &Game::mShowFPS, true)
			.addFunction("PushState", &Game::PushState)
			.addFunction("PopState", &Game::PopState)
			.addFunction("SetState", &Game::SetState)
			.addFunction("LoadTileMap", &Game::LoadTileMap)
			.addFunction("AddEntity", &Game::CreateEntity)
			.addFunction("SetText", &Game::SetText)
		.endClass()
		.beginClass<Stats>("Stats")
			.addConstructor<void(*)(void)>()
			.addProperty("name", &Stats::GetName, &Stats::SetName)
			.addProperty("level", &Stats::GetLevel, &Stats::SetLevel)
			.addProperty("curExp", &Stats::GetCurExp, &Stats::SetCurExp)
			.addProperty("nextLevelExp", &Stats::GetNextLevelExp, &Stats::SetNextLevelExp)
			.addProperty("hp", &Stats::GetCurHP, &Stats::SetCurHP)
			.addProperty("maxHp", &Stats::GetMaxHP, &Stats::SetMaxHP)
			.addProperty("strength", &Stats::GetStrength, &Stats::SetStrength)
			.addProperty("magic", &Stats::GetMagic, &Stats::SetMagic)
			.addProperty("skill", &Stats::GetSkill, &Stats::SetSkill)
			.addProperty("speed", &Stats::GetSpeed, &Stats::SetSpeed)
			.addProperty("luck", &Stats::GetLuck, &Stats::SetLuck)
			.addProperty("defense", &Stats::GetDefense, &Stats::SetDefense)
			.addProperty("resistance", &Stats::GetResistance, &Stats::SetResistance)
			.addFunction("ToString", &Stats::ToString)
		.endClass();

	luabridge::setGlobal(mLuaState, this, "game");

	LoadConfig();

	mFont.loadFromFile("Resources/Fonts/FreeSans.ttf");
	mTestText.setFont(mFont);
	mTestText.setCharacterSize(15);
	mTestText.setPosition(sf::Vector2f(0.0f, 100.0f));

	mFpsText.setFont(mFont);
	mFpsText.setCharacterSize(15);
	mFpsText.setString("FPS: 0");

	mInputText.setFont(mFont);
	mInputText.setCharacterSize(10);
	mInputText.setPosition(sf::Vector2f(0.0f, 50.0f));

	luaL_dofile(mLuaState, "Resources/Scripts/Test.lua");

	luabridge::LuaRef gameStateRef = luabridge::getGlobal(mLuaState, "Stats");
	if (gameStateRef.isTable())
	{
		printLog("gameStateRef is a table\n");
		std::stringstream ss;
		for (luabridge::Iterator iter(gameStateRef); !iter.isNil(); ++iter)
		{
			ss << *iter << std::endl;
		}
		printLog(ss.str().c_str());

		luabridge::LuaRef gsOnEnter = gameStateRef["OnEnter"];
		if (gsOnEnter.isFunction())
		{
			printLog("gsOnEnter is a function");
		}

		luabridge::LuaRef gsOnExit = gameStateRef["OnExit"];
		if (gsOnExit.isFunction())
		{
			printLog("gsOnExit is a function");
		}
	}

	luabridge::LuaRef testingStateRef = luabridge::getGlobal(mLuaState, "testingState");
	if (testingStateRef.isTable())
	{
		luabridge::LuaRef gsEnter = testingStateRef["OnEnter"];
		if (gsEnter.isFunction())
		{
			gsEnter(testingStateRef);
		}

		luabridge::LuaRef gsExit = testingStateRef["OnExit"];
		if (gsExit.isFunction())
		{
			gsExit(testingStateRef);
		}
	}
}

void Game::Main()
{
	sf::Event sfmlEvent;

	sf::Uint32 frames = 0;
	sf::Time fpsTimer;
	
	sf::Clock clock;
	sf::Time lag;
	const sf::Time timeStepPerFrame(sf::seconds(1.0f / 60.0f));

	while (mRunning)
	{
		while (mWindow.pollEvent(sfmlEvent))
		{
			switch (sfmlEvent.type)
			{
			case sf::Event::Closed:
				OnClosedEvent();
				break;

			case sf::Event::KeyReleased:
				OnKeyReleasedEvent(sfmlEvent.key);
				break;

			case sf::Event::TextEntered:
				OnTextEnteredEvent(sfmlEvent.text);
				break;
			}
		}

		sf::Time elapsedTime = clock.restart();
		lag += elapsedTime;

		while (lag >= timeStepPerFrame)
		{
			Update();
			lag -= timeStepPerFrame;
		}

		if (mShowFPS)
		{
			++frames;
			fpsTimer += elapsedTime;
			if (fpsTimer >= sf::milliseconds(1000))
			{
				char buffer[16] = {};
				sprintf_s(buffer, "FPS: %i", frames);
				mFpsText.setString(buffer);

				frames = 0;
				fpsTimer -= sf::milliseconds(1000);
			}
		}

		Render(lag / timeStepPerFrame);

		if (mStateChange != StateChange::None)
		{
			switch (mStateChange)
			{
			case StateChange::Push:
				mStateManager.Push(mChangingState);
				break;

			case StateChange::Pop:
				mStateManager.Pop();
				break;

			case StateChange::Set:
				mStateManager.Set(mChangingState);
				break;
			}

			mChangingState = nullptr;
			mStateChange = StateChange::None;
		}
	}
}

void Game::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		mCam.Move(sf::Vector2f(100.0f / 60.0f, 0.0f));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		mCam.Move(sf::Vector2f(-100.0f / 60.0f, 0.0f));
		if (mCam.GetRect().left <= 0.0f)
			mCam.MoveTo(sf::Vector2f(0.0f, mCam.GetRect().top));
	}

	mStateManager.UpdateStates();
}

void Game::Render(float lerpBetweenFrame)
{
	mWindow.clear(sf::Color::Magenta);

	if (mTileMap)
		mTileMap->Render(mWindow, mCam);

	if (mShowFPS)
		mWindow.draw(mFpsText);

	if (mTextInputActive)
		mWindow.draw(mInputText);

	mWindow.draw(mTestText);

	mStateManager.RenderStates(mWindow);

	mWindow.display();
}

void Game::ProcessInput(const sf::String& str)
{
	sf::String command = str;
	size_t spaceIndex = str.find(" ");
	if (spaceIndex != sf::String::InvalidPos)
		command = str.substring(0, spaceIndex);

	if (command == "LoadConfig")
	{
		LoadConfig();
	}
	else if (command == "script")
	{
		sf::String file = "Resources/Scripts/";
		file += str.substring(spaceIndex + 1);
		luaL_dofile(mLuaState, file.toAnsiString().c_str());
	}
	else if (command == "lua")
	{
		sf::String file = str.substring(spaceIndex + 1);
		luaL_dostring(mLuaState, file.toAnsiString().c_str());
	}
}

void Game::LoadConfig()
{
	sf::String windowTitle = "Game";
	int windowWidth = 320;
	int windowHeight = 240;
	bool fullscreen = false;
	bool resizable = true;
	bool vsync = true;

	luaL_dofile(mLuaState, "Resources/Scripts/Config.lua");

	luabridge::LuaRef app = luabridge::getGlobal(mLuaState, "app");
	if (app.isTable())
	{
		luabridge::LuaRef win = app["window"];
		if (win.isTable())
		{
			if (win["title"].isString())
				windowTitle = win["title"].cast<std::string>();

			if (win["width"].isNumber())
				windowWidth = win["width"].cast<int>();
			
			if (win["height"].isNumber())
				windowHeight = win["height"].cast<int>();
			
			if (!win["fullscreen"].isNil())
				fullscreen = win["fullscreen"].cast<bool>();
			
			if (!win["resizable"].isNil())
				resizable = win["resizable"].cast<bool>();
			
			if (!win["vsync"].isNil())
				vsync = win["vsync"].cast<bool>();

			std::stringstream ss;
			for (luabridge::Iterator iter(win); !iter.isNil(); ++iter)
			{
				ss << iter.key() << " : " << iter.value() << std::endl;
			}
			printLog(ss.str().c_str());
		}
	}

	sf::Uint32 windowStyle = sf::Style::Default;
	if (fullscreen)
		windowStyle = sf::Style::Fullscreen;
	else if (!resizable)
		windowStyle &= ~sf::Style::Resize;

	mWindow.create(sf::VideoMode(windowWidth, windowHeight), windowTitle, windowStyle);
	mWindow.setVerticalSyncEnabled(vsync);

	mCam.SetRect(sf::FloatRect(0.0f, 0.0f, (float)windowWidth, (float)windowHeight));
}

bool Game::IsValidInputText(sf::Uint32 unicode)
{
	return (unicode >= '0' && unicode <= '9') ||
		(unicode >= 'A' && unicode <= 'Z') ||
		(unicode >= 'a' && unicode <= 'z') ||
		unicode == ' ' || unicode == '.' ||
		unicode == '=' || unicode == '"' ||
		unicode == '(' || unicode == ')' ||
		unicode == '{' || unicode == '}' ||
		unicode == '[' || unicode == ']' ||
		unicode == ':';
}

bool Game::LoadTileMap(const char* file)
{
	mTileMap = TileMap::GetTileMap(file);
	return mTileMap != nullptr;
}

sf::Text* Game::CreateText(const char* fontName, const char* text)
{
	Identifier fontId(fontName);

	std::unique_ptr<sf::Text> t = std::make_unique<sf::Text>();
	sf::Font* f;

	auto fIter = mFonts.find(fontId);
	if (fIter != mFonts.end())
	{
		f = fIter->second.get();
	}
	else
	{
		std::unique_ptr<sf::Font> newFont = std::make_unique<sf::Font>();
		newFont->loadFromFile(fontName);
		f = newFont.get();
		mFonts[fontId] = std::move(newFont);
	}

	t->setFont(*f);
	t->setString(text);

	mTexts.push_back(std::move(t));

	return mTexts.back().get();
}

void Game::PushState(RefCountedObjectPtr<IGameState> state)
{
	mChangingState = state;
	mStateChange = StateChange::Push;
}

void Game::PopState()
{
	mChangingState = nullptr;
	mStateChange = StateChange::Pop;
}

void Game::SetState(RefCountedObjectPtr<IGameState> state)
{
	mChangingState = state;
	mStateChange = StateChange::Set;
}

void Game::CreateEntity()
{

}

void Game::SetText(const std::string& str)
{
	mTestText.setString(sf::String::fromUtf8(str.begin(), str.end()));
}

static void printLog(const char* message)
{
	LogOutput(message);
}

static RefCountedObjectPtr<IGameState> CreateGameState(luabridge::LuaRef selfRef,
	luabridge::LuaRef onEnterRef, luabridge::LuaRef onExitRef)
{
	return new GameStateWrapper(selfRef, onEnterRef, onExitRef);
}

void Game::OnClosedEvent()
{
	mRunning = false;
}

void Game::OnKeyReleasedEvent(sf::Event::KeyEvent event)
{
	switch (event.code)
	{
	case sf::Keyboard::Escape:
		mRunning = false;
		break;

	case sf::Keyboard::Tilde:
		if (event.shift)
		{
			mTextInputActive = !mTextInputActive;
			mInputString.clear();
			mInputText.setString(mInputString);
		}
		break;

	case sf::Keyboard::Return:
		if (mTextInputActive)
		{
			ProcessInput(mInputString);
			mInputString.clear();
			mInputText.setString(mInputString);
		}
		break;
	}
}

void Game::OnTextEnteredEvent(sf::Event::TextEvent event)
{
	if (mTextInputActive)
	{
		if (IsValidInputText(event.unicode))
		{
			mInputString += event.unicode;
			mInputText.setString(mInputString);
		}
		// Backspace was pressed. Erase characters.
		else if (event.unicode == 0x8 && !mInputString.isEmpty())
		{
			mInputString.erase(mInputString.getSize() - 1);
			mInputText.setString(mInputString);
		}
	}
}