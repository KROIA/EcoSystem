#include "Simulator/Enviroment/Map/Map.h"
#include "SaveSystem/ApplicationSettings.h"
#include "FastNoiseLite.h"
#include "Simulator/Enviroment/Map/MapChunk.h"

namespace EcoSystem
{

	using namespace QSFML;
	using namespace QSFML::Utilities;

	// Folder name for the map tile textures
	const std::string Map::s_mapTileTexturePath = "MapTiles.png";

	Map* Map::s_instance = nullptr;
	Log::LogObject& Map::getLogger()
	{
		static Log::LogObject s_log(EcoSystem::Logger::getID(), "Map");
		return s_log;
	}

	Map::Map(const sf::Vector2u& chunkCount, const std::string& name)
		: Entity(name)
		, m_chunkCount(chunkCount)
	{
		s_instance = this;
		m_chunkManager = new ChunkManager(Assets::TextureManager::getTexture(s_mapTileTexturePath), sf::Vector2u(5, 5));
		m_chunkManager->setChunkFactory(Chunk::createFactory<MapChunk>());
		generateMap();
		setupPainter();


		/*
		// Prints the current tile which is under the mouse
		addUpdateFunction([this](GameObject& obj)
			{
				static sf::Vector2i lastTilePos;
				sf::Vector2i tilePos(obj.getMouseWorldPosition());
				static MapChunkData::MapTile* currentTile = &getTile(tilePos);
				if (lastTilePos != tilePos)
				{
					currentTile = &getTile(tilePos);
					lastTilePos = tilePos;
				}
				

				switch (currentTile->type)
				{
				case MapTileType::Dirt:
				{
					getLogger().logInfo("Dirt fertilized: " + std::to_string(currentTile->data.dirt.fertility));
					break;
				}
				case MapTileType::Grass:
				{
					getLogger().logInfo("Grass growth: " + std::to_string(currentTile->data.grass.growth));
					break;
				}
				case MapTileType::Rock:
				{
					getLogger().logInfo("Rock temperature: " + std::to_string(currentTile->data.rock.temperature));
					break;
				}
				case MapTileType::Sand:
				{
					getLogger().logInfo("Sand");
					break;
				}
				case MapTileType::Water:
				{
					getLogger().logInfo("Water depth: " + std::to_string(currentTile->data.water.depth));
					break;
				}

				}
			});*/
	}

	Map::~Map()
	{
		if (s_instance == this)
			s_instance = nullptr;
	}



	void Map::generateMap()
	{
		ES_GENERAL_PROFILING_FUNCTION(ES_COLOR_STAGE_1);
		logInfo("Map::generateMap()");
		sf::FloatRect mapBounds(0, 0, m_chunkCount.x * Chunk::CHUNK_SIZE-1, m_chunkCount.y * Chunk::CHUNK_SIZE-1);
		m_chunkManager->loadChunk(mapBounds);

		auto chunks = m_chunkManager->getChunks();
		m_chunkData.reserve(chunks.size());
		m_chunks.reserve(chunks.size());
		for (auto& chunk : chunks)
		{
			MapChunkData* chunkData = dynamic_cast<MapChunkData*>(chunk->getChunkData());
			if (!chunkData)
			{
				getLogger().logError("Failed to cast chunk data to MapChunkData");
				continue;
			}
			m_chunks.push_back(dynamic_cast<MapChunk*>(chunk));
			m_chunkData.push_back(chunkData);
		}
	}	
	MapChunkData::MapTile& Map::getTile(const sf::Vector2f& pos)
	{
		if (!isInMap(pos))
		{
			//getLogger().logWarning("Position is out of map bounds: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
			static MapChunkData::MapTile dummy;
			return dummy;
		}
		Chunk* chunk = s_instance->m_chunkManager->getChunk(pos);
		if (chunk)
		{
			// Get the chunks relative pos
			return dynamic_cast<MapChunkData*>(chunk->getChunkData())->getTile(pos);
		}
		getLogger().logError("Failed to get tile type at position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
		static MapChunkData::MapTile dummy;
		return dummy;
	}
	MapChunkData::MapTile& Map::getTile(const sf::Vector2i& pos)
	{
		sf::Vector2f posf = sf::Vector2f(pos);
		if (!isInMap(posf))
		{
			//getLogger().logWarning("Position is out of map bounds: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
			static MapChunkData::MapTile dummy;
			return dummy;
		}
		Chunk* chunk = s_instance->m_chunkManager->getChunk(posf);
		if (chunk)
		{
			// Get the chunks relative pos
			return dynamic_cast<MapChunkData*>(chunk->getChunkData())->getTile(posf);
		}
		getLogger().logError("Failed to get tile type at position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
		static MapChunkData::MapTile dummy;
		return dummy;
	}
	MapChunkData::MapTile& Map::getTile(float x, float y)
	{
		sf::Vector2f pos(x,y);
		if (!isInMap(pos))
		{
			//getLogger().logWarning("Position is out of map bounds: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
			static MapChunkData::MapTile dummy;
			return dummy;
		}
		Chunk* chunk = s_instance->m_chunkManager->getChunk(pos);
		if (chunk)
		{
			// Get the chunks relative pos
			return dynamic_cast<MapChunkData*>(chunk->getChunkData())->getTile(pos);
		}
		getLogger().logError("Failed to get tile type at position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
		static MapChunkData::MapTile dummy;
		return dummy;
	}
	void Map::setTile(const sf::Vector2f& pos, const MapChunkData::MapTile& tile)
	{
		if (!isInMap(pos))
		{
			//getLogger().logWarning("Position is out of map bounds: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
			return;
		}
		Chunk* chunk = s_instance->m_chunkManager->getChunk(pos);
		if (chunk)
		{
			// Get the chunks relative pos
			dynamic_cast<MapChunkData*>(chunk->getChunkData())->setTile(pos.x, pos.y, tile);
		}
		else
		{
			getLogger().logError("Failed to set tile at position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
		}
	}
	void Map::setTile(const sf::Vector2f& pos, MapTileType type)
	{
		if (!isInMap(pos))
		{
			//getLogger().logWarning("Position is out of map bounds: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
			return;
		}
		Chunk* chunk = s_instance->m_chunkManager->getChunk(pos);
		if (chunk)
		{
			// Get the chunks relative pos
			dynamic_cast<MapChunkData*>(chunk->getChunkData())->setTile(pos.x,pos.y, type);
		}
		else
		{
			getLogger().logError("Failed to set tile type at position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
		}
	}

	bool Map::isInMap(const sf::Vector2f& pos)
	{
		return !(
			pos.x < 0 || 
			pos.y < 0 || 
			pos.x >= s_instance->m_chunkCount.x * Chunk::CHUNK_SIZE || 
			pos.y >= s_instance->m_chunkCount.y * Chunk::CHUNK_SIZE);
	}
	sf::Vector2f Map::getNearestMapBorderDirection(const sf::Vector2f& pos)
	{
		sf::Vector2f dir;
		if (pos.x < 0)
			dir.x = -pos.x;
		else if (pos.x >= s_instance->m_chunkCount.x * Chunk::CHUNK_SIZE)
			dir.x = s_instance->m_chunkCount.x * Chunk::CHUNK_SIZE - pos.x;
		if (pos.y < 0)
			dir.y = -pos.y;
		else if (pos.y >= s_instance->m_chunkCount.y * Chunk::CHUNK_SIZE)
			dir.y = s_instance->m_chunkCount.y * Chunk::CHUNK_SIZE - pos.y;
		return dir;

	}
	sf::Vector2f Map::getRandomPosition(float borderPadding)
	{
		sf::Vector2f min(borderPadding, borderPadding);
		sf::Vector2f max(s_instance->m_chunkCount.x * Chunk::CHUNK_SIZE - borderPadding, s_instance->m_chunkCount.y * Chunk::CHUNK_SIZE - borderPadding);
		return QSFML::Utilities::RandomEngine::getVector(min, max);
	}
	
	void Map::update()
	{
		float deltaT = getDeltaT();
		float waterSpeed = 2.f;
		float waterAnimationTime = getAge() * waterSpeed;
		MapChunkData::setWaterTextureWaveIndex(static_cast<int>(waterAnimationTime) % 5);

		for (size_t i = 0; i < m_chunkData.size(); i++)
		{
			m_chunkData[i]->update(deltaT);
			m_chunks[i]->updateTextureCoords();
		}
	}

	void Map::setupPainter()
	{
		this->addDrawFunction([this](const Objects::GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
			{
				ES_UNUSED(obj);
				m_chunkManager->draw(target, states);
			});
	}
}