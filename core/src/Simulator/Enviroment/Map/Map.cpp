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
		QSFML::Utilities::PerlinNoise perlinNoise(0);

		float scale = 1;
		for (unsigned int y = 0; y < m_dim.y; y++)
		{
			for (unsigned int x = 0; x < m_dim.x; x++)
			{
				float value = perlinNoise.noise(x* scale, y* scale, 5, m_dim)*10;
				MapTileType type = MapTileType::Grass;
				if (value < 0.2f)
					type = MapTileType::Water;
				else if (value < 0.3f)
					type = MapTileType::Sand;
				else if (value < 0.6f)
					type = MapTileType::Dirt;
				else if (value < 0.8f)
					type = MapTileType::Rock;
				getTile({ x, y }).type = type;
			}
		}

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
	{
		std::string textureBasePath = s_mapTileTexturePath+"/";
		for (int i = 0; i < static_cast<int>(MapTileType::Count); i++)
		{
			m_textures[i] = &QSFML::Assets::TextureManager::getTexture(textureBasePath + toString(static_cast<MapTileType>(i)) + ".png");
		}
	}

	Map::MapPainter::~MapPainter()
	{
	}

	COMPONENT_IMPL(Map::MapPainter);

	void Map::MapPainter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Sprite sprite;
		sprite.setScale(m_map.m_tileSize.x / m_textures[0]->getSize().x, m_map.m_tileSize.y / m_textures[0]->getSize().y);
		for (unsigned int y = 0; y < m_map.m_dim.y; y++)
		{
			for (unsigned int x = 0; x < m_map.m_dim.x; x++)
			{
				const MapTile& tile = m_map.getTile({ x, y });
				sprite.setTexture(*m_textures[static_cast<int>(tile.type)]);
				sprite.setPosition(x * m_map.m_tileSize.x, y * m_map.m_tileSize.y);
				target.draw(sprite, states);
			}
		}
	}
}