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
		


		static Log::LogObject& getLogger();

		Map(const sf::Vector2u &chunkCount, const std::string& name = "Map");
		Map(const Map& other) = delete;
		~Map();

		

		void generateMap();

		MapChunkData::MapTile& getTile(const sf::Vector2f& pos) const;
		MapChunkData::MapTile& getTile(const sf::Vector2i& pos) const;

		bool isInMap(const sf::Vector2f& pos) const;

	private:
		void update() override;
		void setupPainter();

		sf::Vector2u m_chunkCount;
		QSFML::Utilities::ChunkManager* m_chunkManager;
		std::vector<MapChunkData*> m_chunkData;
		std::vector<MapChunk*> m_chunks;

		static const std::string s_mapTileTexturePath;
	};
}