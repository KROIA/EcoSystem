#include "Simulator/Creature/Animals/Sheep.h"
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
					sf::Vector2f(1, -1) 
				});
		}
		Sheep::~Sheep()
		{

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
			float deltaTime = getDeltaT();

			

			float energy = getEnergy();
			float health = getHealth();
			if (energy > getMaxEnergy()*0.3f && health < getMaxHealth())
			{
				float energyCost = deltaTime * 1;
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
					float energyGain = deltaTime * idleTime*0.1;
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
					damage(deltaTime);
				}
			}

			addHunger(0.1 * deltaTime);
			addThirst(0.1 * deltaTime);
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
				setStainaSpeedMultiplier(0.5);
				energy = 0;
			}
			setEnergy(energy);
		}

	}
}