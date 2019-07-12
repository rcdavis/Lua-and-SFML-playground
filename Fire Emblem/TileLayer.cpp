
#include "TileLayer.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include "TileSet.h"
#include "Camera.h"

TileLayer::TileLayer()
{
}

TileLayer::~TileLayer()
{
	Destroy();
}

void TileLayer::Destroy()
{

}

void TileLayer::Render(sf::RenderTarget& rt, Camera& cam)
{
	sf::FloatRect camRect = cam.GetRect();

	const sf::Uint32 startRow = (sf::Uint32)std::max((sf::Int32)(camRect.top / mTileHeight), 0);
	const sf::Uint32 startCol = (sf::Uint32)std::max((sf::Int32)(camRect.left / mTileWidth), 0);
	const sf::Uint32 endRow = std::min(((sf::Uint32)(camRect.left + camRect.height) / mTileHeight) + 1, mRowCount);
	const sf::Uint32 endCol = std::min(((sf::Uint32)(camRect.left + camRect.width) / mTileWidth) + 1, mColCount);

	for (sf::Uint32 row = startRow; row < endRow; ++row)
	{
		for (sf::Uint32 col = startCol; col < endCol; ++col)
		{
			mTileSprite.setTextureRect(mTileSet->GetTerrain(mTiles[(row * mColCount) + col].mIndex).GetRect());
			mTileSprite.setPosition((col * mTileWidth) - camRect.left, (row * mTileHeight) - camRect.top);
			rt.draw(mTileSprite);
		}
	}
}