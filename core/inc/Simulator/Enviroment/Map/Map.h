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

			//COMPONENT_DECL(MapPainter);

			void updateMap();

			void drawComponent(sf::RenderTarget& target, sf::RenderStates states) const override;
		
			Map& m_map;

			constexpr static size_t resolutionCount = 4;
			constexpr static size_t chunkSize = 32;

			const sf::Texture* m_texture[resolutionCount];
			//sf::VertexArray m_vertices;
			struct Chunk
			{
				sf::VertexArray vertices;
				sf::Vector2f pos;
				sf::Color averageColor;

				void draw(sf::RenderTarget& target, sf::RenderStates states, bool drawBorder) const
				{
					states.transform.translate(pos);
					/*if (detailStage == 3)
					{
						sf::FloatRect bounds = vertices.getBounds();
						sf::VertexArray _vertices(sf::Quads, 4);
						_vertices[0].position = sf::Vector2f(0, 0);
						_vertices[1].position = sf::Vector2f(bounds.width, 0);
						_vertices[2].position = sf::Vector2f(bounds.width, bounds.height);
						_vertices[3].position = sf::Vector2f(0, bounds.height);
						states.texture = nullptr;
						_vertices[0].color = averageColor;
						_vertices[1].color = averageColor;
						_vertices[2].color = averageColor;
						_vertices[3].color = averageColor;
						target.draw(_vertices, states);
					}
					else
					{*/
						target.draw(vertices, states);
					//}

					if (!drawBorder)
						return;
					// Draw red border
					sf::VertexArray border(sf::LinesStrip, 5);
					sf::FloatRect bounds = vertices.getBounds();
					border[0].position = sf::Vector2f(bounds.left, bounds.top);
					border[1].position = sf::Vector2f(bounds.left + bounds.width, bounds.top);
					border[2].position = sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
					border[3].position = sf::Vector2f(bounds.left, bounds.top + bounds.height);
					border[4].position = sf::Vector2f(bounds.left, bounds.top);
					for (size_t i = 0; i < 5; i++)
						border[i].color = sf::Color::Red;
					target.draw(border, states);
				}
			};
			//std::unordered_map<size_t, Chunk*> m_chunks;
			std::vector<Chunk*> m_chunks;
			sf::Vector2u m_chunkDim;

			std::vector<Chunk*> m_previewChunks;
			sf::Vector2u m_previewChunkDim;

			Chunk*& getChunk(const sf::Vector2u& pos) {
				return m_chunks[(size_t)(pos.y * m_chunkDim.x + pos.x)];
			}

			Chunk*& getPreviewChunk(const sf::Vector2u& pos) {
				return m_previewChunks[(size_t)(pos.y * m_previewChunkDim.x + pos.x)];
			}
			
			struct VertexPos
			{
				sf::Vector2f TL;
				sf::Vector2f TR;
				sf::Vector2f BL;
				sf::Vector2f BR;

				VertexPos() {}
				VertexPos(float x, float y, float w, float h, float padding)
				{
					TL = sf::Vector2f(x + padding, y + padding );
					TR = sf::Vector2f(x + w - padding, padding);
					BR = sf::Vector2f(x + w - padding, y + h - padding);
					BL = sf::Vector2f(x + padding, y + h - padding);
				}
			};
			VertexPos m_textCoords[static_cast<size_t>(MapTileType::Count)];
		
			mutable float m_lastZoom = 0;
			mutable sf::Vector2f m_lastViewCenter;
			mutable std::vector<Chunk*> m_visibleChunks;
		};

		sf::Vector2u m_dim;
		sf::Vector2f m_tileSize;
		bool m_wrapAround;



		QSFML::vector<MapTile> m_tiles;

		MapPainter* m_painter = nullptr;


		static const std::string s_mapTileTexturePath;
	};
}