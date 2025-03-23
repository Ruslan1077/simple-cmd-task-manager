#include "pch.h"
#include "system_info_provider_impl.h"
#include "system_info_guards.h"
#include "system_info_process_data_provider.h"
#include "system_info_general_info_provider.h"

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
            cpu_usage_provider process_cup_usage_provider;
            general_info_provider general_provider;

            while (true)
            {
                {
                    std::lock_guard lock(mutex_);
                    if (!is_started_)
                        return;
                }

                Sleep(1000);

                double general_cpu_usage = 0.0;
                double general_ram_usage = 0.0;
                double up_time = 0.0;
                general_provider.get_general_data(
                    general_cpu_usage,
                    general_ram_usage,
                    up_time);

                auto active_processes_info = process_cup_usage_provider.get_cpu_usage_for_all_proc();

                //
                // NEED TO UPDATE INTERFACE
                //
                /*if (observer_)
                {
                    observer_->on_new_info(
                        cpu_val.doubleValue,
                        ram_val.doubleValue,
                        up_time_val.doubleValue);
                }*/
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
