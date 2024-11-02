#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Creature/Animals/Animal.h"

namespace EcoSystem
{
	namespace Controller
	{
		class ECO_SYSTEM_EXPORT PlayerController
		{
			PlayerController() = delete;
			~PlayerController() = delete;
		public:
			static void updateSelectedAnimal(Animals::Animal* animal);
			static void drawSelectedAnimal(Animals::Animal* animal, sf::RenderTarget& target, sf::RenderStates states);
		};

	}
}
