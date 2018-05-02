#include "lhtimer.h"


namespace lh_pipeline {
    LhTimer::LhTimer() :_expired(true), _try_to_expire(false) {
    }
    LhTimer::LhTimer(const LhTimer& t) {
        _expired = t._expired.load();
        _try_to_expire = t._try_to_expire.load();
    }

    LhTimer::~LhTimer(){
        release();
    }

    void LhTimer::set_count_time(int ms) {
        _ms_count = ms;
        _aysnc_ms_count = std::chrono::system_clock::now();
    }
    bool LhTimer::count_time() {
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - _aysnc_ms_count);
        if (duration.count() >= _ms_count) {
            _aysnc_ms_count = std::chrono::system_clock::now();
            return true;
        }
        return false;
    }

    void LhTimer::start_timer(int interval, std::function<void()> task) {
        if (_expired == false) {
            //当前正在运行
            return;
        }
        _expired = false;
        std::thread([this, interval, task]() {
            while (!_try_to_expire) {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                task();
            }
            //std::cout << "stop task..." << std::endl;
            {
                std::lock_guard<std::mutex> locker(_mutex);
                _expired = true;
                _expired_cond.notify_one();
            }
        }).detach();
    }

    void LhTimer::release() {
        if (_expired) {
            return;
        }

        if (_try_to_expire) {
            //          std::cout << "timer is trying to expire, please wait..." << std::endl;
            return;
        }
        _try_to_expire = true;
        {
            std::unique_lock<std::mutex> locker(_mutex);
            _expired_cond.wait(locker, [this] {return _expired == true; });
            if (_expired == true) {
                //std::cout << "timer expired!" << std::endl;
                _try_to_expire = false;
            }
        }
    }
}