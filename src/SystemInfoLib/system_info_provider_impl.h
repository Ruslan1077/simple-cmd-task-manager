#pragma once
#include <thread>
#include <memory>
#include <mutex>
#include "system_info_provider.h"

namespace sys
{
    class system_info_provider_impl
        : public system_info_provider
        , public std::enable_shared_from_this<system_info_provider_impl>
    {
    public:
        system_info_provider_impl(system_info_observer* observer);
        ~system_info_provider_impl();

        static std::shared_ptr<system_info_provider> create(
            system_info_observer* observer)
        {
            return std::make_shared<system_info_provider_impl>(observer);
        }

    private:
        // system_info_provider
        void run_update_async() override;
        void stop_update() override;

        // helper
        void thread_fn();

    private:
        std::mutex mutex_;
        bool is_started_ = false;
        system_info_observer* observer_;
        std::unique_ptr<std::thread> update_thread_;
    };

} // namespace sys