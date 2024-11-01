#include "Editor/UI_EcoSystem.h"
#include <QDebug>
#include "SaveSystem/ApplicationSettings.h"

using namespace QSFML;
using namespace QSFML::Objects;



namespace EcoSystem
{
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


    void UI_EcoSystem::loadAssets()
    {
		const SaveSystem::AssetSettings& assetSettings = SaveSystem::ApplicationSettings::getAssetSettings();
        std::string assetBasePath = assetSettings.m_assetPath.getValue().toString().toStdString();
        std::string texturePath = assetBasePath + "/"+ assetSettings.m_relativeTexturePath.getValue().toString().toStdString();
        Assets::TextureManager::loadTextures(texturePath);
    }
    void UI_EcoSystem::setupScene()
    {
        
        SceneSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300, 100);
        settings.contextSettings.antialiasingLevel = 0;
        settings.timing.frameTime = 0.02;
        //settings.updateControlls.enableMultithreading = false;
        //settings.updateControlls.enablePaintLoop = false;
        //settings.updateControlls.enableEventLoop = false;
        //settings.updateControlls.enableUpdateLoop = false;
        m_scene = new Scene(ui.scene_widget, settings);

		sf::Vector2u mapChunkCount(5, 5);

        DefaultEditor* defaultEditor = new DefaultEditor("Editor",sf::Vector2f(mapChunkCount * (unsigned)QSFML::Utilities::Chunk::CHUNK_SIZE));
        defaultEditor->getGrid()->setEnabled(false);
        defaultEditor->getCamera()->setMinZoom(0.1);
        defaultEditor->getCamera()->setMaxZoom(10);
        m_scene->addObject(defaultEditor);

		EcoSystem::Map* map = new EcoSystem::Map(mapChunkCount);
		m_scene->addObject(map);
        m_scene->start();
    }
}