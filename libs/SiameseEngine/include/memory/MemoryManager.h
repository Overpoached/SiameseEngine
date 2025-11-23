#pragma once

#include "core/Core.h"
#include "core/Log.h"

#include "MemoryPool.h"

namespace sengine
{
	/**
	 * \brief Memory manager that contains multiple linked list of varying types
	 * Memory is allocated on the heap
	 */
	class MemoryManager
	{
	public:
		/**
		 * \brief Destructor
		 */
		~MemoryManager();
		/**
		 * \brief Gets the reference to the static instance
		 * \return The instance
		 */
		static MemoryManager& Get();
		/**
		 * \brief Initializes the memory manager
		 */
		void Init();
		/**
		 * \brief Releases the memory allocated by the memory manager
		 */
		void Release();
		/**
		 * \brief Dumps the information in text for the memory being in used
		 */
		void Dump();

		/**
		 * \brief Allocates memory for a object or a group of objects
		 * \tparam T The object type
		 * \param count The number of objects to allocate for
		 * \return The pointer to the memory block
		 */
		template<typename T, Systems sys = Systems::Undefined>
		[[nodiscard]] T* Allocate(const size_t& count = 1)
		{
			static size_t size = count * sizeof(T);
			if (size <= 64)
				return m_pool_64.Allocate<T, sys>();
			else if (size <= 128)
				return m_pool_128.Allocate<T, sys>();
			else if (size <= 256)
				return m_pool_256.Allocate<T, sys>();
			else if (size <= 512)
				return m_pool_512.Allocate<T, sys>();
			else if (size <= 1024)
				return m_pool_1024.Allocate<T, sys>();
			else if (size <= 2048)
				return m_pool_2048.Allocate<T, sys>();
			else if (size <= 4096)
				return m_pool_4096.Allocate<T, sys>();
			else if (size <= 8192)
				return m_pool_8192.Allocate<T, sys>();
			else if (size <= 16384)
				return m_pool_16384.Allocate<T, sys>();
			else if (size <= 32768)
				return m_pool_32768.Allocate<T, sys>();
			else if (size <= 65536)
				return m_pool_65536.Allocate<T, sys>();
			else if (size <= 131072)
				return m_pool_131072.Allocate<T, sys>();
#ifdef SE_DEBUG
#if SE_MEMORY_DEBUG_LOG
			SENGINE_DEBUG("Allocated memory of size {} using normal new for {}", size, typeid(T).name());
#endif
#endif
			//block size exceed maximum pool size, allocate a normal block
			uint8_t* rawMemory = new uint8_t[size];
			m_allocatedBlocks.push_back(rawMemory);
			return reinterpret_cast<T*>(rawMemory);
		}
		/**
		 * \brief Deallocate a pointer and add it to the freelist
		 * \tparam T The type of the block being deallocated
		 * \param p The pointer to the block
		 */
		template<typename T, Systems sys = Systems::Undefined>
		void Deallocate(T* p)
		{
			if (!p)
				return;
			static constexpr size_t size = sizeof(T);
			if constexpr (size <= 64)
				if (m_pool_64.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 128)
				if (m_pool_128.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 256)
				if (m_pool_256.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 512)
				if (m_pool_512.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 1024)
				if (m_pool_1024.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 2048)
				if (m_pool_2048.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 4096)
				if (m_pool_4096.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 8192)
				if (m_pool_8192.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 16384)
				if (m_pool_16384.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 32768)
				if (m_pool_32768.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 65536)
				if (m_pool_65536.TryDeallocate<T, sys>(p)) return;
			if constexpr (size <= 131072)
				if (m_pool_131072.TryDeallocate<T, sys>(p)) return;
			//not allocated using memory manager
			auto it = std::find(m_allocatedBlocks.begin(), m_allocatedBlocks.end(), reinterpret_cast<uint8_t*>(p));
			if (it != m_allocatedBlocks.end())
			{
				delete[] reinterpret_cast<uint8_t*>(p);
				m_allocatedBlocks.erase(it);
				return;
			}
			SE_ASSERT(false, "Tried freeing memory of type {} that was not allocated using the memory manager.", typeid(T).name());
		}
	private:
		/**
		 * \brief The instance
		 */
		static std::unique_ptr<MemoryManager> s_instance;
		
		std::list<uint8_t*> m_allocatedBlocks;
#define DEFINE_POOL(blockSize) MemoryPool<blockSize> m_pool_##blockSize;
		DEFINE_POOL(64);
		DEFINE_POOL(128);
		DEFINE_POOL(256);
		DEFINE_POOL(512);
		DEFINE_POOL(1024);
		DEFINE_POOL(2048);
		DEFINE_POOL(4096);
		DEFINE_POOL(8192);
		DEFINE_POOL(16384);
		DEFINE_POOL(32768);
		DEFINE_POOL(65536);
		DEFINE_POOL(131072);
	};
}
