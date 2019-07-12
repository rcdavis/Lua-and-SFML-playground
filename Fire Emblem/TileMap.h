///////////////////////////////////////////
/// \file TileMap.h
/// \brief Represents a tiled scene.
/// \author Ren Davis
///////////////////////////////////////////
#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include <vector>
#include <map>
#include <memory>

#include "Identifier.h"

class TileLayer;
class Camera;

namespace sf
{
	class RenderTarget;
}

class TileMap
{
public:
	TileMap();
	~TileMap();

	TileMap(const TileMap&) = delete;
	TileMap& operator=(const TileMap&) = delete;

	static std::shared_ptr<TileMap> GetTileMap(const char* file);

	void Render(sf::RenderTarget& rt, Camera& cam);

private:
	bool Load(const char* file);

	void Destroy();

	static std::map<Identifier, std::shared_ptr<TileMap> > mTileMaps;

	std::vector<std::shared_ptr<TileLayer> > mLayers;
	Identifier mId;
};

#endif // !_TILEMAP_H_