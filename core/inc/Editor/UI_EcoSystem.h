#pragma once

#include "Editor_base.h"
#include <QWidget>
#include "QSFML_EditorWidget.h"


#include "ui_UI_EcoSystem.h"
#include "Editor/Visualization/UI_Entity.h"


#include "Simulator/Simulator_incs.h"

namespace EcoSystem
{
	class ECO_SYSTEM_EXPORT UI_EcoSystem : public QWidget
	{
		Q_OBJECT
		
	public:
		UI_EcoSystem(QWidget* parent = nullptr);
		~UI_EcoSystem();
		static UI_EcoSystem& getInstance();


		static void setSelectedEntity(EcoSystem::Entity* e);

		static void start();
		static void stop();

	private:
		void loadAssets();
		void setupScene();


		Ui::UI_EcoSystem ui;

		QSFML::Scene* m_scene;
		UI_Entity* m_ui_entity;

	};
}
