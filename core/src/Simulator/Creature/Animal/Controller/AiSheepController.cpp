#include "Simulator/Creature/Animal/Controller/AiSheepController.h"

namespace EcoSystem
{
	namespace Controller
	{
		Log::LogObject& AiSheepController::getLogger()
		{
			static Log::LogObject logger(Logger::getID(), "AiSheepController");
			return logger;
		}
		AiSheepController::AiSheepController(Animals::Sheep* sheep)
			: AiController()
		{
			if (sheep == nullptr)
			{
				getLogger().logError("Sheep is nullptr");
				return;
			}

			setAnimal(sheep);
			
			std::vector<float> inputData = getNNInputData();
			unsigned int inputCount = inputData.size();
			unsigned int hiddenLayerCount = 1;
			unsigned int hiddenLayerSize = 20;
			unsigned int outputCount = 5;

			std::vector < NeuralNet::Neuron::ID> inputIDs(inputCount, 0);
			std::vector < NeuralNet::Neuron::ID> outputIDs(outputCount, 0);
			for (size_t i = 0; i < inputIDs.size(); ++i)
				inputIDs[i] = i;
			for (size_t i = 0; i < outputIDs.size(); ++i)
				outputIDs[i] = inputIDs.size() + i;

			NeuralNet::FullConnectedNeuralNet* net = new NeuralNet::FullConnectedNeuralNet(inputIDs, hiddenLayerCount, hiddenLayerSize, outputIDs);
			net->setActivationType(NeuralNet::Activation::Type::tanh_);
			setNeuralNet(net);
			
		}

		AiSheepController::~AiSheepController()
		{
		}

		void AiSheepController::update()
		{
			std::vector<float> inputData = getNNInputData();
			NeuralNet::CustomConnectedNeuralNet* net = getNeuralNet();
			net->setInputValues(inputData);
			net->update();
			std::vector<float> output = net->getOutputValues();
			processOutput(output);			
		}
		float AiSheepController::getFitness()
		{
			Animals::Animal* animal = getAnimal();
			if (!animal)
			{
				getLogger().logError("Animal is nullptr");
				return 0;
			}
			bool alive = animal->isAlive();
			if (!alive)
				return 0.1;
			float forwardDistance = animal->getWalkedDistance();
			float backwardDistance = animal->getWalkedBackwardsDistance();
			float turned = animal->getTurnedDistance()/3600.f;
			//float leftTurned = animal->getTurnedLeftDistance();
			float fat = animal->getFat();
			float health = animal->getHealth();
			
			return	(forwardDistance - backwardDistance)*0.01 / (turned + 1)+
					fat*10 + 
					health*10;
		}
		std::vector<float> AiSheepController::getNNInputData()
		{
			Animals::Animal::SensoryData sensoryData = getSensoryData();
			float noise = 0.1;

			static constexpr unsigned int internalInputCount = 12;
			static constexpr unsigned int tileSensitivityCount = 2;
			const unsigned int visionRayCount = sensoryData.enviromental.visionPoints.size();
			std::vector<float> inputData(internalInputCount + tileSensitivityCount * visionRayCount);
			size_t index = 0;
			inputData[index++] = sensoryData.internal.health;
			inputData[index++] = sensoryData.internal.speed;
			inputData[index++] = sensoryData.internal.demageSpeedMultiplier;
			inputData[index++] = sensoryData.internal.fatSpeedMultiplier;
			inputData[index++] = sensoryData.internal.staminaSpeedMultiplier;
			inputData[index++] = sensoryData.internal.energy;
			inputData[index++] = sensoryData.internal.hunger;
			inputData[index++] = sensoryData.internal.thirst;
			inputData[index++] = sensoryData.internal.fat;
			inputData[index++] = sensoryData.internal.age/100;
			inputData[index++] = sensoryData.internal.reproduction;
			inputData[index++] = sensoryData.internal.alive;


			for (size_t i = 0; i < visionRayCount; ++i)
			{
				const MapChunkData::MapTile& mapTile = Map::getTile(sensoryData.enviromental.visionPoints[i]);
				float tileDetector[tileSensitivityCount] = { 0 };
				switch (mapTile.type)
				{
				case MapTileType::Grass:
					tileDetector[0] = mapTile.data.grass.growth;
					break;
				case MapTileType::Water:
					tileDetector[1] = mapTile.data.water.depth;
					break;
				}
				inputData[index++] = tileDetector[0];
				inputData[index++] = tileDetector[1];
			}

			for (size_t i = 0; i < inputData.size(); ++i)
				inputData[i] += (rand() % 100) / 100.f * noise - noise / 2.f;

			return inputData;
		}
		void AiSheepController::processOutput(const std::vector<float>& output)
		{
			Animals::Animal* animal = getAnimal();
			if(!animal)
			{
				getLogger().logError("Animal is nullptr");
				return;
			}
			float moveForward = output[0];
			float turnLeft = output[1];
			float turnRight = output[2];
			float eat = output[3];
			float drink = output[4];

			if (moveForward > 0.1 || moveForward < -0.1)
				animal->tryWalkForward(moveForward);
			if (turnLeft > 0.1 && turnRight < 0.1)
				animal->tryTurnLeft(turnLeft);
			else if (turnRight > 0.1 && turnLeft < 0.1)
				animal->tryTurnRight(turnRight);
			if (eat > 0.5)
				animal->tryEat();
			if (drink > 0.5)
				animal->tryDrink();
		}

		
	}
}