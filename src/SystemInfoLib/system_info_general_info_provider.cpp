#include "pch.h"
#include "system_info_general_info_provider.h"

namespace sys
{
    general_info_provider::general_info_provider()
    {
        PdhAddCounter(
            query_.get(),
            L"\\Processor(_Total)\\% Processor Time",
            NULL,
            &cpu_used_);

        PdhAddCounter(
            query_.get(),
            L"\\Memory\\% Committed Bytes In Use",
            NULL,
            &ram_used_);

        PdhAddCounter(
            query_.get(),
            L"\\System\\System Up Time",
            NULL,
            &up_time_);
    }

    void general_info_provider::get_general_data(
        double& out_cpu_usage,
        double& out_ram_usage,
        double& out_up_time)
    {
        PDH_FMT_COUNTERVALUE cpu_val{};
        PDH_FMT_COUNTERVALUE ram_val{};
        PDH_FMT_COUNTERVALUE up_time_val{};

        PdhCollectQueryData(query_.get());

        PdhGetFormattedCounterValue(
            cpu_used_,
            PDH_FMT_DOUBLE,
            NULL,
            &cpu_val);

        PdhGetFormattedCounterValue(
            ram_used_,
            PDH_FMT_DOUBLE,
            NULL,
            &ram_val);

        PdhGetFormattedCounterValue(
            up_time_,
            PDH_FMT_DOUBLE,
            NULL,
            &up_time_val);

        out_cpu_usage = cpu_val.doubleValue;
        out_ram_usage = ram_val.doubleValue;
        out_up_time = up_time_val.doubleValue;
    }

} // namespace sys