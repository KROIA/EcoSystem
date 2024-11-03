#pragma once

#include "EcoSystem_base.h"
#include "NeuralNet.h"
#include "Simulator/Creature/Animal/Animal.h"

namespace EcoSystem
{
	namespace Controller
	{
		class ECO_SYSTEM_EXPORT AiController
		{
		public:
			AiController();
			virtual ~AiController();

			void updateAnimal();

			NeuralNet::CustomConnectedNeuralNet* getNeuralNet() { return m_geneticPerformance.net; }
			void setNeuralNet(NeuralNet::CustomConnectedNeuralNet* net) { m_geneticPerformance.net = net; }
			NeuralNet::LearnAlgo::GeneticLearn::GeneticPerformance getGeneticPerformance();

			void addFitness(float fitnessDelta) { m_geneticPerformance.fitness += fitnessDelta; }

			Animals::Animal::SensoryData getSensoryData() const
			{
				if (m_animal)
					return m_animal->getSensoryData();
				Animals::Animal::SensoryData data;
				return data;
			}

			void setAnimal(Animals::Animal* animal) { m_animal = animal; }
			Animals::Animal* getAnimal() { return m_animal; }
			
			virtual void update() = 0;
			virtual float getFitness() = 0;
		protected:
			
		private:
			Animals::Animal* m_animal;
			NeuralNet::LearnAlgo::GeneticLearn::GeneticPerformance m_geneticPerformance;
		};

	}
}