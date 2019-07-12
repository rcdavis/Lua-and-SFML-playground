
#include "TileMap.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include "TileLayer.h"
#include "TileSet.h"
#include "Camera.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

std::map<Identifier, std::shared_ptr<TileMap> > TileMap::mTileMaps;

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
	Destroy();
}

bool TileMap::Load(const char* file)
{
	mId = file;

	rapidxml::file<> f(file);
	rapidxml::xml_document<> doc;
	doc.parse<0>(f.data());

	rapidxml::xml_node<>* root = doc.first_node();
	if (!root)
		return false;

	for (rapidxml::xml_node<>* tlel = root->first_node(); tlel; tlel = tlel->next_sibling())
	{
		std::shared_ptr<TileLayer> layer = std::make_shared<TileLayer>();
		rapidxml::xml_node<>* tsel = tlel->first_node();
		layer->mTileSet = TileSet::GetTileSet(tsel->value());
		layer->mTileSprite.setTexture(layer->mTileSet->GetTexture());

		rapidxml::xml_node<>* rowCountEl = tsel->next_sibling();
		layer->mRowCount = (sf::Uint32)std::stoi(rowCountEl->value());

		rapidxml::xml_node<>* colCountEl = rowCountEl->next_sibling();
		layer->mColCount = (sf::Uint32)std::stoi(colCountEl->value());

		rapidxml::xml_node<>* tileWidthEl = colCountEl->next_sibling();
		layer->mTileWidth = (sf::Uint32)std::stoi(tileWidthEl->value());

		rapidxml::xml_node<>* tileHeightEl = tileWidthEl->next_sibling();
		layer->mTileHeight = (sf::Uint32)std::stoi(tileHeightEl->value());

		layer->mTiles.reserve(layer->mRowCount * layer->mColCount);
		rapidxml::xml_node<>* tilesEl = tileHeightEl->next_sibling();
		for (rapidxml::xml_node<>* rowEl = tilesEl->first_node(); rowEl; rowEl = rowEl->next_sibling())
		{
			for (rapidxml::xml_node<>* colEl = rowEl->first_node(); colEl; colEl = colEl->next_sibling())
			{
				layer->mTiles.push_back(TileLayer::Tile((sf::Uint32)std::stoi(colEl->value())));
			}
		}

		mLayers.push_back(layer);
	}

	return true;
}

void TileMap::Destroy()
{

}

std::shared_ptr<TileMap> TileMap::GetTileMap(const char* file)
{
	Identifier id(file);
	std::shared_ptr<TileMap> tileMap;
	auto iter = mTileMaps.find(id);
	if (iter != mTileMaps.end())
	{
		tileMap = iter->second;
	}
	else
	{
		tileMap = std::make_shared<TileMap>();
		tileMap->Load(file);
		mTileMaps[id] = tileMap;
	}

	return tileMap;
}

void TileMap::Render(sf::RenderTarget& rt, Camera& cam)
{
	for (auto layer : mLayers)
	{
		layer->Render(rt, cam);
	}
}