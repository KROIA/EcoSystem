#include "Simulator/Enviroment/Map/MapChunk.h"
#include "Simulator/Enviroment/Map/Map.h"
#include "Simulator/Enviroment/Map/MapChunkData.h"


namespace EcoSystem
{
	MapChunk::MapChunk(const sf::Vector2i& pos, const Resources& resources, float scale)
		: Chunk(pos, resources, scale)
	{
	}

	void MapChunk::onGenerate()
	{
		MapChunkData *chunkData = new MapChunkData();
		m_chunkData = chunkData;
		const sf::Vector2i& m_position = getPosition();

		//static Utilities::PerlinNoise noise;
		static FastNoiseLite noise1;
		noise1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise1.SetFrequency(0.005);
		noise1.SetFractalOctaves(10);
		noise1.SetFractalGain(1);
		noise1.SetFractalWeightedStrength(0.5);
		noise1.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
		noise1.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance);
		noise1.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
		noise1.SetSeed(1234);

		static FastNoiseLite noise2;
		noise2.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise2.SetFractalOctaves(1);
		noise2.SetFractalWeightedStrength(0.0);
		noise2.SetFrequency(0.005);
		noise2.SetFractalType(FastNoiseLite::FractalType::FractalType_Ridged);

		float scale = 4;
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_SIZE; y++)
			{
				sf::Vector2f rawPos((float)x + m_position.x, (float)y + m_position.y);
				sf::Vector2f pos = rawPos * scale;
				float noise2Val = noise2.GetNoise((float)rawPos.x, (float)rawPos.y);
				float noise1Val = noise1.GetNoise(pos.x, pos.y) + 0.1;
				float value = noise1Val * 10;

				float riverNoise = (0.92 - noise2Val) * 2;

				MapTileType tileType = MapTileType::Dirt;
				float riverRange = 0.2;
				if (riverNoise < riverRange)
				{
					if (riverNoise > 0)
					{
						tileType = MapTileType::Sand;
					}
					else
					{
						tileType = MapTileType::Water;
					}
				}
				else
				{

					if (value < 0.1f)
						tileType = MapTileType::Water;
					else if (value < 0.4f)
						tileType = MapTileType::Sand;
					else if (value < 0.6f)
						tileType = MapTileType::Dirt;
					else if (value < 0.7f)
						tileType = MapTileType::Rock;

				}
				
				chunkData->setTile(x, y, tileType);
			}
		}
	}

	
}