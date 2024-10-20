#include "EcoSystemScene.h"
#include "ui_ecoSystemScene.h"
#include <iostream>
#include <QCloseEvent>



using namespace QSFML;
using namespace QSFML::Objects;

EcoSystemScene::EcoSystemScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EcoSystemScene)
{
    ui->setupUi(this);
    //m_scene = nullptr;

    m_ui_simulator = new UI_EcoSystem(this);
    ui->central_widget->layout()->addWidget(m_ui_simulator);
}

EcoSystemScene::~EcoSystemScene()
{
    delete ui;
}
void EcoSystemScene::closeEvent(QCloseEvent* event)
{
    m_ui_simulator->stop();
    event->accept();
}

