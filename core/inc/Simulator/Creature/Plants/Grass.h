#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Entity.h"

namespace EcoSystem
{
	class ECO_SYSTEM_EXPORT Grass : public Entity
	{
	public:
		Grass(const std::string& name = "Grass");
		Grass(const Grass& other);
		~Grass();

		OBJECT_DECL(Grass);

	private:
		QSFML::Components::TextureAnimation* m_animation = nullptr;
		sf::Sprite *m_sprite = nullptr;

		
	};
}