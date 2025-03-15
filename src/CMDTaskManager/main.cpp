#include <iostream>
#include <system_info_provider_impl.h>

// DBG

void print_up_time(double up_time_sec)
{
    int minutes_param = 60;
    int hours_param = minutes_param * 60;
    int days_param = hours_param * 24;

    int days = static_cast<int>(up_time_sec / days_param);
    up_time_sec = std::fmod(up_time_sec, days_param);

    int hours = static_cast<int>(up_time_sec / hours_param);
    up_time_sec = std::fmod(up_time_sec, hours_param);

    int minutes = static_cast<int>(up_time_sec / minutes_param);
    up_time_sec = std::fmod(up_time_sec, minutes_param);

    int seconds = static_cast<int>(up_time_sec);

    std::cout << days << "d " << hours << "h " << minutes << "m " << seconds << "s" << std::endl;
}

class observer : public sys::system_info_observer
{
private:
    void on_error(const std::string& what) override
    {
        std::cout << "Error: " << what << std::endl;
    }

    void on_new_info(
        double percent_of_cpu_usage,
        double percent_of_ram_usage,
        double up_time_seconds) override
    {
        system("cls");
        std::cout << "CPU:     " << std::ceil(percent_of_cpu_usage * 10.0) / 10.0 << std::endl;
        std::cout << "RAM:     " << std::ceil(percent_of_ram_usage * 10.0) / 10.0 << std::endl;
        std::cout << "UP TIME: ";
        print_up_time(up_time_seconds);
        std::cout << std::endl;
    }
};

int main()
{
    system("pause");

    observer obs;
    auto provider = sys::system_info_provider_impl::create(&obs);

    provider->run_update_async();

    int cmd;
    while (std::cin >> cmd)
    {
        if (cmd == 1)
        {
            provider->stop_update();
            return 0;
        }
    }

    return 0;
}