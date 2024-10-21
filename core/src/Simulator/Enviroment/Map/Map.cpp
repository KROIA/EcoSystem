#include "Simulator/Enviroment/Map/Map.h"
#include "SaveSystem/ApplicationSettings.h"

namespace EcoSystem
{
	// Folder name for the map tile textures
	const std::string Map::s_mapTileTexturePath = "MapTile";

	Log::LogObject& Map::getLogger()
	{
		static Log::LogObject s_log(EcoSystem::Logger::getID(), "Map");
		return s_log;
	}
	Map::Map(const sf::Vector2u& dim, const sf::Vector2f& tileSize, const std::string& name)
		: Entity(name)
		, m_dim(dim)
		, m_tileSize(tileSize)
		, m_wrapAround(false)
	{
		m_tiles.resize(dim.x * dim.y);
		for (auto& tile : m_tiles)
		{
			tile.type = MapTileType::Grass;
		}

		m_painter = new MapPainter(*this);
		add(m_painter);

		generateMap();
	}

	Map::Map(const Map& other)
		: Entity(other)
		, m_dim(other.m_dim)
		, m_tileSize(other.m_tileSize)
		, m_wrapAround(other.m_wrapAround)
		, m_tiles(other.m_tiles)
	{
		

		m_painter = new MapPainter(*this);
		add(m_painter);
	}

	Map::~Map()
	{
	}

	OBJECT_IMPL(Map);

	void Map::generateMap()
	{
		ES_GENERAL_PROFILING_FUNCTION(ES_COLOR_STAGE_1);
		logInfo("Map::generateMap()");
		QSFML::Utilities::PerlinNoise perlinNoise(0);

		float scale = 50;
		static constexpr int resolutionCount = 20;
		static constexpr bool useThread = true;

		if (useThread)
		{
			size_t threadCount = sqrt(std::thread::hardware_concurrency());
			sf::Vector2u chunkSize(m_dim.x / threadCount, m_dim.y / threadCount);

			std::vector<std::thread> threads;
			for (size_t y = 0; y < threadCount; y++)
			{
				for (size_t x = 0; x < threadCount; x++)
				{
					threads.push_back(std::thread([this, x, y, chunkSize, scale, &perlinNoise]()
						{
							for (size_t yy = 0; yy < chunkSize.y; yy++)
							{
								for (size_t xx = 0; xx < chunkSize.x; xx++)
								{
									float value = (perlinNoise.noise((x * chunkSize.x + xx) * scale, (y * chunkSize.y + yy) * scale, resolutionCount, m_dim) + 0.5);
									
									MapTileType type = MapTileType::Grass;
									if (value < 0.2f)
										type = MapTileType::Water;
									else if (value < 0.4f)
										type = MapTileType::Sand;
									else if (value < 0.6f)
										type = MapTileType::Dirt;
									else if (value < 0.7f)
										type = MapTileType::Rock;
									if (value < -10)
									{
										int X = 0;
										X;
										//getLogger().logError("Map::generateMap(): value out of bounds: " + std::to_string(value));
									}
									getTile(sf::Vector2u( x * chunkSize.x + xx, y * chunkSize.y + yy )).type = type;
								}
							}
						}));
				}
			}

			for (auto& thread : threads)
			{
				thread.join();
			}


		}
		else
		{
			for (unsigned int y = 0; y < m_dim.y; y++)
			{
				for (unsigned int x = 0; x < m_dim.x; x++)
				{
					float value = (perlinNoise.noise(x * scale, y * scale, resolutionCount, m_dim) + 0.5);
					MapTileType type = MapTileType::Grass;
					if (value < 0.2f)
						type = MapTileType::Water;
					else if (value < 0.4f)
						type = MapTileType::Sand;
					else if (value < 0.6f)
						type = MapTileType::Dirt;
					else if (value < 0.7f)
						type = MapTileType::Rock;
					getTile({ x, y }).type = type;
				}
			}
		}
		m_painter->updateMap();
	}

	bool Map::indexValid(const sf::Vector2u& pos) const
	{
		if(m_wrapAround)
			return true;
		return pos.x < m_dim.x && pos.y < m_dim.y;
	}
	Map::MapTile& Map::getTile(const sf::Vector2u& pos)
	{
		if (pos.x < m_dim.x && pos.y < m_dim.y)
			return m_tiles[pos.y * m_dim.x + pos.x];

		if (!m_wrapAround)
		{
			getLogger().logError("Map::getTile(): Index out of bounds: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
			
			static MapTile emptyTile;
			return emptyTile;
		}
		// Wrap around
		sf::Vector2u newPos(pos);
		newPos.x = newPos.x % m_dim.x;
		newPos.y = newPos.y % m_dim.y;
		return m_tiles[newPos.y * m_dim.x + newPos.x];
	}


	

	Map::MapPainter::MapPainter(Map& map)
		: QSFML::Components::Drawable("MapPainter")
		, m_map(map)
		//, m_texture(QSFML::Assets::TextureManager::getTexture("MapTiles.png"))
	{
		m_texture[0] = &QSFML::Assets::TextureManager::getTexture("MapTiles0.png");
		m_texture[1] = &QSFML::Assets::TextureManager::getTexture("MapTiles1.png");
		m_texture[2] = &QSFML::Assets::TextureManager::getTexture("MapTiles2.png");
		m_texture[3] = &QSFML::Assets::TextureManager::getTexture("MapTiles3.png");

		

		sf::Vector2u textureSize = m_texture[0]->getSize();
		sf::Vector2u tileSize(textureSize.x / static_cast<size_t>(MapTileType::Count), textureSize.y);
		
		float padding = 0.05;
		for (size_t i = 0; i < static_cast<size_t>(MapTileType::Count); i++)
		{
			m_textCoords[i] = VertexPos(i * tileSize.x, 0.f , tileSize.x, tileSize.y, padding);
		}
	}

	Map::MapPainter::~MapPainter()
	{
	}

	//COMPONENT_IMPL(Map::MapPainter);

	void Map::MapPainter::updateMap()
	{
		ES_GENERAL_PROFILING_FUNCTION(ES_COLOR_STAGE_2);
		logInfo("Map::MapPainter::updateMap()");

		//m_vertices.setPrimitiveType(sf::Quads);
		//m_vertices.resize(m_map.m_dim.x * m_map.m_dim.y * 4);

		if (m_chunks.size() == 0)
		{
			m_chunkDim = sf::Vector2u(m_map.m_dim.x / chunkSize + 1, m_map.m_dim.y / chunkSize + 1);
			m_chunks.resize(m_chunkDim.x * m_chunkDim.y);
		}

		if (m_previewChunks.size() == 0)
		{
			m_previewChunkDim = sf::Vector2u(m_chunkDim.x / chunkSize + 1, m_chunkDim.y / chunkSize + 1);
			m_previewChunks.resize(m_previewChunkDim.x * m_previewChunkDim.y);
		}

		sf::Color typeAvarageColor[static_cast<size_t>(MapTileType::Count)];
		for (int i = 0; i < static_cast<size_t>(MapTileType::Count); i++)
		{
			typeAvarageColor[i] = m_texture[resolutionCount-1]->copyToImage().getPixel(i * 16+8, 8);
		}

		for (size_t x = 0; x < m_chunkDim.x; ++x)
		{
			for (size_t y = 0; y < m_chunkDim.y; ++y)
			{
				Chunk*& chunk = getChunk(sf::Vector2u(x, y));
				if (!chunk)
				{
					chunk = new Chunk();
					chunk->vertices.clear();
					chunk->vertices.setPrimitiveType(sf::Quads);
					chunk->vertices.resize(chunkSize * chunkSize * 4);
					chunk->pos = sf::Vector2f(x * chunkSize * m_map.m_tileSize.x, y * chunkSize * m_map.m_tileSize.y);
				}

				// Calculate average color
				int typeCount[static_cast<size_t>(MapTileType::Count)];
				memset(typeCount, 0, sizeof(typeCount));
				for (size_t xx = 0; xx < chunkSize; ++xx)
				{
					for (size_t yy = 0; yy < chunkSize; ++yy)
					{
						sf::Vector2u mapCoord(x * chunkSize + xx, y * chunkSize + yy);
						if (mapCoord.x >= m_map.m_dim.x || mapCoord.y >= m_map.m_dim.y)
							continue;
						const MapTile& tile = m_map.getTile(mapCoord);
						size_t vIndex = (yy * chunkSize + xx) * 4;
						sf::Vertex* quad = &chunk->vertices[vIndex];
						//sf::Vertex* quad = &m_vertices[(x + y * m_map.m_dim.x) * 4];
						VertexPos& texCoords = m_textCoords[static_cast<size_t>(tile.type)];
						typeCount[static_cast<size_t>(tile.type)]++;

						quad[0].position = sf::Vector2f(xx * m_map.m_tileSize.x, yy * m_map.m_tileSize.y);
						quad[1].position = sf::Vector2f((xx + 1) * m_map.m_tileSize.x, yy * m_map.m_tileSize.y);
						quad[2].position = sf::Vector2f((xx + 1) * m_map.m_tileSize.x, (yy + 1) * m_map.m_tileSize.y);
						quad[3].position = sf::Vector2f(xx * m_map.m_tileSize.x, (yy + 1) * m_map.m_tileSize.y);

						quad[0].texCoords = texCoords.TL;
						quad[1].texCoords = texCoords.TR;
						quad[2].texCoords = texCoords.BR;
						quad[3].texCoords = texCoords.BL;
					}
				}

				int maxCount = 0;
				int maxIndex = 0;
				for (int i = 0; i < static_cast<size_t>(MapTileType::Count); i++)
				{
					if (typeCount[i] > maxCount)
					{
						maxCount = typeCount[i];
						maxIndex = i;
						//break;
					}
				}
				chunk->averageColor = typeAvarageColor[maxIndex];

				

			}
		}

		for (size_t x = 0; x < m_previewChunkDim.x; ++x)
		{
			for (size_t y = 0; y < m_previewChunkDim.y; ++y)
			{
				// fill previewChunk
				Chunk*& previewChunk = getPreviewChunk(sf::Vector2u(x, y));
				if (!previewChunk)
				{
					previewChunk = new Chunk();
					previewChunk->vertices.clear();
					previewChunk->vertices.setPrimitiveType(sf::Quads);
					previewChunk->vertices.resize(chunkSize * chunkSize * 4);
					previewChunk->pos = sf::Vector2f(x * chunkSize * chunkSize * m_map.m_tileSize.x, y * chunkSize * chunkSize * m_map.m_tileSize.y);
				}

				for (size_t xx = 0; xx < chunkSize; ++xx)
				{
					for (size_t yy = 0; yy < chunkSize; ++yy)
					{
						sf::Vector2u mapCoord(x * chunkSize + xx, y * chunkSize + yy);
						sf::Color col = sf::Color::Transparent;
						if (mapCoord.x < m_chunkDim.x && mapCoord.y < m_chunkDim.y)
						{
							Chunk*& chunk = getChunk(mapCoord);
							col = chunk->averageColor;
						}
						size_t vIndex = (yy * chunkSize + xx) * 4;
						
						sf::Vertex* quad = &previewChunk->vertices[vIndex];
						//sf::Vertex* quad = &m_vertices[(x + y * m_map.m_dim.x) * 4];
						

						quad[0].position = sf::Vector2f(xx * m_map.m_tileSize.x * chunkSize, yy * m_map.m_tileSize.y * chunkSize);
						quad[1].position = sf::Vector2f((xx + 1) * m_map.m_tileSize.x * chunkSize, yy * m_map.m_tileSize.y * chunkSize);
						quad[2].position = sf::Vector2f((xx + 1) * m_map.m_tileSize.x * chunkSize, (yy + 1) * m_map.m_tileSize.y * chunkSize);
						quad[3].position = sf::Vector2f(xx * m_map.m_tileSize.x * chunkSize, (yy + 1) * m_map.m_tileSize.y * chunkSize);

						quad[0].color = col;
						quad[1].color = col;
						quad[2].color = col;
						quad[3].color = col;
					}
				}
			}
		}



		/*
		for (unsigned int y = 0; y < m_map.m_dim.y; y++)
		{
			for (unsigned int x = 0; x < m_map.m_dim.x; x++)
			{
				const MapTile& tile = m_map.getTile({ x, y });
				sf::Vector2u chunkVertexPos(x % chunkSize, y % chunkSize);

				Chunk*& chunk = getChunk(sf::Vector2u( x / chunkSize, y / chunkSize ));
				if (!chunk)
				{
					chunk = new Chunk();
					chunk->vertices.setPrimitiveType(sf::Quads);
					chunk->vertices.resize(chunkSize * chunkSize * 4);
					chunk->pos = sf::Vector2f(x / chunkSize * chunkSize * m_map.m_tileSize.x, y / chunkSize * chunkSize * m_map.m_tileSize.y);
				}
				size_t vIndex = (chunkVertexPos.y * chunkSize + chunkVertexPos.x) * 4;
				sf::Vertex* quad = &chunk->vertices[vIndex];
				//sf::Vertex* quad = &m_vertices[(x + y * m_map.m_dim.x) * 4];
				VertexPos& texCoords = m_textCoords[static_cast<size_t>(tile.type)];

				quad[0].position = sf::Vector2f(chunkVertexPos.x * m_map.m_tileSize.x, chunkVertexPos.y * m_map.m_tileSize.y);
				quad[1].position = sf::Vector2f((chunkVertexPos.x + 1) * m_map.m_tileSize.x, chunkVertexPos.y * m_map.m_tileSize.y);
				quad[2].position = sf::Vector2f((chunkVertexPos.x + 1) * m_map.m_tileSize.x, (chunkVertexPos.y + 1) * m_map.m_tileSize.y);
				quad[3].position = sf::Vector2f(chunkVertexPos.x * m_map.m_tileSize.x, (chunkVertexPos.y + 1) * m_map.m_tileSize.y);

				quad[0].texCoords = texCoords.TL;
				quad[1].texCoords = texCoords.TR;
				quad[2].texCoords = texCoords.BR;
				quad[3].texCoords = texCoords.BL;
			}	
		}*/
	}
	void Map::MapPainter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		bool hasChanged = false;
		const sf::View view = target.getView();
		// Get zoom factor to switch between high and low res textures
		float zoom = view.getSize().x / view.getViewport().width;
		sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

		if (m_lastZoom != zoom || m_lastViewCenter != view.getCenter())
		{
			m_lastZoom = zoom;
			m_lastViewCenter = view.getCenter();
			hasChanged = true;
		}

		// Set the texture based on the zoom factor

		// select one of the 4 textures based on the zoom factor
		int textureIndex = 0;
		float zoomThreshold = 30 * m_map.m_tileSize.x;
		if (zoom > zoomThreshold)
			textureIndex = 1;
		if (zoom > zoomThreshold * 2)
			textureIndex = 2;
		if (zoom > zoomThreshold * 4)
			textureIndex = 3;
		if (zoom > zoomThreshold * 8)
			textureIndex = 4;

		

		// Draw chunks if visible
		/*sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
		sf::Vector2u chunkSize(m_map.m_tileSize.x * chunkSize, m_map.m_tileSize.y * chunkSize);
		sf::Vector2u chunkPos = sf::Vector2u(viewRect.left / chunkSize.x, viewRect.top / chunkSize.y);
		sf::Vector2u chunkCount = sf::Vector2u(viewRect.width / chunkSize.x, viewRect.height / chunkSize.y);
		for (unsigned int y = 0; y < chunkCount.y; y++)
		{
			for (unsigned int x = 0; x < chunkCount.x; x++)
			{
				size_t index = chunkPos.y / chunkSize.x * m_map.m_dim.x + chunkPos.x;
				const auto it = m_chunks.find(index);
				if (it == m_chunks.end())
					continue;
				const Chunk* chunk = m_chunks.at(index);
				if (chunk)
					chunk->draw(target, states, false);
				chunkPos.x++;
			}
			chunkPos.x = chunkPos.x % m_map.m_dim.x;
			chunkPos.y++;
		}*/

		//for (const auto& chunk : m_chunks)
		//{
		//	chunk.second->draw(target, states, true);
		//}
		//hasChanged = true;

		if (hasChanged)
		{
			size_t lastCap = m_visibleChunks.size();
			m_visibleChunks.clear();
			m_visibleChunks.reserve(lastCap * 2);

			

			if (textureIndex < resolutionCount)
			{
				sf::Vector2i fromChunkPos(viewRect.left / (m_map.m_tileSize.x * chunkSize), viewRect.top / (m_map.m_tileSize.y * chunkSize));
				sf::Vector2i toChunkPos(1 + (viewRect.left + viewRect.width) / (m_map.m_tileSize.x * chunkSize), 1 + (viewRect.top + viewRect.height) / (m_map.m_tileSize.y * chunkSize));
				states.texture = m_texture[textureIndex];
				if (fromChunkPos.x < 0)
					fromChunkPos.x = 0;
				if (fromChunkPos.y < 0)
					fromChunkPos.y = 0;
				if (toChunkPos.x >= (int)m_chunkDim.x)
					toChunkPos.x = (int)m_chunkDim.x;
				if (toChunkPos.y >= (int)m_chunkDim.y)
					toChunkPos.y = (int)m_chunkDim.y;
				for (int x = fromChunkPos.x; x < toChunkPos.x; ++x)
				{
					for (int y = fromChunkPos.y; y < toChunkPos.y; ++y)
					{
						size_t index = y * m_chunkDim.x + x;
						if (index >= m_chunks.size())
						{
							logWarning("Index out of bounds: " + std::to_string(index));
							continue;
						}
						Chunk* chunk = m_chunks[index];
						if (chunk)
						{
							m_visibleChunks.push_back(chunk);
							chunk->draw(target, states, true);
						}
					}
				}
			}
			else
			{
				sf::Vector2i fromChunkPos(viewRect.left / (m_map.m_tileSize.x * chunkSize* chunkSize), viewRect.top / (m_map.m_tileSize.y * chunkSize* chunkSize));
				sf::Vector2i toChunkPos(1 + (viewRect.left + viewRect.width) / (m_map.m_tileSize.x * chunkSize* chunkSize), 1 + (viewRect.top + viewRect.height) / (m_map.m_tileSize.y * chunkSize* chunkSize));
				states.texture = nullptr;
				if (fromChunkPos.x < 0)
					fromChunkPos.x = 0;
				if (fromChunkPos.y < 0)
					fromChunkPos.y = 0;
				if (toChunkPos.x >= (int)m_previewChunkDim.x)
					toChunkPos.x = (int)m_previewChunkDim.x;
				if (toChunkPos.y >= (int)m_previewChunkDim.y)
					toChunkPos.y = (int)m_previewChunkDim.y;
				for (int x = fromChunkPos.x; x < toChunkPos.x; ++x)
				{
					for (int y = fromChunkPos.y; y < toChunkPos.y; ++y)
					{
						size_t index = y * m_previewChunkDim.x + x;
						if (index >= m_previewChunks.size())
						{
							logWarning("Index out of bounds: " + std::to_string(index));
							continue;
						}
						Chunk* chunk = m_previewChunks[index];
						if (chunk)
						{
							m_visibleChunks.push_back(chunk);
							chunk->draw(target, states, true);
						}
					}
				}
			}

			//static size_t index = 0;
			//index++;
			//if (index >= m_chunks.size())
			//	index = 0;
			//
			//m_chunks[index]->draw(target, states, true);

			/*size_t lastCap = m_visibleChunks.size();
			m_visibleChunks.clear();
			m_visibleChunks.reserve(lastCap*2);

			size_t fromChunkX = viewRect.left / (m_map.m_tileSize.x * chunkSize);
			size_t fromChunkY = viewRect.top / (m_map.m_tileSize.y * chunkSize);
			size_t toChunkX = (viewRect.left + viewRect.width) / (m_map.m_tileSize.x * chunkSize);
			size_t toChunkY = (viewRect.top + viewRect.height) / (m_map.m_tileSize.y * chunkSize);

			for (size_t y = fromChunkY; y <= toChunkY; y++)
			{
				for (size_t x = fromChunkX; x <= toChunkX; x++)
				{
					size_t index = y * m_map.m_dim.x / (chunkSize * m_map.m_tileSize.x) + x;
					if (index >= m_chunks.size())
						continue;
					Chunk* chunk = m_chunks[index];
					if (chunk)
					{
						sf::FloatRect bounds = chunk->vertices.getBounds();
						bounds.left += chunk->pos.x;
						bounds.top += chunk->pos.y;
						if (viewRect.intersects(bounds))
						{
							m_visibleChunks.push_back(chunk);
							chunk->draw(target, states, true);
						}
					}
				}
			}*/


			/*sf::Vector2f offset(m_map.m_tileSize.x * chunkSize, m_map.m_tileSize.y * chunkSize);
			for (const auto& chunk : m_chunks)
			{
				sf::FloatRect bounds = chunk.second->vertices.getBounds();
				bounds.left += chunk.second->pos.x;
				bounds.top += chunk.second->pos.y;
				if (isVisible(bounds, target))
				{
					m_visibleChunks.push_back(chunk.second);
					chunk.second->draw(target, states, true);
				}
			}*/
		}
		else
		{
			if (textureIndex < resolutionCount)
				states.texture = m_texture[textureIndex];
			for (const auto& chunk : m_visibleChunks)
			{
				chunk->draw(target, states, true);
			}
		}

		logInfo("Detail["+std::to_string(textureIndex)+"] Drawed " + std::to_string(m_visibleChunks.size()) + " chunks");

		//target.draw(m_vertices, states);
	}
}