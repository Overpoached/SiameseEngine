#pragma once

#include <ml.h>

namespace sengine
{
	//contains the typical transform information for an entity, such as position, scale, rotation and the model matrix
	//double buffered component system already has the previous frames values
	struct Transform
	{
		float3 m_localPosition{ 0.f, 0.f, 0.f };
		float3 m_localScale{ 1.f, 1.f, 1.f };
		float3 m_localRotation{ 0.f, 0.f, 0.f };

		float4x4 m_modelMatrix{ float4x4::Identity() };
	};
}