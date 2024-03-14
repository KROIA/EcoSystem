#pragma once

#include "Editor/Editor_base.h"
#include <QWidget>
#include <QTimer>

#include "ui_UI_Entity.h"
#include "Simulator/Entity.h"


class ECO_SYSTEM_EXPORT UI_Entity : public QWidget
{
	Q_OBJECT

public:
	UI_Entity(QWidget *parent = nullptr);
	~UI_Entity();

	void setObject(EcoSystem::Entity* entity);
	void readDataFromObject();
	void clearData();

	void setAutoUpdateData(bool enable);

private slots:
	void onUpdateTimer();
private:

	QTimer* m_updateTimer;

	Ui::UI_Entity ui;

	EcoSystem::Entity* m_entity;
};
