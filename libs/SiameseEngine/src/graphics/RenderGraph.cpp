#include "sepch.h"
#include "graphics/RenderGraph.h"

sengine::RenderGraph::RenderGraph(DeviceManager* deviceManager) :
	m_deviceManagerRef(deviceManager)
{
}

sengine::RenderGraph::~RenderGraph()
{
}

void sengine::RenderGraph::InitDefaultPasses()
{
}
