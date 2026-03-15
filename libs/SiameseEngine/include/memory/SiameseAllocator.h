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

	//smart pointers
	template<typename T>
	using SSharedPtr = std::shared_ptr<T>;
	template<typename T, Systems sys = Systems::Undefined, typename... Args>
	inline SSharedPtr<T> SMakeShared(Args&&... args) { return std::allocate_shared<T>(SiameseAllocator<T, sys>{}, std::forward<Args>(args)...); }
	template<typename T, Systems sys>
	struct SUniquePtrDeleter
	{
		void operator()(T* ptr) const 
		{
			if (ptr) 
			{
				ptr->~T();
				MemoryManager::Get().Deallocate<T, sys>(ptr);
			}
		}
	};
	template<typename T, Systems sys>
	using SUniquePtr = std::unique_ptr<T, SUniquePtrDeleter<T, sys>>;
	template<typename T, Systems sys = Systems::Undefined, typename... Args>
	inline SUniquePtr<T, sys> SMakeUnique(Args&&... args)
	{
		void* mem = MemoryManager::Get().Allocate<T, sys>();
		T* ptr = new (mem) T(std::forward<Args>(args)...);
		return SUniquePtr<T>(ptr, SUniquePtrDeleter<T>());
	}

	//containers
	template<Systems system = Systems::Undefined>
	using SString = std::basic_string<char, std::char_traits<char>, sengine::SiameseAllocator<char, system>>;
	template<Systems system = Systems::Undefined>
	using SStringStream = std::basic_stringstream<char, std::char_traits<char>, sengine::SiameseAllocator<char, system>>;
	template<Systems system = Systems::Undefined>
	using SOStringStream = std::basic_ostringstream<char, std::char_traits<char>, sengine::SiameseAllocator<char, system>>;
	template<typename KeyType, typename ValueType, Systems system = Systems::Undefined>
	using SMap = std::map<KeyType, ValueType, std::less<KeyType>, sengine::SiameseAllocator<std::pair<const KeyType, ValueType>, system>>;
	template<typename KeyType, typename ValueType, Systems system = Systems::Undefined>
	using SUnorderedMap = std::unordered_map<KeyType, ValueType, std::hash<KeyType>, std::equal_to<KeyType>, sengine::SiameseAllocator<std::pair<const KeyType, ValueType>, system>>;
	template<typename T, Systems system = Systems::Undefined>
	using SVector = std::vector<T, sengine::SiameseAllocator<T, system>>;
	template<typename T, Systems system = Systems::Undefined>
	using SList = std::list<T, sengine::SiameseAllocator<T, system>>;
}

