////////////////////////////////////////////////
/// \file TileLayer.h
/// \brief Represents a layer in the TileMap.
/// \author Ren Davis
////////////////////////////////////////////////
#ifndef _TILELAYER_H_
#define _TILELAYER_H_

#include <SFML\Config.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include <vector>
#include <memory>

class TileSet;
class Camera;

namespace sf
{
	class RenderTarget;
}

class TileLayer
{
	friend class TileMap;

	struct Tile
	{
		sf::Uint32 mIndex;

		Tile() : mIndex(0) {}
		Tile(sf::Uint32 index) : mIndex(index) {}
	};

public:
	TileLayer();
	~TileLayer();

	TileLayer(const TileLayer&) = delete;
	TileLayer& operator=(const TileLayer&) = delete;

	void Render(sf::RenderTarget& rt, Camera& cam);

private:
	void Destroy();

	sf::Sprite mTileSprite;

	std::vector<Tile> mTiles;
	std::shared_ptr<TileSet> mTileSet;

	sf::Uint32 mRowCount = 0;
	sf::Uint32 mColCount = 0;

	sf::Uint32 mTileWidth = 0;
	sf::Uint32 mTileHeight = 0;
};

#endif // !_TILELAYER_H_