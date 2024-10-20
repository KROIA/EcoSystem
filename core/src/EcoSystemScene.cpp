#include "EcoSystemScene.h"
#include "ui_ecoSystemScene.h"
#include <iostream>
#include <QCloseEvent>
#include "AppSettings.h"


using namespace QSFML;
using namespace QSFML::Objects;

EcoSystemScene::EcoSystemScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EcoSystemScene)
    , m_ui_simulator(EcoSystem::UI_EcoSystem::getInstance())
{
    ui->setupUi(this);

   

   
    ui->central_widget->layout()->addWidget(&m_ui_simulator);
}

EcoSystemScene::~EcoSystemScene()
{
    ui->central_widget->layout()->removeWidget(&m_ui_simulator);
    m_ui_simulator.setParent(nullptr);
    delete ui;
}
void EcoSystemScene::on_actionApplication_settings_triggered()
{
	static AppSettings::UI::UI_AppSettingsEditor settingsEditor;
	settingsEditor.show();
}
void EcoSystemScene::closeEvent(QCloseEvent* event)
{
    m_ui_simulator.stop();
    event->accept();
}

