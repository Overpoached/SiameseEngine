#pragma once
#include <nvrhi/d3d12.h>

#include "memory/SiameseAllocator.h"
#include "core/UUID.h"

namespace sengine
{
	class DeviceManager;
	class IRenderPass;
	class RetainedResourceRegistry;
	class RenderGraph
	{
	public:
		RenderGraph(DeviceManager* deviceManager);
		~RenderGraph();

		void Compile();
		void Execute();
		//helper to create default render passes used by engine, if user wishes to write their own they can try
		void InitDefaultPasses();
		void AddPass(SSharedPtr<IRenderPass> pass);

	private:
		DeviceManager* m_deviceManagerRef{};
	};
}