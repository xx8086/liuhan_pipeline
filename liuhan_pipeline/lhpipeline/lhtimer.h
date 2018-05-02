#pragma once

#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

namespace lh_pipeline {
    class LhTimer
    {
    public:
        LhTimer();
        LhTimer(const LhTimer& t);
        ~LhTimer();
    public:
        void set_count_time(int ms);
        bool count_time();//每隔ms毫秒返回true,在ms时间内返回都是false
        void start_timer(int interval, std::function<void()> task);
        void release();

        template<typename callable, class... arguments>
        void sync_wait(int after, callable&& f, arguments&&... args) {
            std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
        template<typename callable, class... arguments>
        void async_wait(int after, callable&& f, arguments&&... args) {
            std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }).detach();
        }

    private:
        int _ms_count;
        std::chrono::system_clock::time_point _aysnc_ms_count;

        std::atomic<bool> _expired;
        std::atomic<bool> _try_to_expire;
        std::mutex _mutex;
        std::condition_variable _expired_cond;
    };
}

