#include "sepch.h"

#include "core/UUID.h"

sengine::UUID sengine::UUID::Generate(IdType flag)
{
	static thread_local UUIDGenerator generator{};
	return generator.Generate(flag);
}

sengine::UUID::IdType sengine::UUID::GetType() const
{
	return static_cast<IdType>(m_high >> 62);
}

uint64_t sengine::UUID::GetTimestamp() const
{
	return (m_high & 0x3FFFFFFFFFFFFFFF) + EPOCH_2026;
}

sengine::SString<sengine::Systems::Core> sengine::UUID::ToString() const
{
	SStringStream<sengine::Systems::Core> ss;
	ss << std::hex << std::setfill('0');
	ss << std::setw(8) << (m_high >> 32) << "-";
	ss << std::setw(4) << ((m_high >> 16) & 0xFFFF) << "-";
	ss << std::setw(4) << (m_high & 0xFFFF) << "-";
	ss << std::setw(4) << (m_low >> 48) << "-";
	ss << std::setw(12) << (m_low & 0xFFFFFFFFFFFFULL);
	return ss.str();
}

bool sengine::UUID::operator==(const UUID& other) const
{
	return m_low == other.m_low && m_high == other.m_high;
}

sengine::UUIDGenerator::UUIDGenerator()
{
	m_machineHash = GenerateMachineHash();
	m_pid = static_cast<uint16_t>(getpid()) & 0x0fff;
}

sengine::UUID sengine::UUIDGenerator::Generate(const sengine::UUID::IdType& type)
{
	auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	uint64_t timestamp = now_ms = EPOCH_2026;
	if (timestamp <= m_lastTimestamp)
	{
		timestamp = m_lastTimestamp;
		m_sequence = (m_sequence + 1) & 0xfffff;
		if (m_sequence == 0)
		{
			while (timestamp <= m_lastTimestamp)
			{
				timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - EPOCH_2026;
			}
		}
		else
		{
			m_sequence = 0;
			m_lastTimestamp = timestamp;
		}
		return 
		{
			(static_cast<uint64_t>(static_cast<uint8_t>(type) & 0x03) << 62) | (timestamp & 0x3FFFFFFFFFFFFFFF),
			(static_cast<uint64_t>(m_machineHash) << 32) | (static_cast<uint64_t>(m_pid) << 20) | m_sequence
		};
	}
	//SENGINE_ERROR("Clock moved backwards. Refusing to generate UUID for {} milliseconds", m_lastTimestamp - timestamp);
	return UUID::NullId();
}

uint32_t sengine::UUIDGenerator::GenerateMachineHash()
{
	SString<sengine::Systems::Core> uniqueId{ "engine-fallback" };
#ifdef _WIN32
	char buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(buffer);
	if (GetComputerNameA(buffer, &size)) uniqueId = buffer;
#else
	std::ifstream f("/etc/machine-id");
	if (f.is_open()) { f >> uniqueId; }
	else {
		char hostname[HOST_NAME_MAX];
		if (gethostname(hostname, HOST_NAME_MAX) == 0) uniqueId = hostname;
	}
#endif
	uint32_t hash = 0x811c9dc5; // FNV-1a
	for (char c : uniqueId) {
		hash ^= static_cast<uint32_t>(c);
		hash *= 0x01000193;
	}
	return hash;
}

std::ostream& sengine::operator<<(std::ostream& os, const UUID& uuid)
{
	os << uuid.ToString();
	return os;
}
