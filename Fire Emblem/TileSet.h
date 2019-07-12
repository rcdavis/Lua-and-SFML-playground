/////////////////////////////////////////////////////
/// \file TileSet.h
/// \brief Represents a tile set in the TileLayer.
/// \author Ren Davis
/////////////////////////////////////////////////////
#ifndef _TILESET_H_
#define _TILESET_H_

#include <SFML\Graphics\Texture.hpp>

#include <vector>
#include <map>
#include <memory>

#include "Identifier.h"

class TileSet
{
public:
	class Terrain
	{
	private:
		sf::IntRect mRect;

	public:
		Terrain(sf::IntRect rect) : mRect(rect) {}

		sf::IntRect GetRect() const { return mRect; }
	};

	TileSet();
	~TileSet();

	TileSet(const TileSet&) = delete;
	TileSet& operator=(const TileSet&) = delete;

	static std::shared_ptr<TileSet> GetTileSet(const char* file);

	const Terrain& GetTerrain(sf::Uint32 index) const { return mTerrains[index]; }
	const sf::Texture& GetTexture() const { return mTexture; }

private:
	static std::map<Identifier, std::shared_ptr<TileSet> > mTileSets;

	bool Load(const char* file);

	std::vector<Terrain> mTerrains;
	sf::Texture mTexture;
	Identifier mId;
};

#endif // !_TILESET_H_