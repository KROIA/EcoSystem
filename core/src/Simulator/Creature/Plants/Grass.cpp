#include "Simulator/Creature/Plants/Grass.h"

namespace EcoSystem
{
	Grass::Grass(const std::string& name)
		: Entity(name)
	{
		m_sprite = new sf::Sprite();
		m_animation = new QSFML::Components::TextureAnimation(QSFML::Assets::TextureManager::getTexture("Grass.png"), {4,1});
		m_animation->bindTo(m_sprite);

		QSFML::Components::TextureAnimation::Animation grow("grow", { { 0, 0.5f},{ 1, 0.5f}, { 2, 0.5f}, { 3, 0.5f} });

		m_animation->addAnimation(grow);
		m_animation->setRepeating(true);
		m_animation->start("grow");


		add(m_sprite);
		add(m_animation);

		setCustomBoundingBoxFunction([this]() -> QSFML::Utilities::AABB
			{
				sf::Vector2u size = m_sprite->getTexture()->getSize();
				sf::Vector2f scale = m_sprite->getScale();
				sf::Vector2f sizef(size.x * scale.x, size.y * scale.y);
				QSFML::Utilities::AABB aabb(getPosition(), sizef);
				return aabb;
			});
		attachSelectorButtonToCollider(true);
	}

	Grass::Grass(const Grass& other)
		: Entity(other)
	{
	}

	Grass::~Grass()
	{
	}

	OBJECT_IMPL(Grass);
}