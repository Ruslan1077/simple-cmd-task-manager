#include "pch.h"
#include "system_info_provider_impl.h"
#include "system_info_guards.h"

namespace sys
{
    system_info_provider_impl::system_info_provider_impl(system_info_observer* observer)
        : observer_(observer)
    {
    }

    system_info_provider_impl::~system_info_provider_impl()
    {
        stop_update();
    }

    void system_info_provider_impl::run_update_async()
    {
        std::lock_guard lock(mutex_);
        if (is_started_)
            throw std::runtime_error("system_info_provider_impl. update already started.");

        update_thread_.reset(new std::thread(
            [self = shared_from_this()]()
            {
                self->thread_fn();
            }));

        is_started_ = true;
    }

    void system_info_provider_impl::stop_update()
    {
        {
            std::lock_guard lock(mutex_);
            if (!is_started_)
                return;

            is_started_ = false;
        }

        if(update_thread_->joinable())
            update_thread_->join();

        update_thread_.reset();
    }

    void system_info_provider_impl::thread_fn()
    {
        try
        {
            pdh_query query;
            PDH_HCOUNTER cpu_used;
            PDH_HCOUNTER ram_used;
            PDH_HCOUNTER up_time;

            PdhAddCounter(
                query.get(),
                L"\\Processor(_Total)\\% Processor Time",
                NULL,
                &cpu_used);

            PdhAddCounter(
                query.get(),
                L"\\Memory\\% Committed Bytes In Use",
                NULL,
                &ram_used);

            PdhAddCounter(
                query.get(),
                L"\\System\\System Up Time",
                NULL,
                &up_time);

            while (true)
            {
                {
                    std::lock_guard lock(mutex_);
                    if (!is_started_)
                        return;
                }

                Sleep(1000);

                PDH_FMT_COUNTERVALUE cpu_val{};
                PDH_FMT_COUNTERVALUE ram_val{};
                PDH_FMT_COUNTERVALUE up_time_val{};

                PdhCollectQueryData(query.get());

                PdhGetFormattedCounterValue(
                    cpu_used,
                    PDH_FMT_DOUBLE,
                    NULL,
                    &cpu_val);

                PdhGetFormattedCounterValue(
                    ram_used,
                    PDH_FMT_DOUBLE,
                    NULL,
                    &ram_val);

                PdhGetFormattedCounterValue(
                    up_time,
                    PDH_FMT_DOUBLE,
                    NULL,
                    &up_time_val);

                if (observer_)
                {
                    observer_->on_new_info(
                        cpu_val.doubleValue,
                        ram_val.doubleValue,
                        up_time_val.doubleValue);
                }
            }
        }
        catch (const std::exception& ex)
        {
            if (observer_)
                observer_->on_error(ex.what());

            return;
        }
    }
} // namespace sys
