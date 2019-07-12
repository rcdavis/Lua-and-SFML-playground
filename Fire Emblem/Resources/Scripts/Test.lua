print("Running Test.lua...\n")

--dofile("Resources/Scripts/MenuItem.lua")
--dofile("Resources/Scripts/MainMenu.lua")

game:LoadTileMap("Resources/Tile Maps/Test.tilemap")

local s = Stats()
s.name = "Test Stats"
s.level = 1
s.nextLevelExp = 25
s.hp = 20
s.maxHp = 20
s.strength = 5
s.magic = 5
s.skill = 5
s.speed = 5
s.luck = 5
s.defense = 5
s.resistance = 5

print(s:ToString().."\n")

game:SetText("Mi ŝatas kaj manĝas kuko")

--[[testingState = IGameState()
testingState.name = "testingState"
testingState.OnEnter = function(self)
	print("Calling "..self.name..":OnEnter...\n")
	game:LoadTileMap("Resources/Tile Maps/Test.tilemap")
end
testingState.OnExit = function(self)
	print("Calling "..self.name..":OnExit...\n")
end
--]]
testingState = {
	name = "testingState",
	OnEnter = function(self)
		print("Calling "..self.name..":OnEnter...\n")
		game:LoadTileMap("Resources/Tile Maps/Test.tilemap")
	end,

	OnExit = function(self)
		print("Calling "..self.name..":OnExit...\n")
	end
}


--game:PushState(testingState)