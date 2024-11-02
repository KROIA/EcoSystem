#include "Simulator/Creature/Animals/Controllers/PlayerController.h"

namespace EcoSystem
{
	namespace Controller
	{
		void PlayerController::updateSelectedAnimal(Animals::Animal* animal)
		{
			// Make player movements using WASD keys
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				animal->walkForward();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				animal->walkBackward();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				animal->turnLeft();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				animal->turnRight();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				// eat if possible
				/*std::vector< MapChunkData::MapTile*> tiles = animal->getVisionTiles();
				for (MapChunkData::MapTile* tile : tiles)
				{
					switch (tile->type)
					{
					case MapTileType::Grass:
					{
						float foodAmount = tile->data.grass.growth * 10;
						tile->data.grass.growth = 0;
						animal->eat(foodAmount);
						animal->consumeEnergy(foodAmount * 0.1f);
						animal->addThirst(foodAmount * 0.1f);
						break;
					}
					case MapTileType::Water:
					{
						float waterAmount = 10;
						animal->drink(waterAmount);
						animal->consumeEnergy(1);
						break;
					}
					}
				}*/
				MapChunkData::MapTile& tile = animal->getMapTileBelowHead();
				switch (tile.type)
				{
				case MapTileType::Grass:
				{
					float foodAmount = tile.data.grass.growth * 10;
					tile.data.grass.growth = 0;
					animal->eat(foodAmount);
					animal->consumeEnergy(foodAmount * 0.1f);
					animal->addThirst(foodAmount * 0.1f);
					break;
				}
				case MapTileType::Water:
				{
					float waterAmount = 10;
					animal->drink(waterAmount);
					animal->consumeEnergy(1);
					break;
				}
				}
			}
		}
		void PlayerController::drawSelectedAnimal(Animals::Animal* animal, sf::RenderTarget& target, sf::RenderStates states)
		{
			animal->showInfo();
			animal->drawGizmos(target, states);
		}
	}
}