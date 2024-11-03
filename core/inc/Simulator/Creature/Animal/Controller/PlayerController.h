#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Creature/Animal/Animal.h"

namespace EcoSystem
{
	namespace Controller
	{
		class ECO_SYSTEM_EXPORT PlayerController
		{
			PlayerController() = delete;
			~PlayerController() = delete;
		public:
			static void updateAnimal(Animals::Animal* animal);
			static void drawAnimal(Animals::Animal* animal, sf::RenderTarget& target, sf::RenderStates states);
		};

	}
}
