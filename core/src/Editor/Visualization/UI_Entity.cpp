#include "Editor/Visualization/UI_Entity.h"


UI_Entity::UI_Entity(QWidget *parent)
	: QWidget(parent)
	, m_updateTimer(nullptr)
{
	ui.setupUi(this);
}

UI_Entity::~UI_Entity()
{
	delete m_updateTimer;
}

void UI_Entity::setObject(EcoSystem::Entity* entity)
{
	if (!entity)
		return;
	m_entity = entity;


	if (m_updateTimer)
	{
		if (!m_entity)
			m_updateTimer->stop();
		else
			m_updateTimer->start();
	}
	readDataFromObject();
}
void UI_Entity::readDataFromObject()
{
	if (!m_entity)
	{
		clearData();
		return;
	}
	ui.name_label->setText(m_entity->getName().c_str());
}
void UI_Entity::clearData()
{
	ui.name_label->setText("");
}

void UI_Entity::setAutoUpdateData(bool enable)
{
	if (enable)
	{
		if (!m_updateTimer)
		{
			m_updateTimer = new QTimer(this);
			connect(m_updateTimer, &QTimer::timeout, this, &UI_Entity::onUpdateTimer);
		}
		m_updateTimer->setInterval(100);
		m_updateTimer->start();
	}
	else
	{
		if (m_updateTimer)
		{
			m_updateTimer->stop();
			delete m_updateTimer;
			m_updateTimer = nullptr;
		}
	}
}

void UI_Entity::onUpdateTimer()
{
	readDataFromObject();
}