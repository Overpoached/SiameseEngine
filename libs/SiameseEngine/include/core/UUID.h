#pragma once

#include "memory/SiameseAllocator.h"

namespace sengine
{
	static constexpr uint32_t EPOCH_2026 = 1767225600000; // seconds since Unix epoch to Jan 1, 2026
	class UUIDGenerator;
	struct UUID
	{
		enum class IdType : uint8_t
		{
			Entity = 0,
			Asset = 1,
			Internal = 2,
			Reserved = 3
		};

		static UUID NullId() { return UUID(); };
		uint64_t m_high{};
		uint64_t m_low{};

		static UUID Generate(IdType flag);

		IdType GetType() const;
		uint64_t GetTimestamp() const;

		SString<sengine::Systems::Core> ToString() const;

		bool operator==(const UUID& other) const;
	};

	std::ostream& operator<<(std::ostream& os, const UUID& uuid);

	class UUIDGenerator
	{
	public:
		UUIDGenerator();
		~UUIDGenerator() = default;
		UUID Generate(const UUID::IdType& type);

	private:
		uint64_t m_lastTimestamp{};
		uint32_t m_sequence{};
		uint32_t m_machineHash{};
		uint16_t m_pid{};

		uint32_t GenerateMachineHash();
	};
}

namespace std
{
	template<>
	struct hash<sengine::UUID>
	{
		size_t operator()(const sengine::UUID& uuid) const
		{
			size_t h = hash<uint64_t>{}(uuid.m_high);
			//golden ratio to prevent bit clumping
			h ^= hash<uint64_t>{}(uuid.m_low) + 0x9e3779b9 + (h << 6) + (h >> 2);
		}
	};
}