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

				

				void tryEat() override;
				void tryDrink() override;
				void tryReproduce() override;
				void getsAttacked(float damage) override;
				void getsHealed(float amount) override;
				void tryAttack() override;
				void tryWalkForward(float speed = 1) override;
				void tryWalkBackward(float speed = 1) override;
				void tryTurnRight(float speed = 1) override;
				void tryTurnLeft(float speed = 1) override;

				//void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const override;
			private:
				void eat(float amount) override;

				void update() override;
				void died() override;
				void reproduce() override;
				void consumeEnergy(float amount) override;				
		};
	}
}