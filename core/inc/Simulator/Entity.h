#pragma once

#include "Simulator_base.h"
#include <QObject>

namespace EcoSystem
{
	class ECO_SYSTEM_EXPORT Entity: public QSFML::Objects::GameObject
	{
		class SelectorButton;
		friend SelectorButton;
		
	public:
		Entity(const std::string& name = "Entity",
			GameObject* parent = nullptr);
		Entity(const Entity& other);
		~Entity();

		OBJECT_DECL(Entity);
		static Log::LogObject& getLogger() { return s_log; }


		void selected();
		static void select(Entity* e);
		static void deselect();
		static Entity* getSelected() { return s_selectedEntity; }

		void attachSelectorButtonToCollider(bool doesAttach);

		//virtual void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const;

	protected:

		//static void setSelected(Entity* e);
		//static void deselect();

	
		void onClickButtonFallingEdge();
		void onClickButtonRisingEdge();

		void setSelectButtonSize(const sf::Vector2f& size)
		{
			m_selectButton->setSize(size);
		}

	private:
		class SelectorButton : public QSFML::Components::MousePressEvent
		{
		public:
			SelectorButton(Entity* parent, const std::string& name = "SelectorButton");
			SelectorButton(const SelectorButton& other);
			~SelectorButton();

			COMPONENT_DECL(SelectorButton);

			void setSize(const sf::Vector2f& size);
			const sf::Vector2f& getSize() const;

			void attachToCollider(bool doesAttach);
		private:
			bool getCurrentValue() override;

			void onFallingEdge() override;
			void onDown() override;
			void onRisingEdge() override;

			Entity* m_parent;
			sf::Vector2f m_buttonSize;
			bool m_wasInside;
			bool m_wasPressed;
			bool m_attachedToCollider;
		};
		SelectorButton* m_selectButton;

		static Entity* s_selectedEntity;
		static Log::LogObject s_log;
	};
}
