#pragma once
#include "system_info_guards.h"
#include <Pdh.h>
#pragma comment(lib, "pdh.lib")

namespace sys
{
    class general_info_provider
    {
    public:
        general_info_provider();

        void get_general_data(
            double& out_cpu_usage,
            double& out_ram_usage,
            double& out_up_time);

    private:
        pdh_query query_;
        PDH_HCOUNTER cpu_used_;
        PDH_HCOUNTER ram_used_;
        PDH_HCOUNTER up_time_;
    };

} // namespace sys