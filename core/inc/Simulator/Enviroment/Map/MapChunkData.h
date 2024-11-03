#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Entity.h"

namespace EcoSystem
{
	enum MapTileType
	{
		Grass,
		Dirt,
		Water,
		Sand,
		Rock,
		//Tree,

		Count
	};

	ECO_SYSTEM_EXPORT const char* toString(MapTileType type);

	class ECO_SYSTEM_EXPORT MapChunkData : public QSFML::Utilities::ChunkData
	{
	public:
		struct GrassTile
		{
			static constexpr unsigned int startTextureIndex = 0;
			float growth = 0.5;
		};
		struct DirtTile
		{
			static constexpr unsigned int startTextureIndex = 5;
			float fertility = 0.5;
		};
		struct WaterTile
		{
			static constexpr unsigned int startTextureIndex = 10;
			float depth = 1;
		};
		struct SandTile
		{
			static constexpr unsigned int startTextureIndex = 15;
		};
		struct RockTile
		{
			static constexpr unsigned int startTextureIndex = 20;
			float temperature = 20;
		};
		union MapTileData
		{
			GrassTile grass;
			DirtTile dirt;
			WaterTile water;
			SandTile sand;
			RockTile rock;
		};
		struct MapTile
		{
			MapTileType type;
			MapTileData data;

			MapTile()
				: type(MapTileType::Grass)
				, data{ GrassTile() }
			{
			}
		};

		static void setGrassGrowthRate(float rate) { m_grassGrowthRate = rate; }
		static float getGrassGrowthRate() { return m_grassGrowthRate; }
		
		static void setDirtFertilityRate(float rate) { m_dirtFertilityRate = rate; }
		static float getDirtFertilityRate() { return m_dirtFertilityRate; }
		
		MapChunkData();
		~MapChunkData();

		void update(float deltaT);

		void setTile(unsigned int x, unsigned int y, const MapTile& tile);
		void setTile(unsigned int x, unsigned int y, MapTileType tile);
		MapTile& getTile(unsigned int x, unsigned int y)
		{
			return m_tiles[y][x];
		}
		MapTile& getTile(const sf::Vector2f &worldPos)
		{
			const sf::Vector2i &chunkPos = getPosition();
			int x = static_cast<int>(worldPos.x - chunkPos.x) % CHUNK_SIZE;
			int y = static_cast<int>(worldPos.y - chunkPos.y) % CHUNK_SIZE;
			return m_tiles[y][x];
		}
		static void nextWaterTextureWaveIndex() { s_waterTextureWaveIndex = (s_waterTextureWaveIndex+1)%5; }
		static void setWaterTextureWaveIndex(int index) { s_waterTextureWaveIndex = index; }
	
	private:
		

		

		void updateBlock(unsigned int x, unsigned int y, float deltaT);

		MapTile m_tiles[CHUNK_SIZE][CHUNK_SIZE];
		static int s_waterTextureWaveIndex;
		static float m_grassGrowthRate;
		static float m_dirtFertilityRate;
	};
}