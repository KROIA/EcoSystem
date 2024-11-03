#include "Simulator/Creature/Animal/Sheep.h"
#include <SFML/OpenGL.hpp>


namespace EcoSystem
{
	namespace Animals
	{
		Log::LogObject& Sheep::getLogger()
		{
			static Log::LogObject s_log(EcoSystem::Logger::getID(), "Sheep");
			return s_log;
		}
		Sheep::Sheep(const std::string& name)
			: Animal(QSFML::Assets::TextureManager::getTexture("Sheep.png"), name)
		{
	
			setVisionRays(
				{ 
					sf::Vector2f(-1, -1), 
					sf::Vector2f(0, -1), 
					sf::Vector2f(1, -1),
					sf::Vector2f(0, -0.01),
					sf::Vector2f(-2, -2),
					sf::Vector2f(-1, -2),
					sf::Vector2f(0, -2),
					sf::Vector2f(1, -2),
					sf::Vector2f(2, -2),
					sf::Vector2f(-2, -3),
					sf::Vector2f(-1, -3),
					sf::Vector2f(0, -3),
					sf::Vector2f(1, -3),
					sf::Vector2f(2, -3),
					sf::Vector2f(-1, -4),
					sf::Vector2f(0, -4),
					sf::Vector2f(1, -4)

				});
			setBodyDimensions(
				{
					QSFML::Utilities::AABB(-2/16.f, -8/16.f, 4/16.f, 13/16.f),
					QSFML::Utilities::AABB(-2/16.f, -8/16.f, 4/16.f, 13/16.f),
					QSFML::Utilities::AABB(-2/16.f, -8/16.f, 4/16.f, 14/16.f),
					QSFML::Utilities::AABB(-3/16.f, -8/16.f, 6/16.f, 14/16.f),
					QSFML::Utilities::AABB(-4/16.f, -8/16.f, 8/16.f, 14/16.f),
					QSFML::Utilities::AABB(-5/16.f, -8/16.f, 10/16.f, 15/16.f),
					QSFML::Utilities::AABB(-6/16.f, -8/16.f, 12/16.f, 16/16.f),
					QSFML::Utilities::AABB(-7/16.f, -8/16.f, 14/16.f, 16/16.f),
					QSFML::Utilities::AABB(-8/16.f, -8/16.f, 16/16.f, 16/16.f)
				});
		}
		Sheep::~Sheep()
		{

		}

		void Sheep::tryEat()
		{
			MapChunkData::MapTile& tile = getMapTileBelowHead();
			switch (tile.type)
			{
			case MapTileType::Grass:
			{
				float foodAmount = tile.data.grass.growth * 0.1;
				tile.data.grass.growth = 0;
				eat(foodAmount);
				consumeEnergy(foodAmount * 0.001f);
				addThirst(foodAmount * 0.001f);
				break;
			}
			}
		}
		void Sheep::tryDrink()
		{
			MapChunkData::MapTile& tile = getMapTileBelowHead();
			switch (tile.type)
			{
			case MapTileType::Water:
			{
				float waterAmount = 0.1;
				drink(waterAmount);
				consumeEnergy(0.01);
				break;
			}
			}
		}
		void Sheep::tryReproduce()
		{

		}
		void Sheep::getsAttacked(float damage)
		{
			Animal::damage(damage);
		}
		void Sheep::getsHealed(float amount)
		{
			Animal::heal(amount);
		}
		void Sheep::tryAttack()
		{

		}
		void Sheep::tryWalkForward(float speed)
		{
			walkForward(speed);
		}
		void Sheep::tryWalkBackward(float speed)
		{
			walkBackward(speed);
		}
		void Sheep::tryTurnRight(float speed)
		{
			turnRight(speed);
		}
		void Sheep::tryTurnLeft(float speed)
		{
			turnLeft(speed);
		}

		void Sheep::eat(float amount)
		{
			if (amount < 0)
				return;
			float hunger = getHunger();
			float fat = getFat();
			float health = getHealth();

			hunger -= amount;
			fat += amount * 0.1f;
			if(hunger < 0)
				fat += amount * 0.3f;
			health += amount * 0.1f;

			setHunger(hunger);
			setFat(fat);
			setHealth(health);
		}

		/*void Sheep::drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const
		{
			Animal::drawGizmos(target, states);
		}*/

		void Sheep::update()
		{
			Animal::update();
			if (!isAlive())
				return;
			float deltaT = static_cast<float>(getDeltaT());

			

			float energy = getEnergy();
			float health = getHealth();
			if (energy > getMaxEnergy()*0.3f && health < getMaxHealth())
			{
				float energyCost = deltaT * 0.01;
				if (energy < energyCost)
				{
					energyCost = energy;
				}
				energy -= energyCost;
				health += energyCost;
				if (health > getMaxHealth())
				{
					health = getMaxHealth();
				}
				setEnergy(energy);
				setHealth(health);				
			}
			if (energy < getMaxEnergy())
			{
				float fat = getFat();
				if (fat > 0)
				{
					float idleTime = (float)getTimeSinceLastMove();
					if(idleTime > 10)
						idleTime = 10;
					float energyGain = deltaT * idleTime*0.001;
					if (fat < energyGain)
					{
						energyGain = fat;
					}
					fat -= energyGain;
					energy += energyGain*10;
					setFat(fat);
					setEnergy(energy);
				}
				else if (energy <= std::numeric_limits<float>::epsilon())
				{
					damage(deltaT*0.01);
				}
				addHunger(0.005 * deltaT);
				addThirst(0.003 * deltaT);
			}
			else
			{
				addHunger(0.001 * deltaT);
				addThirst(0.001 * deltaT);
			}

			
		}
		void Sheep::died()
		{
			getLogger().logInfo("Sheep died");
		}
		void Sheep::reproduce()
		{
			getLogger().logInfo("Sheep reproduced");
		}
		void Sheep::consumeEnergy(float amount)
		{
			if (amount <= 0)
				return;
			float energy = getEnergy();
			energy -= amount;
			if (energy < 0)
			{
				float fat = getFat();
				fat -= amount * 0.1;
				setStaminaSpeedMultiplier(0.5);
				energy = 0;
				setFat(fat);
			}
			setEnergy(energy);
		}

	}
}