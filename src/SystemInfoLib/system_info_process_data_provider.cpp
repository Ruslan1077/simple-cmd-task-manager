#include "pch.h"
#include "system_info_process_data_provider.h"
#include "system_info_consts.h"
#include "system_info_utils.h"
#include "system_info_guards.h"

namespace sys
{
    cpu_usage_provider::cpu_usage_provider()
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);

        processor_count_ = info.dwNumberOfProcessors;
    }

    std::vector<std::pair<int, int>> cpu_usage_provider::get_cpu_usage_for_all_proc()
    {
        std::vector<DWORD> pids = get_all_active_pids();
        std::vector<pid_cpu_usage_t> result;

        for (auto& pid : pids)
        {
            try
            {
                result.push_back(get_cpu_usage_for_single_proc(pid));
            }
            catch (const std::exception&)
            {
            }
        }

        FILETIME now;
        GetSystemTimeAsFileTime(&now);

        int64_t now_utc = file_time_2_utc(&now);
        for (auto it = process_data_cache_.begin(); it != process_data_cache_.end();)
        {
            int64_t update_time_delta = (now_utc - it->second.time_of_last_update_) / g_second_in_ticks;
            if (update_time_delta >= g_cpu_data_cache_lifetime_s)
            {
                it = process_data_cache_.erase(it);
            }
            else
            {
                ++it;
            }
        }

        return result;
    }

    std::pair<int, int> cpu_usage_provider::get_cpu_usage_for_single_proc(DWORD pid)
    {
        sys::process_handle proc(pid);
        FILETIME creation_time, exit_time; // not used
        FILETIME kernel_time, user_time;
        FILETIME now;

        GetSystemTimeAsFileTime(&now);

        bool res = GetProcessTimes(
            proc.get(),
            &creation_time,
            &exit_time,
            &kernel_time,
            &user_time);
        if (!res)
            return std::make_pair(pid, 0);;

        int64_t now_utc = file_time_2_utc(&now);
        int64_t system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;

        process_data& data = process_data_cache_[pid];
        if (data.last_system_time_ == 0 ||
            data.time_of_last_update_ == 0)
        {
            data.last_system_time_ = system_time;
            data.time_of_last_update_ = now_utc;
            return std::make_pair(pid, 0);
        }

        int64_t system_type_delta = system_time - data.last_system_time_;
        int64_t update_delta = now_utc - data.time_of_last_update_;
        if (!update_delta)
            return std::make_pair(pid, 0);

        cpu_usage_t cpu = static_cast<cpu_usage_t>(
            (system_type_delta * 100 + update_delta / 2) / update_delta);

        data.last_system_time_ = system_time;
        data.time_of_last_update_ = now_utc;
        return std::make_pair(pid, cpu);
    }

} // namespace sys