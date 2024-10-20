#pragma once

#include <QMainWindow>
#include "Editor/UI_EcoSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EcoSystemScene; }
QT_END_NAMESPACE

class EcoSystemScene : public QMainWindow
{
    Q_OBJECT

public:
    EcoSystemScene(QWidget* parent = nullptr);
    ~EcoSystemScene();

private slots:
	void on_actionApplication_settings_triggered();

private:
    void closeEvent(QCloseEvent* event) override;


    Ui::EcoSystemScene* ui;
    EcoSystem::UI_EcoSystem& m_ui_simulator;
    
    
};
