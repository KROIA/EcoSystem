#include "Simulator/Creature/Animals/Animal.h"

namespace EcoSystem
{
	namespace Animals
	{
		Animal::Animal(const sf::Texture& texture, const std::string& name)
			: Entity(name)
			, health(100)
			, maxHealth(100)
			, speed(1)
			, normalSpeed(0.1)
			, maxSpeed(1)
			, demageSpeedMultiplier(1)
			, fatSpeedMultiplier(1)
			, stainaSpeedMultiplier(1)
			, energy(100)
			, maxEnergy(100)
			, hunger(0)
			, maxHunger(100)
			, thirst(0)
			, maxThirst(100)
			, fat(0)
			, maxFat(100)
			, maxAge(3600)
			, reproduction(0)
			, maxReproduction(100)
			, walkEnergyCost(0.1)
			, alive(true)
			, lastMoveTimePoint(0)
			, walkedDistance(0)
			, turnedDistance(0)
			, headPosition(0, -0.45)
			, textureMap(texture, sf::Vector2u(textureSizeX, textureSizeY))
		{
			bodySprite = new sf::Sprite();
			demageSprite = new sf::Sprite();
			bodySprite->setTexture(textureMap.getTexture());
			demageSprite->setTexture(textureMap.getTexture());
			bodySprite->setOrigin(8, 8);
			demageSprite->setOrigin(8, 8);

			bodySprite->setScale(1/16.f, 1/16.f);
			demageSprite->setScale(1/16.f, 1/16.f);
			addComponent(bodySprite);
			addComponent(demageSprite);

			speedChanged();
			updateBodyTexture();
			updateDemageTexture();

			setCustomBoundingBoxFunction([this]() -> QSFML::Utilities::AABB
				{
					return QSFML::Utilities::AABB(getPosition()-sf::Vector2f(0.5,0.5), sf::Vector2f(1,1));
				});

			attachSelectorButtonToCollider(true);


			gizmos.head = sf::CircleShape(0.1, 3);
			gizmos.head.setFillColor(sf::Color::Green);
			gizmos.head.setOrigin(0.1, 0.1);
			gizmos.head.setPosition(headPosition);
		}

		Animal::~Animal()
		{
		}

		void Animal::setVisionRays(const std::vector<sf::Vector2f>& visionRays)
		{
			this->visionRays = visionRays;
			gizmos.visionVertexArray.resize(visionRays.size() * 2);
			gizmos.visionVertexArray.setPrimitiveType(sf::Lines);
			
			for (size_t i = 0; i < visionRays.size(); i++)
			{
				sf::Vector2f visionDir = visionRays[i];
				sf::Vector2f visionEnd = headPosition + visionDir;
				gizmos.visionVertexArray[i * 2].position = headPosition;
				gizmos.visionVertexArray[i * 2 + 1].position = visionEnd;
				gizmos.visionVertexArray[i * 2].color = sf::Color::Blue;
				gizmos.visionVertexArray[i * 2 + 1].color = sf::Color::Blue;
			}
		}

		std::vector<sf::Vector2f> Animal::getVisionPoints() const
		{
			std::vector<sf::Vector2f> visionPoints;
			visionPoints.resize(visionRays.size());
			for (size_t i = 0; i < visionRays.size(); i++)
			{
				visionPoints[i] = headPosition + visionRays[i];
			}
			return visionPoints;
		}
		std::vector<sf::Vector2f> Animal::getVisionGlobalPoints() const
		{
			std::vector<sf::Vector2f> visionPoints;
			visionPoints.resize(visionRays.size());
			sf::Transform transform = getGlobalTransform();
			for (size_t i = 0; i < visionRays.size(); i++)
			{
				visionPoints[i] = transform.transformPoint(headPosition + visionRays[i]);
			}
			return visionPoints;
		}
		std::vector<MapChunkData::MapTile*> Animal::getVisionTiles() const
		{
			std::vector<MapChunkData::MapTile*> tiles;
			tiles.resize(visionRays.size());
			sf::Transform transform = getGlobalTransform();
			for (size_t i = 0; i < visionRays.size(); i++)
			{
				sf::Vector2f pos = transform.transformPoint(headPosition + visionRays[i]);
				tiles[i] = &Map::getInstance()->getTile(pos);
			}
			return tiles;
		}

		void Animal::eat(float amount)
		{
			if (amount < 0)
				return;
			hunger -= amount;
			if (hunger < 0)
			{
				health -= hunger;
				hunger = 0;
				if (health > maxHealth)
				{
					fat += health - maxHealth;
					health = maxHealth;
					if (fat > maxFat)
					{
						fat = maxFat;
					}
					fatChanged();
				}
				healthChanged();
			}
			hungerChanged();
		}
		void Animal::drink(float amount)
		{
			if (amount < 0)
				return;
			thirst -= amount;
			if (thirst < 0)
			{
				// Healing only when health is below 50%
				if (health < maxHealth / 2.f)
				{
					health -= thirst;
					healthChanged();
				}
				thirst = 0;
			}
			thirstChanged();
		}
		void Animal::damage(float amount)
		{
			if (amount < 0)
				return;
			health -= amount;
			if (health < 0)
			{
				health = 0;
			}
			healthChanged();
		}
		void Animal::heal(float amount)
		{
			if (amount < 0)
				return;
			health += amount;
			if (health > maxHealth)
			{
				health = maxHealth;
			}
			//std::min()
			healthChanged();
		}

		void Animal::showInfo()
		{
			// Display info about the animal using IMGui and ImPlot
			ImGui::Begin(("Animal Info: "+getName()).c_str());
			ImGui::Text("Is alive: %s", alive ? "true" : "false");
			ImGui::Text("Age: %f", getAge());

			// Create slider to visualize different values
			if (ImGui::SliderFloat("Health", &health, 0, maxHealth)) healthChanged();
			ImGui::SliderFloat("Speed", &speed, 0, maxSpeed);
			if (ImGui::SliderFloat("  Normal speed", &normalSpeed, 0, maxSpeed)) speedChanged();
			if (ImGui::SliderFloat("  Demage speed multiplyer", &demageSpeedMultiplier, 0, maxSpeed)) speedChanged();
			if (ImGui::SliderFloat("  Fat speed multiplyer", &fatSpeedMultiplier, 0, maxSpeed)) speedChanged();

			if (ImGui::SliderFloat("Energy", &energy, 0, maxEnergy)) energyChanged();
			if (ImGui::SliderFloat("Hunger", &hunger, 0, maxHunger)) hungerChanged();
			if (ImGui::SliderFloat("Thirst", &thirst, 0, maxThirst)) thirstChanged();
			if (ImGui::SliderFloat("Fat", &fat, 0, maxFat)) fatChanged();
			if (ImGui::SliderFloat("Reproduction", &reproduction, 0, maxReproduction)) reproductionChanged();


			ImGui::End();


		}
		void Animal::drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const
		{
			Entity::drawGizmos(target, states);
			target.draw(gizmos.head, states);
			target.draw(gizmos.visionVertexArray, states);
		}
		void Animal::onAwake()
		{
			lastMoveTimePoint = getAge();
		}
		void Animal::update()
		{
			//float deltaTime = getDeltaT();
			if (alive)
			{				
				if (getAge() > maxAge)
				{
					died();
					return;
				}
			}
		}
		void Animal::consumeEnergy(float amount)
		{
			if (amount < 0)
				return;
			energy -= amount;
			if (energy < 0)
			{
				fat += energy;
				energy = 0;			
				if (fat < 0)
				{
					damage(-fat);
					fat = 0;
				}
				fatChanged();
			}
			energyChanged();
		}
	}
}