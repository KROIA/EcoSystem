#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Entity.h"

namespace EcoSystem
{
	class ECO_SYSTEM_EXPORT Map : public Entity
	{
		friend class MapPainter;
	public:
		enum class MapTileType
		{
			Grass,
			Dirt,
			Water,
			Sand,
			Rock,
			//Tree,

			Count
		};
		struct MapTile
		{
			MapTileType type;
			sf::Sprite sprite;
		};

		static Log::LogObject& getLogger();

		Map(const sf::Vector2u &dim, const sf::Vector2f &tileSize, const std::string& name = "Map");
		Map(const Map& other);
		~Map();

		OBJECT_DECL(Map);

		static const char* toString(MapTileType type)
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

		void generateMap();

	private:

		bool indexValid(const sf::Vector2u& pos) const;
		MapTile& getTile(const sf::Vector2u& pos);

		class ECO_SYSTEM_EXPORT MapPainter : public QSFML::Components::Drawable
		{
		public:
			MapPainter(Map &map);
			~MapPainter();

			COMPONENT_DECL(MapPainter);

			void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;
		
			Map& m_map;

			const sf::Texture* m_textures[static_cast<int>(MapTileType::Count)];
		};

		sf::Vector2u m_dim;
		sf::Vector2f m_tileSize;
		bool m_wrapAround;



		QSFML::vector<MapTile> m_tiles;

		MapPainter* m_painter = nullptr;


		static const std::string s_mapTileTexturePath;
	};
}