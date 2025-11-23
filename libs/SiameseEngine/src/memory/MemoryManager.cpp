#include "sepch.h"

#include "core/Log.h"

#include "memory/MemoryManager.h"

std::unique_ptr<sengine::MemoryManager> sengine::MemoryManager::s_instance;

sengine::MemoryManager::~MemoryManager()
{
	Release();
}

sengine::MemoryManager& sengine::MemoryManager::Get()
{
	if (!s_instance)
		s_instance = std::make_unique<MemoryManager>();
	return *s_instance;
}

void sengine::MemoryManager::Init()
{

}

void sengine::MemoryManager::Release()
{
	for(auto& it : m_allocatedBlocks)
	{
		delete[] it;
	}
	s_instance.reset();
}

void sengine::MemoryManager::Dump()
{
	m_pool_64.Dump();
	m_pool_128.Dump();
	m_pool_256.Dump();
	m_pool_512.Dump();
	m_pool_1024.Dump();
	m_pool_2048.Dump();
	m_pool_4096.Dump();
	m_pool_8192.Dump();
	m_pool_16384.Dump();
	m_pool_32768.Dump();
	m_pool_65536.Dump();
	m_pool_131072.Dump();
}
