#include "pch.h"
#include "system_info_utils.h"

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

} // namespace sys
