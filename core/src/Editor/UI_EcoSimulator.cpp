#include "Editor/UI_EcoSystem.h"
#include <QDebug>

using namespace QSFML;
using namespace QSFML::Objects;


UI_EcoSystem* UI_EcoSystem::s_instance = nullptr;


UI_EcoSystem::UI_EcoSystem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    s_instance = this;

	m_scene = nullptr;

	m_ui_entity = new UI_Entity(this);
	ui.entity_frame->layout()->addWidget(m_ui_entity);

	setupScene();
}

UI_EcoSystem::~UI_EcoSystem()
{
    delete m_scene;
}

void UI_EcoSystem::setSelectedEntity(EcoSystem::Entity* e)
{
    if (!s_instance)
        return;
    if (!s_instance->m_ui_entity)
        return;
    s_instance->m_ui_entity->setObject(e);
}

void UI_EcoSystem::stop()
{
    if (m_scene)
        m_scene->stop();
}

void UI_EcoSystem::setupScene()
{
    SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_scene = new Scene(ui.scene_widget, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();
    m_scene->addObject(defaultEditor);
    qDebug() << defaultEditor->toString().c_str();


    EcoSystem::Entity* e = new EcoSystem::Entity();
    m_scene->addObject(e);

    m_scene->start();
}
