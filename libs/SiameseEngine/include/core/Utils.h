#pragma once

namespace sengine
{
    inline std::string GetTimestamp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto time = system_clock::to_time_t(now);
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S")
            << '.' << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }

}