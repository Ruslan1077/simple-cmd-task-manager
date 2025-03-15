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

} // namespace sys