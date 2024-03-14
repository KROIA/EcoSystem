#pragma once

#include "Editor_base.h"
#include <QWidget>
#include "QSFML_EditorWidget.h"


#include "ui_UI_EcoSystem.h"
#include "Editor/Visualization/UI_Entity.h"


#include "Simulator/Simulator_incs.h"

class ECO_SYSTEM_EXPORT_EXPORT UI_EcoSystem : public QWidget
{
	Q_OBJECT

public:
	UI_EcoSystem(QWidget *parent = nullptr);
	~UI_EcoSystem();


	static void setSelectedEntity(EcoSystem::Entity* e);

	void stop();

private:
	void setupCanvas();


	Ui::UI_EcoSystem ui;

	QSFML::Canvas* m_canvas;
	UI_Entity* m_ui_entity;

	static UI_EcoSystem* s_instance;
};
