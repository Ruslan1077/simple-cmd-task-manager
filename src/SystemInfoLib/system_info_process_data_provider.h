#pragma once
#include <Windows.h>
#include <unordered_map>

namespace sys
{
    class cpu_usage_provider
    {
        // get process name?
        // https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes

        using pid_t = int;
        using cpu_usage_t = int;
        using pid_cpu_usage_t = std::pair<pid_t, cpu_usage_t>;

    private:
        struct process_data
        {
            int64_t last_system_time_ = 0;
            int64_t time_of_last_update_ = 0;
        };

    public:
        cpu_usage_provider();
        std::vector<pid_cpu_usage_t> get_cpu_usage_for_all_proc();

    private:
        pid_cpu_usage_t get_cpu_usage_for_single_proc(DWORD pid);

    private:
        int processor_count_ = -1;
        std::unordered_map<pid_t, process_data> process_data_cache_;
    };

} // namespace sys