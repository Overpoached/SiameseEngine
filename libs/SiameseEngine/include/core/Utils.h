#pragma once

#include "memory/SiameseAllocator.h"

namespace sengine
{
    inline SString<Systems::General> GetTimestamp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto time = system_clock::to_time_t(now);
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        SOStringStream<Systems::General> oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S")
            << '.' << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }

}