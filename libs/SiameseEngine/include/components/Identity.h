#pragma once
#include "core/UUID.h"

namespace sengine
{
	//contains what an entity is, such as a name, tag, layer and whether it is active or not
	struct Identity
	{
		bool m_active{ true };
		uint32_t m_layer{ 0xffffffff };
		char m_name[32]{ 0 };
		char m_tag[32]{ 0 };
		UUID uuid{};
	};
}