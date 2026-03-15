#pragma once

#include <entt/entity/registry.hpp>

#include "memory/SiameseAllocator.h"

namespace sengine
{
	class EntityManager
	{
	public:


	private:
		entt::basic_registry<entt::entity, SiameseAllocator<entt::entity>> m_registry{};

	};
}