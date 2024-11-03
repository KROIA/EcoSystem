#include "Simulator/Creature/Animal/Controller/AiController.h"

namespace EcoSystem
{
	namespace Controller
	{
		AiController::AiController()
			: m_animal(nullptr)
			, m_geneticPerformance{ nullptr, 0.0f }
		{

		}

		AiController::~AiController()
		{

		}

		void AiController::updateAnimal()
		{
			// Make AI movements
		}

		NeuralNet::LearnAlgo::GeneticLearn::GeneticPerformance AiController::getGeneticPerformance()
		{
			m_geneticPerformance.fitness = getFitness();
			return m_geneticPerformance;
		}
	}
}