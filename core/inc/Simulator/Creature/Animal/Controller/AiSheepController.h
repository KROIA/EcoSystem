#pragma once

#include "EcoSystem_base.h"
#include "AiController.h"
#include "Simulator/Creature/Animal/Sheep.h"

namespace EcoSystem
{
	namespace Controller
	{
		class ECO_SYSTEM_EXPORT AiSheepController : public AiController
		{
		public:
			static Log::LogObject& getLogger();
			AiSheepController(Animals::Sheep* sheep);
			~AiSheepController();

			void update() override;
			float getFitness() override;
		private:
			
			std::vector<float> getNNInputData();
			void processOutput(const std::vector<float>& output);

			NeuralNet::Visualisation::CustomConnectedNeuralNetPainter *m_visu = nullptr;
		};
	}
}