#pragma once
#include "system_info_commons.h"
#include <Windows.h>

namespace sys
{
    up_time_data parse_up_time_seconds(double up_time_sec);

    std::vector<DWORD> get_all_active_pids();
    int64_t file_time_2_utc(const FILETIME* ftime);

} // namespace sys