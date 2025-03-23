#pragma once
#include <Pdh.h>
#include <stdexcept>

namespace sys
{
    class pdh_query
    {
    public:
        pdh_query()
        {
            PDH_STATUS status;
            status = PdhOpenQuery(nullptr, 0, &query_);

            if (status != ERROR_SUCCESS)
            {
                throw std::runtime_error("PdhOpenQuery failed");
            }
        }

        HQUERY get() const
        {
            return query_;
        }

        ~pdh_query()
        {
            if(query_)
                PdhCloseQuery(query_);
        }

    private:
        HQUERY query_;
    };

    class process_handle
    {
    public:
        explicit process_handle(DWORD pid)
        {
            process_handle_ = OpenProcess(
                PROCESS_ALL_ACCESS,
                false,
                pid);
        }

        HANDLE get()
        {
            return process_handle_;
        }

        ~process_handle()
        {
            if (process_handle_)
                CloseHandle(process_handle_);
        }

    private:
        HANDLE process_handle_;
    };

} // namespace sys