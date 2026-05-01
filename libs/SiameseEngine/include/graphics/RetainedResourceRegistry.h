#pragma once

#include <nvrhi/d3d12.h>

#include "core/UUID.h"
#include "memory/SiameseAllocator.h"

namespace sengine
{
	class RetainedResouceRegistry
	{
	public:
		void ImportResource(const UUID& uuid, nvrhi::TextureHandle);
		nvrhi::TextureHandle Get(const UUID& uuid);
		void Clear();
	private:
		SUnorderedMap<UUID, nvrhi::TextureHandle, Systems::Render> m_resources{};
	};
}