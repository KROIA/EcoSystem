#include "Simulator/Entity.h"
#include "Editor/UI_EcoSystem.h"

namespace EcoSystem
{
	Entity* Entity::s_selectedEntity = nullptr;
	Log::LogObject Entity::s_log(EcoSystem::Logger::getID(), "Entity");

	Entity::Entity(const std::string& name, GameObject* parent)
		: GameObject(name, parent)
	{
		m_selectButton = new SelectorButton(this);
		m_selectButton->setSize(sf::Vector2f(0, 0));
		addComponent(m_selectButton);
	}
	Entity::Entity(const Entity& other)
		: GameObject(other)
	{
		m_selectButton = new SelectorButton(this);
		addComponent(m_selectButton);
	}
	Entity::~Entity()
	{

	}

	OBJECT_IMPL(Entity);

	void Entity::selected()
	{
		select(this);		
	}
	void Entity::select(Entity* e)
	{
		if (!e || e == s_selectedEntity) [[unlikely]]
			return;
		s_log.logInfo("Entity selected: " + e->getName());
		if (s_selectedEntity)
		{
			deselect();
		}
		s_selectedEntity = e;
		UI_EcoSystem::setSelectedEntity(s_selectedEntity);
	}
	void Entity::deselect()
	{
		if(s_selectedEntity)
			s_log.logInfo("Entity deselected: " + s_selectedEntity->getName());
		s_selectedEntity = nullptr;
		UI_EcoSystem::setSelectedEntity(s_selectedEntity);
	}

	void Entity::attachSelectorButtonToCollider(bool doesAttach)
	{
		m_selectButton->attachToCollider(doesAttach);
	}

	void Entity::drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw rectangle around the entity
		QSFML::Utilities::AABB box = getBoundingBox();
		sf::RectangleShape rect(box.getSize());
		rect.setPosition(box.getPos());
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(1);
		target.draw(rect, states);
	}

	void Entity::onClickButtonFallingEdge()
	{
		selected();
	}
	void Entity::onClickButtonRisingEdge()
	{

	}



	Entity::SelectorButton::SelectorButton(Entity* parent, const std::string& name)
		: MousePressEvent(name)
		, m_parent(parent)
		, m_buttonSize(1, 1)
		, m_wasInside(false)
		, m_wasPressed(false)
		, m_attachedToCollider(false)
	{

	}
	Entity::SelectorButton::SelectorButton(const SelectorButton& other)
		: MousePressEvent(other)
		, m_parent(nullptr)
		, m_buttonSize(other.m_buttonSize)
		, m_wasInside(other.m_wasInside)
		, m_wasPressed(other.m_wasPressed)
		, m_attachedToCollider(other.m_attachedToCollider)
	{

	}
	Entity::SelectorButton::~SelectorButton()
	{

	}
	COMPONENT_IMPL(Entity::SelectorButton);

	void Entity::SelectorButton::setSize(const sf::Vector2f& size)
	{
		m_buttonSize = size;
	}
	const sf::Vector2f& Entity::SelectorButton::getSize() const
	{
		return m_buttonSize;
	}
	void Entity::SelectorButton::attachToCollider(bool doesAttach)
	{
		m_attachedToCollider = doesAttach;
	}

	bool Entity::SelectorButton::getCurrentValue()
	{
		if (!m_parent)
			return false;
		bool isPressed = sf::Mouse::isButtonPressed(getTriggerButton());
		sf::Vector2f pos = getParent()->getMouseWorldPosition();

		QSFML::Utilities::AABB box(getParent()->getGlobalPosition(), m_buttonSize);
		if (m_attachedToCollider)
			box = m_parent->getBoundingBox();
		bool isInside = box.contains(pos);

		if (isPressed && m_wasInside && isInside)
		{
			m_wasInside = isInside;
			m_wasPressed = isPressed;
			return true;
		}

		m_wasInside = isInside;
		m_wasPressed = isPressed;

		return false;
	}


	void Entity::SelectorButton::onFallingEdge()
	{
		m_parent->onClickButtonFallingEdge();
	}
	void Entity::SelectorButton::onDown()
	{
		
	}
	void Entity::SelectorButton::onRisingEdge()
	{
		m_parent->onClickButtonRisingEdge();
	}
}
