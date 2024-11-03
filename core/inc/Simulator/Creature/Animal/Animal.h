#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Entity.h"
#include "Simulator/Enviroment/Map/Map.h"

namespace EcoSystem
{
	namespace Animals
	{
		class ECO_SYSTEM_EXPORT Animal : public Entity
		{
			static constexpr unsigned int textureSizeX = 9;
			static constexpr unsigned int textureSizeY = 7;
			struct TextureCoords
			{
				// Body texture start index
				static constexpr unsigned int body = 0;
				// Offset between body textures
				static constexpr unsigned int bodyIncrement = 1;
				// Number of body textures
				static constexpr unsigned int bodyCount = textureSizeX;

				// Demage texture start index
				static constexpr unsigned int demage = body + bodyCount;
				// Offset between demage textures
				static constexpr unsigned int demageIncrement = textureSizeX;
				// Number of demage textures
				static constexpr unsigned int demageCount = textureSizeY-1;
			};
			
		public:
			struct SensoryData
			{
				struct Internal
				{
					float health;
					float speed;
					float demageSpeedMultiplier;
					float fatSpeedMultiplier;
					float staminaSpeedMultiplier;
					float energy;
					float hunger;
					float thirst;
					float fat;
					float age;
					float reproduction;
					bool alive;
				};
				struct Enviromental
				{
					sf::Vector2f position;
					sf::Vector2f direction;

					// Vision points in world coordinates
					std::vector<sf::Vector2f> visionPoints;
				};

				Internal internal;
				Enviromental enviromental;
			};

			Animal(const sf::Texture &texture, const std::string& name = "Animal");
			Animal(const Animal& other) = delete;
			virtual ~Animal();

			inline void setHeadPosition(const sf::Vector2f& headPosition) { this->headPosition = headPosition; }
			const sf::Vector2f &getHeadPosition() const { return headPosition; }
			sf::Vector2f getHeadWorldPosition() const { return getTransform().transformPoint(headPosition); }
			MapChunkData::MapTile& getMapTileBelowHead() const { return Map::getInstance()->getTile(getHeadWorldPosition()); }
			
			void setBodyDimensions(const std::array<QSFML::Utilities::AABB, TextureCoords::bodyCount>& bodyDimensions);
			const std::array<QSFML::Utilities::AABB, TextureCoords::bodyCount>& getBodyDimensions() const { return bodyDimensions; }
			const QSFML::Utilities::AABB& getCurrentBodyDimension() const { return currentBodyDimension; }

			void setVisionRays(const std::vector<sf::Vector2f>& visionRays);
			const std::vector<sf::Vector2f>& getVisionRays() const { return visionRays; }

			inline void setHealth(float health) { this->health = health; healthChanged(); }
			inline void addHealth(float delta) { setHealth(health + delta); }
			inline float getHealth() const { return health; }
			inline bool isAlive() const { return alive; }
			void revive() { alive = true; setHealth(100); }

			inline void setMaxHealth(float maxHealth) { this->maxHealth = maxHealth; updateDemageTexture(); }
			inline float getMaxHealth() const { return maxHealth; }

			inline void setNormalSpeed(float speed) { this->normalSpeed = speed; speedChanged(); }
			inline float getNormalSpeed() const { return normalSpeed; }
			inline void setStaminaSpeedMultiplier(float multiplier) { this->staminaSpeedMultiplier = multiplier; speedChanged(); }
			inline float getStaminaSpeedMultiplier() const { return staminaSpeedMultiplier; }
			inline float getDemageSpeedMultiplier() const { return demageSpeedMultiplier; }
			inline float getFatSpeedMultiplier() const { return fatSpeedMultiplier; }
			inline float getSpeed() const { return normalSpeed * demageSpeedMultiplier * fatSpeedMultiplier; }


			inline void setMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }
			inline float getMaxSpeed() const { return maxSpeed; }

			inline void setEnergy(float energy) { this->energy = energy; energyChanged(); }
			inline void addEnergy(float delta) { setEnergy(energy + delta); }
			inline float getEnergy() const { return energy; }

			inline void setMaxEnergy(float maxEnergy) { this->maxEnergy = maxEnergy; }
			inline float getMaxEnergy() const { return maxEnergy; }

			inline void setHunger(float hunger) { this->hunger = hunger; hungerChanged(); }
			inline void addHunger(float delta) { setHunger(hunger + delta); }
			inline float getHunger() const { return hunger; }

			inline void setMaxHunger(float maxHunger) { this->maxHunger = maxHunger; }
			inline float getMaxHunger() const { return maxHunger; }

			inline void setThirst(float thirst) { this->thirst = thirst; thirstChanged(); }
			inline void addThirst(float delta) { setThirst(thirst + delta); }
			inline float getThirst() const { return thirst; }

			inline void setMaxThirst(float maxThirst) { this->maxThirst = maxThirst; }
			inline float getMaxThirst() const { return maxThirst; }

			inline void setFat(float fat) { this->fat = fat; fatChanged(); }
			inline void addFat(float delta) { setFat(fat + delta); }
			inline float getFat() const { return fat; }

			inline void setMaxFat(float maxFat) { this->maxFat = maxFat; updateBodyTexture(); }
			inline float getMaxFat() const { return maxFat; }

			inline double getAge() const { return GameObject::getAge(); }

			inline void setMaxAge(float maxAge) { this->maxAge = maxAge; }
			inline float getMaxAge() const { return maxAge; }

			inline void setReproduction(float reproduction) { this->reproduction = reproduction; reproductionChanged(); }
			inline void addReproduction(float delta) { setReproduction(reproduction + delta); }
			inline float getReproduction() const { return reproduction; }

			inline void setMaxReproduction(float maxReproduction) { this->maxReproduction = maxReproduction; }
			inline float getMaxReproduction() const { return maxReproduction; }

			inline void setWalkEnergyCost(float walkEnergyCost) { this->walkEnergyCost = walkEnergyCost; }
			inline float getWalkEnergyCost() const { return walkEnergyCost; }

			inline double getLastMoveTimePoint() const { return lastMoveTimePoint; }
			inline double getTimeSinceLastMove() const { return GameObject::getAge() - lastMoveTimePoint; }

			inline float getWalkedDistance() const { return walkedDistance; }
			inline float getTurnedDistance() const { return turnedDistance; }
			inline float getWalkedBackwardsDistance() const { return walkedBackwardsDistance; }
			inline float getTurnedLeftDistance() const { return turnedLeftDistance; }

			std::vector<sf::Vector2f> getVisionPoints() const;
			std::vector<sf::Vector2f> getVisionGlobalPoints() const;
			std::vector<MapChunkData::MapTile*> getVisionTiles() const;

			sf::Vector2f getDirection() const
			{
				double rot = static_cast<double>(GameObject::getRotation());
				return sf::Vector2f(static_cast<float>(std::cos(rot * M_PI / 180.0)), 
									static_cast<float>(std::sin(rot * M_PI / 180.0)));
			}

			SensoryData getSensoryData();
			

			virtual void tryEat() = 0;
			virtual void tryDrink() = 0;
			virtual void tryReproduce() = 0;
			virtual void getsAttacked(float damage) = 0;
			virtual void tryAttack() = 0;
			virtual void getsHealed(float amount) = 0;
			virtual void tryWalkForward(float speed = 1) = 0;
			virtual void tryWalkBackward(float speed = 1) = 0;
			virtual void tryTurnRight(float speed = 1) = 0;
			virtual void tryTurnLeft(float speed = 1) = 0;


			

			virtual void showInfo(sf::RenderTarget& target, sf::RenderStates states);

			void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const override;
		protected:
			virtual void eat(float amount);
			virtual void drink(float amount);
			virtual void damage(float amount);
			virtual void heal(float amount);
			virtual void consumeEnergy(float amount);
			void walkForward(float speed);
			void walkBackward(float speed);
			void turnRight(float speed);
			void turnLeft(float speed);

			void onAwake() override;
			virtual void update() override;
			virtual void died() = 0;
			virtual void reproduce() = 0;
			



			void move(const sf::Vector2f& offset) override
			{
				GameObject::move(offset);
			}
			void move(float offsetX, float offsetY) override
			{
				GameObject::move(offsetX, offsetY);
			}
			void rotate(float angle) override
			{
				GameObject::rotate(angle);
			}
			void setOrigin(const sf::Vector2f& origin) override
			{
				GameObject::setOrigin(origin);
			}
			void checkPositionOutOfMap();

			inline void updateBodyTexture()
			{
				unsigned int fatIndex = static_cast<int>((fat * static_cast<float>(TextureCoords::bodyCount-1)) / maxFat);
				const QSFML::Assets::TextureMap::UVMapCoords& bodyCoords = textureMap.getUVMapCoords(TextureCoords::body + TextureCoords::bodyIncrement * fatIndex);
				bodySprite->setTextureRect(bodyCoords.toIntRect());
				currentBodyDimension = bodyDimensions[fatIndex];
				bodyCollider->setVertecies({
				currentBodyDimension.TL(),
				currentBodyDimension.TR(),
				currentBodyDimension.BR(),
				currentBodyDimension.BL()
					});
				bodyCollider->updateColliderData();
			}
			inline void updateDemageTexture()
			{
				unsigned int demageIndex = static_cast<int>((health * static_cast<float>(TextureCoords::demageCount-1)) / maxHealth);
				unsigned int fatIndex = static_cast<int>((fat * static_cast<float>(TextureCoords::bodyCount - 1)) / maxFat);
				const QSFML::Assets::TextureMap::UVMapCoords& demageCoords = textureMap.getUVMapCoords(TextureCoords::demage + TextureCoords::demageIncrement * demageIndex + TextureCoords::bodyIncrement * fatIndex);
				demageSprite->setTextureRect(demageCoords.toIntRect());
			}
			

			inline void healthChanged()
			{
				if (health > maxHealth)
					health = maxHealth;
				else if(health < 0)
					health = 0;
				// Make animal slower when it is demaged: min speed is 50% of normal speed
				demageSpeedMultiplier = 0.5f + health / (maxHealth*2);
				speedChanged();
				updateDemageTexture();
				if (health <= 0)
				{
					alive = false;
					died();
				}
			}
			inline void speedChanged()
			{
				speed = normalSpeed * demageSpeedMultiplier * fatSpeedMultiplier * staminaSpeedMultiplier;
				if (speed > maxSpeed)
					speed = maxSpeed;
				else if (speed < 0)
					speed = 0;
			}
			inline void energyChanged()
			{
				if (energy > maxEnergy)
					energy = maxEnergy;
				else if (energy < 0)
					energy = 0;
			}
			inline void hungerChanged()
			{
				if (hunger > maxHunger)
					hunger = maxHunger;
				else if (hunger < 0)
					hunger = 0;

			}
			inline void thirstChanged()
			{
				if (thirst > maxThirst)
					thirst = maxThirst;
				else if (thirst < 0)
					thirst = 0;

			}
			inline void fatChanged()
			{
				if (fat > maxFat)
					fat = maxFat;
				else if (fat < 0)
					fat = 0;
				// Make animal slower when it is fat: min speed is 50% of normal speed
				fatSpeedMultiplier = 1 - fat / (maxFat*2);
				speedChanged();
				updateBodyTexture();
				if (health < 100)
					updateDemageTexture();
			}
			inline void reproductionChanged()
			{
				if (reproduction > maxReproduction)
					reproduction = maxReproduction;
				// reproduce();
			}

		private:
			float health;
			float maxHealth;
			float speed;
			float normalSpeed;
			float demageSpeedMultiplier;
			float fatSpeedMultiplier;
			float staminaSpeedMultiplier;
			float maxSpeed;
			float energy;
			float maxEnergy;
			float hunger;
			float maxHunger;
			float thirst;
			float maxThirst;
			float fat;
			float maxFat;
			float maxAge;
			float reproduction;
			float maxReproduction;

			float walkEnergyCost;


			bool alive;
			double lastMoveTimePoint;
			float walkedDistance;
			float walkedBackwardsDistance;
			float turnedDistance;
			float turnedLeftDistance;
			

			

			
			sf::Vector2f headPosition;
			
			QSFML::Utilities::AABB currentBodyDimension;
			std::array<QSFML::Utilities::AABB, TextureCoords::bodyCount> bodyDimensions;
			std::vector<sf::Vector2f> visionRays;
			struct Gizmos
			{
				sf::VertexArray visionVertexArray;
				sf::CircleShape head;
			};
			Gizmos gizmos;
			

			QSFML::Assets::TextureMap textureMap;
			sf::Sprite *bodySprite = nullptr;
			sf::Sprite *demageSprite = nullptr;
			QSFML::Components::Collider* bodyCollider = nullptr;

		};
	}
}