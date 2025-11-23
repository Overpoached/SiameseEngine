#pragma once

#include "core/Log.h"
#include "core/Core.h"

//number of blocks per chunk
#define SE_MEM_BLOCK_COUNT 32

namespace sengine
{
	struct BlockHeader
	{
		uint32_t magic = 0xDEADBEEF;
		BlockHeader* m_next{};
	};

	template<uint32_t BlockSize, uint32_t BlockCount>
	struct MemoryChunk
	{
		static_assert(BlockSize > sizeof(BlockHeader), "Block size must be larger than BlockHeader size");
		static constexpr uint32_t s_blockSize{ BlockSize };
		static constexpr uint32_t s_blockCount{ BlockCount };

		MemoryChunk()
		{
			m_memory = new uint8_t[BlockSize * BlockCount];
#ifdef SE_DEBUG
			SENGINE_DEBUG("Allocated MemoryChunk<BlockSize={}, BlockCount={}> at address {}", BlockSize, BlockCount, reinterpret_cast<void*>(m_memory));
#endif
			for (size_t i = 0; i < BlockCount; ++i)
			{
				BlockHeader* header = reinterpret_cast<BlockHeader*>(m_memory + i * BlockSize);
				if (i < BlockCount - 1)
				{
					header->m_next = reinterpret_cast<BlockHeader*>(m_memory + (i + 1) * BlockSize);
				}
				else
				{
					header->m_next = nullptr;
				}
			}
		}

		~MemoryChunk()
		{
			if (m_memory)
			{
				delete[] m_memory;
				m_memory = nullptr;
			}
		}

		uint8_t* m_memory{ nullptr };
	};

#ifdef SE_DEBUG
	struct MemoryPoolDebugInfo
	{
		struct MemoryUseInfo
		{
			size_t allocationCount{};
			size_t totalAllocatedSize{};
		};
		std::unordered_map<Systems, MemoryUseInfo> systemMemoryUsage;
		uint32_t chunksAllocated{ 0 };
		uint32_t totalMemoryAllocated{ 0 };
		uint32_t totalMemoryInUse{ 0 };
		uint32_t totalBlocksAllocated{ 0 };
		uint32_t totalBlocksInUse{ 0 };
	};
#endif

	template<uint32_t BlockSize>
	class MemoryPool
	{
		static constexpr uint32_t s_blockSize{ BlockSize };

		std::list<MemoryChunk<BlockSize, SE_MEM_BLOCK_COUNT>> m_chunks{};
		BlockHeader* m_freeList{ nullptr };

#ifdef SE_DEBUG
		MemoryPoolDebugInfo m_debugInfo;
#endif

		void AllocateNewChunk()
		{
			m_chunks.emplace_back();
			m_freeList = reinterpret_cast<BlockHeader*>(m_chunks.back().m_memory);
#ifdef SE_DEBUG
			m_debugInfo.chunksAllocated++;
			m_debugInfo.totalMemoryAllocated += s_blockSize * SE_MEM_BLOCK_COUNT;
			m_debugInfo.totalBlocksAllocated += SE_MEM_BLOCK_COUNT;
#endif
		};

	public:
		~MemoryPool()
		{
			m_chunks.clear();
		}

		template<typename T, Systems sys = Systems::Undefined>
		T* Allocate()
		{
			if (!m_freeList)
			{
				//need to allocate a new chunk
				AllocateNewChunk();
			}
			T* ret = reinterpret_cast<T*>(m_freeList);
			m_freeList = m_freeList->m_next;
#ifdef SE_DEBUG
#if SE_MEMORY_DEBUG_LOG
			SENGINE_DEBUG("Allocated a block from MemoryPool<BlockSize={}> for {}", s_blockSize, typeid(T).name());
#endif
			//add debug info
			m_debugInfo.totalBlocksInUse++;
			m_debugInfo.totalMemoryInUse += s_blockSize;
			auto& usageInfo = m_debugInfo.systemMemoryUsage[sys];
			usageInfo.allocationCount++;
			usageInfo.totalAllocatedSize += s_blockSize;
#endif
			return ret;
		}

		template<typename T, Systems sys = Systems::Undefined>
		bool TryDeallocate(T* ptr)
		{
			char* p = reinterpret_cast<char*>(ptr);
			for (auto& chunk : m_chunks)
			{
				char* chunkStart = reinterpret_cast<char*>(chunk.m_memory);
				char* chunkEnd = chunkStart + (s_blockSize * SE_MEM_BLOCK_COUNT);
				if (p >= chunkStart && p < chunkEnd)
				{
					reinterpret_cast<BlockHeader*>(ptr)->m_next = m_freeList;
					m_freeList = reinterpret_cast<BlockHeader*>(p);
#ifdef SE_DEBUG
					//debug info
#if SE_MEMORY_DEBUG_LOG
					SENGINE_DEBUG("Deallocated a block from MemoryPool<BlockSize={}> for {}", s_blockSize, typeid(T).name());
#endif
					m_debugInfo.totalBlocksInUse--;
					m_debugInfo.totalMemoryInUse -= s_blockSize;
					auto& usageInfo = m_debugInfo.systemMemoryUsage[sys];
					usageInfo.allocationCount--;
					usageInfo.totalAllocatedSize -= s_blockSize;
#endif
					return true;
				}
			}
			return false;
		}

#ifdef SE_DEBUG
		//dump all the memory debug info
		void Dump()
		{
			SENGINE_DEBUG("MemoryPool<BlockSize={}> Debug Info:", s_blockSize);
			SENGINE_DEBUG("  Chunks Allocated: {}", m_debugInfo.chunksAllocated);
			SENGINE_DEBUG("  Total Memory Allocated: {} bytes", m_debugInfo.totalMemoryAllocated);
			SENGINE_DEBUG("  Total Memory In Use: {} bytes", m_debugInfo.totalMemoryInUse);
			SENGINE_DEBUG("  Total Blocks Allocated: {}", m_debugInfo.totalBlocksAllocated);
			SENGINE_DEBUG("  Total Blocks In Use: {}", m_debugInfo.totalBlocksInUse);
			SENGINE_DEBUG("  System Memory Usage:");
			for (const auto& [system, usage] : m_debugInfo.systemMemoryUsage)
			{
				SENGINE_DEBUG("    System: {}, Allocation Count: {}, Total Allocated Size: {} bytes",
					SystemNames[static_cast<size_t>(system)],
					usage.allocationCount,
					usage.totalAllocatedSize);
			}
		}
#endif
	};
}
