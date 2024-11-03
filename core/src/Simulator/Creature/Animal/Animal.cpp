#include "Simulator/Creature/Animal/Animal.h"
#include "Simulator/Enviroment/Map/Map.h"

namespace EcoSystem
{
	namespace Animals
	{
		Animal::Animal(const sf::Texture& texture, const std::string& name)
			: Entity(name)
			, health(1)
			, maxHealth(1)
			, speed(1)
			, normalSpeed(5)
			, maxSpeed(5)
			, demageSpeedMultiplier(1)
			, fatSpeedMultiplier(1)
			, staminaSpeedMultiplier(1)
			, energy(1)
			, maxEnergy(1)
			, hunger(0)
			, maxHunger(1)
			, thirst(0)
			, maxThirst(1)
			, fat(0)
			, maxFat(1)
			, maxAge(3600)
			, reproduction(0)
			, maxReproduction(1)
			, walkEnergyCost(0.1)
			, alive(true)
			, lastMoveTimePoint(0)
			, walkedDistance(0)
			, walkedBackwardsDistance(0)
			, turnedDistance(0)
			, turnedLeftDistance(0)
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


			for (size_t i = 0; i < bodyDimensions.size(); i++)
			{
				bodyDimensions[i] = QSFML::Utilities::AABB(-0.5, -0.5, 1, 1);
			}
			if (bodyDimensions.size() > 0)
				currentBodyDimension = bodyDimensions[0];
			else
				currentBodyDimension = QSFML::Utilities::AABB(-0.5, -0.5, 1, 1);
			bodyCollider = new QSFML::Components::Collider("BodyCollider");
			addComponent(bodyCollider);
			bodyCollider->setVertecies({
				currentBodyDimension.TL(),
				currentBodyDimension.TR(),
				currentBodyDimension.BR(),
				currentBodyDimension.BL()
				});

			//addComponent(bodyCollider->createPainter());
			updateObjectChanges();

			speedChanged();
			updateBodyTexture();
			updateDemageTexture();

			/*setCustomBoundingBoxFunction([this]() -> QSFML::Utilities::AABB
				{
					sf::Transform transform = getGlobalTransform();
					sf::Vector2f currentDimensions = getCurrentBodyDimension() * 0.5f;
					sf::Vector2f TL = -currentDimensions;
					sf::Vector2f TR(currentDimensions.x, -currentDimensions.y);
					sf::Vector2f BL(-currentDimensions.x, currentDimensions.y);
					sf::Vector2f BR = currentDimensions;

					TL = transform.transformPoint(TL);
					TR = transform.transformPoint(TR);
					BL = transform.transformPoint(BL);
					BR = transform.transformPoint(BR);

					sf::Vector2f min(std::min({ TL.x, TR.x, BL.x, BR.x }), std::min({ TL.y, TR.y, BL.y, BR.y }));
					sf::Vector2f max(std::max({ TL.x, TR.x, BL.x, BR.x }), std::max({ TL.y, TR.y, BL.y, BR.y }));

					return QSFML::Utilities::AABB(min, max - min);
				});*/

			attachSelectorButtonToCollider(true);


			gizmos.head = sf::CircleShape(0.1, 3);
			gizmos.head.setFillColor(sf::Color::Green);
			gizmos.head.setOrigin(0.1, 0.1);
			gizmos.head.setPosition(headPosition);

			
			//addDrawFunction([this](const GameObject&, sf::RenderTarget& target, sf::RenderStates states)
			//	{
			//		static bool ena = true;
			//		if (ena)
			//		{
			//			drawGizmos(target, states);
			//			
			//		}
			//	});
			
		}

		Animal::~Animal()
		{
			
		}

		void Animal::setBodyDimensions(const std::array<QSFML::Utilities::AABB, TextureCoords::bodyCount>& bodyDimensions)
		{
			this->bodyDimensions = bodyDimensions;

			unsigned int fatIndex = static_cast<int>((fat * static_cast<float>(TextureCoords::bodyCount - 1)) / maxFat);
			currentBodyDimension = bodyDimensions[fatIndex];
			bodyCollider->setVertecies({
				currentBodyDimension.TL(),
				currentBodyDimension.TR(),
				currentBodyDimension.BR(),
				currentBodyDimension.BL()
				});
			bodyCollider->updateColliderData();
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

		Animal::SensoryData Animal::getSensoryData()
		{
			SensoryData data;
			data.internal.health = health;
			data.internal.speed = speed;
			data.internal.demageSpeedMultiplier = demageSpeedMultiplier;
			data.internal.fatSpeedMultiplier = fatSpeedMultiplier;
			data.internal.staminaSpeedMultiplier = staminaSpeedMultiplier;
			data.internal.energy = energy;
			data.internal.hunger = hunger;
			data.internal.thirst = thirst;
			data.internal.fat = fat;
			data.internal.age = getAge();
			data.internal.reproduction = reproduction;
			data.internal.alive = alive;

			data.enviromental.position = getGlobalPosition();
			data.enviromental.direction = getDirection();
			data.enviromental.visionPoints = getVisionGlobalPoints();
			
			return data;
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

		void Animal::showInfo(sf::RenderTarget& target, sf::RenderStates)
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

			// 2. Get the ImGui widget's screen coordinates
			ImVec2 windowPos = ImGui::GetWindowPos();  // Top-left corner
			ImVec2 _maxCoords = ImGui::GetWindowSize();
			

			ImGui::End();
			
			sf::Vector2f minCoords = getInWorldSpace({ (int)windowPos.x, (int)windowPos.y });
			sf::Vector2f maxCoords = getInWorldSpace({ (int)(_maxCoords.x+ windowPos.x), (int)(_maxCoords.y+ windowPos.y) });

			sf::Vector2f objPos = getGlobalPosition();
			sf::Vector2f pointerStart = maxCoords;
			if (QSFML::VectorMath::getLength(minCoords - objPos) < QSFML::VectorMath::getLength(maxCoords - objPos))
			{
				pointerStart = minCoords;
			}
			sf::VertexArray pointer(sf::Lines, 2);
			pointer[0].position = pointerStart;
			pointer[1].position = objPos;
			pointer[0].color = sf::Color::Blue;
			pointer[1].color = sf::Color::Blue;
			target.draw(pointer);
		}
		void Animal::drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const
		{
			Entity::drawGizmos(target, states);
			target.draw(gizmos.head, states);
			target.draw(gizmos.visionVertexArray, states);
		}

		void Animal::walkForward(float speed)
		{
			if (!alive) [[unlikely]]
				return;
			if (speed < 0)
			{
				walkBackward(-speed);
				return;
			}
			else if (speed > 1)
				speed = 1;
			double rot = static_cast<double>(GameObject::getRotation());
			sf::Vector2f direction(static_cast<float>(std::sin(rot * M_PI / 180.0)),
				static_cast<float>(-std::cos(rot * M_PI / 180.0)));
			float deltaT = static_cast<float>(getDeltaT());
			speed *= this->speed * deltaT;
			GameObject::move(direction * speed);
			walkedDistance += speed;
			consumeEnergy(walkEnergyCost * deltaT);
			lastMoveTimePoint = GameObject::getAge();
		}
		void Animal::walkBackward(float speed = 1)
		{
			if (!alive) [[unlikely]]
				return;
			if (speed < 0)
			{
				walkForward(-speed);
				return;
			}
			else if (speed > 1)
				speed = 1;
			double rot = static_cast<double>(GameObject::getRotation());
			sf::Vector2f direction(static_cast<float>(std::sin(rot * M_PI / 180.0)),
				static_cast<float>(-std::cos(rot * M_PI / 180.0)));
			float deltaT = static_cast<float>(getDeltaT());
			speed *= this->speed * deltaT;
			GameObject::move(-direction * speed);
			consumeEnergy(walkEnergyCost*2 * deltaT);
			walkedDistance += speed;
			walkedBackwardsDistance += speed;
			lastMoveTimePoint = GameObject::getAge();
		}
		void Animal::turnRight(float speed)
		{
			if (!alive) [[unlikely]]
				return;
			if (speed < 0)
			{
				turnLeft(-speed);
				return;
			}
			else if (speed > 1)
				speed = 1;
			float deltaT = static_cast<float>(getDeltaT());
			speed *= this->speed * deltaT;

			GameObject::rotate(speed * 90.f);
			turnedDistance += speed * 90.f;
			consumeEnergy(walkEnergyCost * deltaT);
			lastMoveTimePoint = GameObject::getAge();
		}
		void Animal::turnLeft(float speed)
		{
			if (!alive) [[unlikely]]
				return;
			if (speed < 0)
			{
				turnRight(-speed);
				return;
			}
			else if (speed > 1)
				speed = 1;
			float deltaT = static_cast<float>(getDeltaT());
			speed *= this->speed * deltaT;
			GameObject::rotate(-speed * 90.f);
			turnedDistance += speed * 90.f;
			turnedLeftDistance += speed * 90.f;
			consumeEnergy(walkEnergyCost * deltaT);
			lastMoveTimePoint = GameObject::getAge();
		}

		void Animal::onAwake()
		{
			lastMoveTimePoint = getAge();
		}
		void Animal::update()
		{
			//float deltaTime = getDeltaT();
			checkPositionOutOfMap();
			if (alive)
			{				
				if (getAge() > maxAge)
				{
					health = 0;
					healthChanged();					
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

		void Animal::checkPositionOutOfMap()
		{
			sf::Vector2f pos = getPosition();
			if (!Map::isInMap(pos))
			{
				sf::Vector2f dir = Map::getNearestMapBorderDirection(pos);
				move(dir);
			}
		}
	}
}