#include "EcoSystemCanvas.h"
#include "ui_ecoSystemCanvas.h"
#include <iostream>
#include <QCloseEvent>



using namespace QSFML;
using namespace QSFML::Objects;

EcoSystemCanvas::EcoSystemCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EcoSystemCanvas)
{
    ui->setupUi(this);
    //m_canvas = nullptr;

    m_ui_simulator = new UI_EcoSystem(this);
    ui->central_widget->layout()->addWidget(m_ui_simulator);
}

EcoSystemCanvas::~EcoSystemCanvas()
{
    delete ui;
}
void EcoSystemCanvas::closeEvent(QCloseEvent* event)
{
    m_ui_simulator->stop();
    event->accept();
}

