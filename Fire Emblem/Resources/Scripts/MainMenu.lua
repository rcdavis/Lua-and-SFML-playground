

class 'MainMenuState' (GameState)

--[[function MainMenuState:__init()
	GameState.__init(self)
	self.menuItems = {
		MenuItem("Play", function() print("Play selected...\n") end),
		MenuItem("Options", function() print("Options selected...\n") end),
		MenuItem("Exit", function() print("Exit selected...\n") end)
	}
end--]]

function MainMenuState:__init()
	GameState.__init(self, self.OnEnter, self.OnExit)
end

function MainMenuState:OnEnter()
	print("Calling MainMenuState:OnEnter...\n")
	game:LoadTileMap("Resources/Tile Maps/Test.tilemap")
end

function MainMenuState:OnExit()
	print("Calling MainMenuState:OnExit...\n")
end