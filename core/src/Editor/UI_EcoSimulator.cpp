#include "Editor/UI_EcoSystem.h"
#include <QDebug>
#include "SaveSystem/ApplicationSettings.h"

using namespace QSFML;
using namespace QSFML::Objects;



namespace EcoSystem
{

	void createEntitySelectFunctionality(QSFML::Scene* scene);
    void createAiSheepTrainer(QSFML::Scene* scene);

    UI_EcoSystem::UI_EcoSystem(QWidget* parent)
        : QWidget(parent)
    {
        ui.setupUi(this);
       

        m_scene = nullptr;

        m_ui_entity = new UI_Entity(this);
        ui.entity_frame->layout()->addWidget(m_ui_entity);

        SaveSystem::ApplicationSettings::getInstance().load();

        loadAssets();
        setupScene();
    }

    UI_EcoSystem::~UI_EcoSystem()
    {
		SaveSystem::ApplicationSettings::getInstance().save();
        delete m_scene;
    }
    UI_EcoSystem& UI_EcoSystem::getInstance()
    {
        static UI_EcoSystem inst;
        return inst;
    }

    void UI_EcoSystem::setSelectedEntity(EcoSystem::Entity* e)
    {
		UI_EcoSystem& instance = getInstance();
        if (!instance.m_ui_entity)
            return;
        instance.m_ui_entity->setObject(e);
    }
    void UI_EcoSystem::start()
    {
        UI_EcoSystem& instance = getInstance();
        if (instance.m_scene)
            instance.m_scene->start();
    }
    void UI_EcoSystem::stop()
    {
        UI_EcoSystem& instance = getInstance();
        if (instance.m_scene)
            instance.m_scene->stop();
    }

    void UI_EcoSystem::on_deltaT_slider_valueChanged(int value)
    {
		float deltaT = value / 100.0f;
        SceneSettings settings = m_scene->getSettings();
		settings.timing.physicsDeltaTScale = deltaT;
		m_scene->setSettings(settings);
    }
    void UI_EcoSystem::loadAssets()
    {
		const SaveSystem::AssetSettings& assetSettings = SaveSystem::ApplicationSettings::getAssetSettings();
        std::string assetBasePath = assetSettings.m_assetPath.getValue().toString().toStdString();
        std::string texturePath = assetBasePath + "/"+ assetSettings.m_relativeTexturePath.getValue().toString().toStdString();
        Assets::TextureManager::loadTextures(texturePath);
    }
    void UI_EcoSystem::setupScene()
    {
        Components::Transform::setGizmoAxisLength(0.5f);
        
        SceneSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300, 100);
        settings.contextSettings.antialiasingLevel = 0;
        settings.timing.frameTime = 0.00;
        //settings.updateControlls.enableMultithreading = false;
        //settings.updateControlls.enablePaintLoop = false;
        //settings.updateControlls.enableEventLoop = false;
        //settings.updateControlls.enableUpdateLoop = false;
		ui.deltaT_slider->setValue(static_cast<int>(settings.timing.physicsDeltaTScale * 100));
        m_scene = new Scene(ui.scene_widget, settings);

		sf::Vector2u mapChunkCount(16, 16);

        DefaultEditor* defaultEditor = new DefaultEditor("Editor",sf::Vector2f(mapChunkCount * (unsigned)QSFML::Utilities::Chunk::CHUNK_SIZE));
        defaultEditor->getGrid()->setEnabled(false);
        defaultEditor->getCamera()->setMinZoom(0.05);
        defaultEditor->getCamera()->setMaxZoom(10);
		

		
		
        m_scene->addObject(defaultEditor);
		createEntitySelectFunctionality(m_scene);

		EcoSystem::Map* map = new EcoSystem::Map(mapChunkCount);
        map->setRenderLayer(QSFML::RenderLayer::layer_0);
		m_scene->addObject(map);

        // Create sheep
		//EcoSystem::Animals::Sheep* sheep = new EcoSystem::Animals::Sheep();
		//sheep->setPosition(sf::Vector2f(32, 32));
        //sheep->setRenderLayer(QSFML::RenderLayer::layer_1);
		//m_scene->addObject(sheep);

		createAiSheepTrainer(m_scene);
        m_scene->start();
    }

	void createEntitySelectFunctionality(QSFML::Scene* scene)
	{
		static GameObject* controller = nullptr;
		if (controller)
			return;
		controller = new GameObject("AnimalController");
		controller->addUpdateFunction([](GameObject&)
			{
				Entity* selectedEntity = Entity::getSelected();
				if (!selectedEntity)
					return;
				Animals::Animal* animal = dynamic_cast<Animals::Animal*>(selectedEntity);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
					Entity::deselect();

				if (animal)
				{
					Controller::PlayerController::updateAnimal(animal);
					return;
				}
			});


		// Draw Animal info
		controller->addDrawFunction([](const GameObject&, sf::RenderTarget& target, sf::RenderStates states)
			{
				Entity* selectedEntity = Entity::getSelected();
				if (selectedEntity)
				{
                    states.transform = selectedEntity->getGlobalTransform();
					Animals::Animal* animal = dynamic_cast<Animals::Animal*>(selectedEntity);
					if (animal)
					{
						Controller::PlayerController::drawAnimal(animal, target, states);
						return;
					}
				}
			});
		controller->setRenderLayer(RenderLayer::layer_5);
		
		scene->addObject(controller);
	}

    void createAiSheepTrainer(QSFML::Scene* scene)
    {
        static GameObject* trainer = nullptr;
        struct Bundle
        {
            Animals::Sheep* sheep;
            Controller::AiSheepController* controller;
        };
        struct TrainerData
        {
            std::vector<Bundle> bundles;
			size_t iteration = 0;
            size_t step = 0;
            size_t sheepCount = 100;
            float averageScore = 0;
			GameObject* trainer = nullptr;
            bool showGizmos = false;

            std::vector<float> scores;
        };
		

        if (trainer)
            return;
        TrainerData* data = new TrainerData();
        NeuralNet::LearnAlgo::GeneticLearn::setCrossoverCountPercentage(50);
		NeuralNet::LearnAlgo::GeneticLearn::setMutationCountPercentage(10);
		NeuralNet::LearnAlgo::GeneticLearn::setMutationRate(0.005);

        scene->addObject(Controller::AiController::getObject());

        trainer = new GameObject("AiSheepTrainer");
		data->trainer = trainer;
        trainer->addUpdateFunction([data](GameObject& obj)
            {
				std::vector<Bundle>& bundles = data->bundles;

                if (bundles.size() == 0)
                {
                    bundles.reserve(data->sheepCount);
                    for (size_t i = 0; i < data->sheepCount; ++i)
                    {
                        Animals::Sheep* sheep = new Animals::Sheep("Sheep_" + std::to_string(i));
                        sheep->setPosition(sf::Vector2f(Map::getRandomPosition()));
						sheep->setRotation((float)(rand() % 360));
                        Controller::AiSheepController* controller = new Controller::AiSheepController(sheep);
                        Bundle bundle = { sheep, controller };
                        bundles.push_back(bundle);
                        sheep->enableDrawGizmosRecursive(data->showGizmos);
						obj.addChild(sheep);
                    }
					obj.updateObjectChanges();
                }

                for (size_t i = 0; i < data->sheepCount; ++i)
                {
                    Bundle& data = bundles[i];
                    data.controller->update();
                }
				data->step++;
            });
        
		trainer->addDrawFunction([data](const GameObject&, sf::RenderTarget&, sf::RenderStates)
			{
				// Draw info using ImGui
				ImGui::Begin("AiSheepTrainer");
				ImGui::Text("Generation: %d", data->iteration);
				ImGui::Text("Step: %d", data->step);
				ImGui::Text("Average score: %f", data->averageScore);

                static bool autoUpdate = true;
                
				ImGui::Checkbox("Auto update", &autoUpdate);
                if (ImGui::Checkbox("Show gizmos", &data->showGizmos))
                {
                    for (size_t i = 0; i < data->bundles.size(); ++i)
                    {
						data->bundles[i].sheep->enableDrawGizmosRecursive(data->showGizmos);
                    }
                }

				if (ImGui::Button("Next generation") || (autoUpdate && data->step > 1000))
				{
                    std::vector<Bundle>& bundles = data->bundles;
					float averageScore = 0;
					for (size_t i = 0; i < data->sheepCount; ++i)
					{
						averageScore += data->bundles[i].controller->getFitness();
					}
					averageScore /= (data->sheepCount * data->step);
					data->averageScore = averageScore;
                    data->scores.push_back(averageScore);
					data->step = 0;
                    data->iteration++;
                    std::vector< NeuralNet::LearnAlgo::GeneticLearn::GeneticPerformance> performances;
                    performances.reserve(data->sheepCount);
                    for (size_t i = 0; i < data->sheepCount; ++i)
                        performances.push_back(bundles[i].controller->getGeneticPerformance());

                    NeuralNet::LearnAlgo::GeneticLearn::learnAndReplace(performances);

                    Entity* selectedEntity = Entity::getSelected();
                    for (size_t i = 0; i < data->sheepCount; ++i)
                    {
                        if (selectedEntity == bundles[i].sheep)
                            Entity::deselect();
                        data->trainer->deleteChildLater(bundles[i].sheep);
                        bundles[i].sheep = new Animals::Sheep("Sheep_" + std::to_string(i));
                        bundles[i].sheep->enableDrawGizmosRecursive(data->showGizmos);
                        bundles[i].sheep->setRotation((float)(rand() % 360));
                        bundles[i].sheep->setPosition(Map::getRandomPosition());
                        bundles[i].controller->setAnimal(bundles[i].sheep);
                        data->trainer->addChild(bundles[i].sheep);
                    }
				}
				
				// Create a Plot using ImPlot that shows the score data->scores over time
				// Auto ajust the plot size to the window size
                ImVec2 plotSize = ImGui::GetContentRegionAvail();
                if (!data->scores.empty()) {
                    ImPlot::SetNextAxesLimits(0, (double)(data->scores.size() - 1), *std::min_element(data->scores.begin(), data->scores.end()),
                        *std::max_element(data->scores.begin(), data->scores.end()), ImGuiCond_Always);
                }
				if (ImPlot::BeginPlot("Scores", plotSize))
				{
					// Auto ajust x-axis
					ImPlot::PlotLine("Score", data->scores.data(), data->scores.size());
					ImPlot::EndPlot();
				}

                
                ImGui::End();

			});

		scene->addObject(trainer);
    }
}



