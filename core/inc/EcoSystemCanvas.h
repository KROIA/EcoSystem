#pragma once

#include <QMainWindow>
#include "Editor/UI_EcoSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EcoSystemCanvas; }
QT_END_NAMESPACE

class EcoSystemCanvas : public QMainWindow
{
    Q_OBJECT

public:
    EcoSystemCanvas(QWidget* parent = nullptr);
    ~EcoSystemCanvas();

private:
    void closeEvent(QCloseEvent* event) override;


    Ui::EcoSystemCanvas* ui;
    UI_EcoSystem* m_ui_simulator;
    
    
};
