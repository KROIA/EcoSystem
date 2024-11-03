#include "Simulator/Creature/Animal/Controller/PlayerController.h"

namespace EcoSystem
{
	namespace Controller
	{
		void PlayerController::updateAnimal(Animals::Animal* animal)
		{
			// Make player movements using WASD keys
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				animal->tryWalkForward();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				animal->tryWalkBackward();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				animal->tryTurnLeft();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				animal->tryTurnRight();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				animal->tryEat();
				animal->tryDrink();
			}
		}
		void PlayerController::drawAnimal(Animals::Animal* animal, sf::RenderTarget& target, sf::RenderStates states)
		{
			animal->showInfo(target, states);
			//animal->drawGizmos(target, states);
		}
	}
}