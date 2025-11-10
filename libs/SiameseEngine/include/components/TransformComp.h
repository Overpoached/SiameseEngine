#pragma once

#include <ml.h>

namespace sengine
{
	struct TransformComponent
	{
		float3 localPosition{ 0.f, 0.f, 0.f };
		float3 localScale{ 1.f, 1.f, 1.f };
		float3 localRotation{ 0.f, 0.f, 0.f };

		float4x4 modelMatrix;
		float4x4 prevModelMatrix;
	};
}