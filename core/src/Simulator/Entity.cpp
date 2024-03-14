#include "Simulator/Entity.h"
#include "Editor/UI_EcoSystem.h"

namespace EcoSystem
{
	Entity* Entity::s_selectedEntity = nullptr;

	Entity::Entity(const std::string& name, CanvasObject* parent)
		: CanvasObject(name, parent)
	{
		m_selectButton = new SelectorButton(this);
		m_selectButton->setSize(sf::Vector2f(50, 50));
		addComponent(m_selectButton);
	}
	Entity::Entity(const CanvasObject& other)
		: CanvasObject(other)
	{
		m_selectButton = new SelectorButton(this);
		addComponent(m_selectButton);
	}
	Entity::~Entity()
	{

	}

	CLONE_FUNC_IMPL(Entity);

	void Entity::selected()
	{
		if (s_selectedEntity)
		{
			if (s_selectedEntity == this)
				return;
			s_selectedEntity->deselect();
		}
		s_selectedEntity = this;

		UI_EcoSystem::setSelectedEntity(s_selectedEntity);
	}
	void Entity::deselect()
	{
		s_selectedEntity = nullptr;
		UI_EcoSystem::setSelectedEntity(s_selectedEntity);
	}

	void Entity::attachSelectorButtonToCollider(bool doesAttach)
	{
		m_selectButton->attachToCollider(doesAttach);
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

		QSFML::Utilities::AABB box(pos, m_buttonSize);
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
