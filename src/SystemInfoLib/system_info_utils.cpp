#include "pch.h"
#include "system_info_utils.h"
#include <Psapi.h>

namespace sys
{
    up_time_data parse_up_time_seconds(double up_time_sec)
    {
        int minutes_param = 60;
        int hours_param = minutes_param * 60;
        int days_param = hours_param * 24;
        up_time_data result;

        result.days_ = static_cast<int>(up_time_sec / days_param);
        up_time_sec = std::fmod(up_time_sec, days_param);

        result.hours_ = static_cast<int>(up_time_sec / hours_param);
        up_time_sec = std::fmod(up_time_sec, hours_param);

        result.minutes_ = static_cast<int>(up_time_sec / minutes_param);
        up_time_sec = std::fmod(up_time_sec, minutes_param);

        result.seconds_ = static_cast<int>(up_time_sec);
        return result;
    }

    std::vector<DWORD> get_all_active_pids()
    {
        std::vector<DWORD> pids(1024);
        DWORD size = 0;

        if (EnumProcesses(pids.data(), pids.size(), &size))
        {
            size_t returned_size = size / sizeof(DWORD);
            pids.resize(returned_size);
            return pids;
        }

        pids.resize(pids.size() * 2);
        if (EnumProcesses(pids.data(), pids.size(), &size))
        {
            size_t returned_size = size / sizeof(DWORD);
            pids.resize(returned_size);
            return pids;
        }

        throw std::runtime_error("Need more size for PIDs array");
    }

    int64_t file_time_2_utc(const FILETIME* ftime)
    {
        LARGE_INTEGER li;

        li.LowPart = ftime->dwLowDateTime;
        li.HighPart = ftime->dwHighDateTime;
        return li.QuadPart;
    }

} // namespace sys
