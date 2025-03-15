#pragma once
#include <functional>

namespace sys
{
    struct system_info_observer
    {
        virtual ~system_info_observer() = default;

        virtual void on_error(const std::string& what) = 0;
        virtual void on_new_info(
            double percent_of_cpu_usage,
            double percent_of_ram_usage,
            double up_time_seconds) = 0;
    };

    struct system_info_provider
    {
        virtual ~system_info_provider() = default;

        virtual void run_update_async() = 0;
        virtual void stop_update() = 0;
    };

} // namespace sys