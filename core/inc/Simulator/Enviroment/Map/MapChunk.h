#pragma once

#include "EcoSystem_base.h"
#include "Simulator/Entity.h"

namespace EcoSystem
{
	class ECO_SYSTEM_EXPORT MapChunk : public QSFML::Utilities::Chunk
	{
	public:
		MapChunk(const sf::Vector2i& pos, const Resources& resources, float scale);

	private:
		void onGenerate() override;
	};

	
}