#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Entity.h"
#include "MapChunkData.h"
#include "MapChunk.h"

namespace EcoSystem
{
	class ECO_SYSTEM_EXPORT Map : public Entity
	{
		friend class MapPainter;
	public:
		static Map* getInstance() { return s_instance; }
		static Log::LogObject& getLogger();

		Map(const sf::Vector2u &chunkCount, const std::string& name = "Map");
		Map(const Map& other) = delete;
		~Map();
		

		void generateMap();

		static void setGrassGrowthRate(float rate) { MapChunkData::setGrassGrowthRate(rate); }
		static float getGrassGrowthRate() { return MapChunkData::getGrassGrowthRate(); }

		static void setDirtFertilityRate(float rate) { MapChunkData::setDirtFertilityRate(rate); }
		static float getDirtFertilityRate() { return MapChunkData::getDirtFertilityRate(); }

		static MapChunkData::MapTile& getTile(const sf::Vector2f& pos);
		static MapChunkData::MapTile& getTile(const sf::Vector2i& pos);
		static MapChunkData::MapTile& getTile(float x, float y);
		static void setTile(const sf::Vector2f& pos, const MapChunkData::MapTile& tile);
		static void setTile(const sf::Vector2f& pos, MapTileType type);

		static bool isInMap(const sf::Vector2f& pos);
		static sf::Vector2f getNearestMapBorderDirection(const sf::Vector2f& pos);

		static sf::Vector2f getRandomPosition(float borderPadding = 32);

	private:
		void update() override;
		void setupPainter();

		sf::Vector2u m_chunkCount;


		QSFML::Utilities::ChunkManager* m_chunkManager;
		std::vector<MapChunkData*> m_chunkData;
		std::vector<MapChunk*> m_chunks;

		static const std::string s_mapTileTexturePath;
		static Map* s_instance;
	};
}