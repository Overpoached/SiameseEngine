#pragma once

#include "memory/SiameseAllocator.h"

namespace sengine
{
	class RenderGraph;
	class IRenderPass
	{
		virtual ~IRenderPass() = default;
		//for the render pass to request for resources, can get from retained or transient resource
		virtual void Compile(SSharedPtr<RenderGraph> renderGraph) = 0;
		virtual void Execute() = 0;
	};
}