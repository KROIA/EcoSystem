#include "Simulator/Enviroment/Map/MapChunkData.h"

namespace EcoSystem
{
	const char* toString(MapTileType type)
	{
		switch (type)
		{
		case MapTileType::Grass: return "Grass";
		case MapTileType::Dirt: return "Dirt";
		case MapTileType::Water: return "Water";
		case MapTileType::Sand: return "Sand";
		case MapTileType::Rock: return "Rock";
			//case MapTileType::Tree: return "Tree";
		default: return "Unknown";
		}
	}


	int MapChunkData::s_waterTextureWaveIndex = 0;

	MapChunkData::MapChunkData()
		: QSFML::Utilities::ChunkData()
	{

	}
	MapChunkData::~MapChunkData()
	{

	}

	void MapChunkData::update(float deltaT)
	{
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				updateBlock(x,y, deltaT);
			}
		}
	}
	void MapChunkData::setTile(unsigned int x, unsigned int y, const MapTile& tile)
	{
		m_tiles[y][x] = tile;
		switch (tile.type)
		{
		case MapTileType::Grass:
			updateBlock(x, y, 0);
			break;
		case MapTileType::Dirt:
			setTextureIndex(x, y, DirtTile::startTextureIndex);
			break;
		case MapTileType::Water:
			setTextureIndex(x, y, WaterTile::startTextureIndex + s_waterTextureWaveIndex);
			break;
		case MapTileType::Sand:
			setTextureIndex(x, y, SandTile::startTextureIndex);
			break;
		case MapTileType::Rock:
			setTextureIndex(x, y, RockTile::startTextureIndex);
			break;
		}
	}
	void MapChunkData::setTile(unsigned int x, unsigned int y, MapTileType tile)
	{
		MapChunkData::MapTile newTile;
		newTile.type = tile; 
		switch (newTile.type)
		{
		case MapTileType::Grass:
			newTile.data.grass = MapChunkData::GrassTile();
			break;
		case MapTileType::Dirt:
			newTile.data.dirt = MapChunkData::DirtTile();
			break;
		case MapTileType::Water:
			newTile.data.water = MapChunkData::WaterTile();
			break;
		case MapTileType::Sand:
			newTile.data.sand = MapChunkData::SandTile();
			break;
		case MapTileType::Rock:
			newTile.data.rock = MapChunkData::RockTile();
			break;
		}
		setTile(x, y, newTile);
	}
	void MapChunkData::updateBlock(unsigned int x, unsigned int y, float deltaT)
	{
		MapTile& tile = m_tiles[y][x];
		switch (tile.type)
		{
		case MapTileType::Grass:
		{
			float growth = tile.data.grass.growth;
			if (growth < 1.0f)
			{
				growth += deltaT * float(rand()%100)/100.f * 0.1;
				if (growth > 1.0f) [[unlikely]]
				{
					growth = 1.0f;
				}

				tile.data.grass.growth = growth;
				int texSubIndex = growth * 4.9f; // 0 - 4

				setTextureIndex(x, y, GrassTile::startTextureIndex + (unsigned int)(texSubIndex));
			}
			break;
		}
		case MapTileType::Dirt:
		{
			float fertilization = tile.data.dirt.fertility;
			if (fertilization < 1.0f)
			{
				fertilization += deltaT * float(rand() % 100) / 100.f * 0.1;
				if (fertilization >= 1.0f) [[unlikely]]
				{
					MapTile grassTile;
					grassTile.type = MapTileType::Grass;
					setTile(x, y, grassTile);
					return;
				}
				tile.data.dirt.fertility = fertilization;
			}
			break;
		}
		case MapTileType::Water:
		{
			setTextureIndex(x, y, WaterTile::startTextureIndex + s_waterTextureWaveIndex);
			break;
		}
		case MapTileType::Sand:
			break;
		case MapTileType::Rock:
			break;
		}
	}
}