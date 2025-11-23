#pragma once

#include "MemoryManager.h"

namespace sengine
{
	template<typename T, Systems sys = Systems::Undefined>
	struct SiameseAllocator
	{
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		SiameseAllocator() = default;
		template<typename U>
		constexpr SiameseAllocator(const SiameseAllocator <U, sys>&) noexcept {}
		template<typename U, Systems sys2>
		bool operator==(const SiameseAllocator<U, sys2>&) const noexcept { return true; }
		template<typename U, Systems sys2>
		bool operator!=(const SiameseAllocator<U, sys2>&) const noexcept { return false; }


		/**
		 * \brief Allocates memory
		 * \param n The number of elements to allocate for
		 * \return Pointer to block of new memory allocated for the request
		 */
		[[nodiscard]] T* allocate(size_t n);
		/**
		 * \brief Deallocate a pointer and free the memory for reuse
		 * \param p The pointer
		 * \param n The number of elements
		 */
		void deallocate(T* p, size_t n) const;

		template<typename U>
		struct rebind {
			using other = SiameseAllocator<U, sys>;
		};
	};

	template <typename T, Systems sys>
	T* SiameseAllocator<T, sys>::allocate(size_t n)
	{
		if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_array_new_length();
		if (auto p = static_cast<T*>(MemoryManager::Get().Allocate<T, sys>(n))) {
			return p;
		}

		throw std::bad_alloc();
	}

	template <typename T, Systems sys>
	void SiameseAllocator<T, sys>::deallocate(T* p, size_t n) const
	{
		UNREFERENCED_PARAMETER(n);
		MemoryManager::Get().Deallocate<T, sys>(p);
	}
	
	_EXPORT_STD template <class _Ty, class _Other>
		_NODISCARD _CONSTEXPR20 bool operator==(const SiameseAllocator<_Ty>&, const SiameseAllocator<_Other>&) noexcept {
		return std::is_same_v<_Ty, _Other>;
	}
}

//helper macros for creating stuff with my allocator
#define SE_MAKE_SHARED(type, ...) std::allocate_shared<type>(sengine::SiameseAllocator<type>{}, __VA_ARGS__)
#define SE_MAKE_SHARED_SYSTEM(type, system, ...) std::allocate_shared<type>(sengine::SiameseAllocator<type, system>{}, __VA_ARGS__)

#define SE_UNORDERED_MAP(keyType, valueType) std::unordered_map<keyType, valueType, std::hash<keyType>, std::equal_to<keyType>, sengine::SiameseAllocator<std::pair<const keyType, valueType>>>
#define SE_UNORDERED_MAP_SYSTEM(keyType, valueType, system) std::unordered_map<keyType, valueType, std::hash<keyType>, std::equal_to<keyType>, sengine::SiameseAllocator<std::pair<const keyType, valueType>, system>>

#define SE_BASIC_STRING(charType) std::basic_string<charType, std::char_traits<charType>, sengine::SiameseAllocator<charType>>
#define SE_BASIC_STRING_SYSTEM(charType, system) std::basic_string<charType, std::char_traits<charType>, sengine::SiameseAllocator<charType, system>>
