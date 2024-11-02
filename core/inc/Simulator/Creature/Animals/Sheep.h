#pragma once

#include "EcoSystem_base.h"
#include "Animal.h"

namespace EcoSystem
{
	namespace Animals
	{
		class ECO_SYSTEM_EXPORT Sheep : public Animal
		{
			public:
				static Log::LogObject& getLogger();
				Sheep(const std::string& name = "Sheep");
				~Sheep();

				void eat(float amount) override;

				//void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const override;
			private:
				void update() override;
				void died() override;
				void reproduce() override;
				void consumeEnergy(float amount) override;				
		};
	}
}