
#include "TileSet.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

std::map<Identifier, std::shared_ptr<TileSet> > TileSet::mTileSets;

TileSet::TileSet()
{
}

TileSet::~TileSet()
{
}

std::shared_ptr<TileSet> TileSet::GetTileSet(const char* file)
{
	Identifier id(file);
	std::shared_ptr<TileSet> tileSet;
	auto iter = mTileSets.find(id);
	if (iter != mTileSets.end())
	{
		tileSet = iter->second;
	}
	else
	{
		tileSet = std::make_shared<TileSet>();
		tileSet->Load(file);
		mTileSets[id] = tileSet;
	}

	return tileSet;
}

bool TileSet::Load(const char* file)
{
	mId = file;

	rapidxml::file<> f(file);
	rapidxml::xml_document<> doc;
	doc.parse<0>(f.data());

	rapidxml::xml_node<>* root = doc.first_node();
	if (!root)
		return false;

	rapidxml::xml_node<>* textureEl = root->first_node();
	if (!mTexture.loadFromFile(textureEl->value()))
		return false;

	for (rapidxml::xml_node<>* terrainEl = textureEl->next_sibling(); terrainEl; terrainEl = terrainEl->next_sibling())
	{
		rapidxml::xml_node<>* rectEl = terrainEl->first_node();
		if (rectEl)
		{
			rapidxml::xml_attribute<>* x = rectEl->first_attribute();
			rapidxml::xml_attribute<>* y = x->next_attribute();
			rapidxml::xml_attribute<>* width = y->next_attribute();
			rapidxml::xml_attribute<>* height = width->next_attribute();

			sf::IntRect rect(std::stoi(x->value()), std::stoi(y->value()), std::stoi(width->value()), std::stoi(height->value()));
			mTerrains.push_back(Terrain(rect));
		}
	}

	return true;
}