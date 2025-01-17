#ifndef CUSTOM_MUTEX_HPP
#define CUSTOM_MUTEX_HPP

#include <FreeRTOS.h>
#include <semphr.h>
#include <chrono>

namespace std {

class mutex {
public:
    mutex() {
        handle = xSemaphoreCreateMutex();
    }
    ~mutex() {
        if (handle) vSemaphoreDelete(handle);
    }
    void lock() {
        xSemaphoreTake(handle, portMAX_DELAY);
    }
    void unlock() {
        xSemaphoreGive(handle);
    }
private:
    SemaphoreHandle_t handle;
};

class recursive_mutex {
public:
    recursive_mutex() {
        handle = xSemaphoreCreateRecursiveMutex();
    }
    ~recursive_mutex() {
        if (handle) vSemaphoreDelete(handle);
    }
    void lock() {
        xSemaphoreTakeRecursive(handle, portMAX_DELAY);
    }
    void unlock() {
        xSemaphoreGiveRecursive(handle);
    }
private:
    SemaphoreHandle_t handle;
};

// Unique lock implementation for FreeRTOS mutexes
template <typename MutexType>
class unique_lock {
public:
    explicit unique_lock(MutexType& mtx) : mutex_(mtx), owns_lock_(false) {
        lock();
    }
    ~unique_lock() {
        if (owns_lock_) unlock();
    }
    void lock() {
        mutex_.lock();
        owns_lock_ = true;
    }
    void unlock() {
        mutex_.unlock();
        owns_lock_ = false;
    }
private:
    MutexType& mutex_;
    bool owns_lock_;
};

// Lock guard implementation for FreeRTOS mutexes
template <typename MutexType>
class lock_guard {
public:
    explicit lock_guard(MutexType& mtx) : mutex_(mtx) {
        mutex_.lock();
    }
    ~lock_guard() {
        mutex_.unlock();
    }

    // Deleting copy and move operations to prevent incorrect usage
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

private:
    MutexType& mutex_;
};

enum class cv_status { no_timeout, timeout };

class condition_variable {
public:
    condition_variable()
        : semaphore(xSemaphoreCreateCounting(configMAX_PRIORITIES, 0)),
          waiters_mutex(xSemaphoreCreateMutex()),
          waiting_threads(0) {
    }

    ~condition_variable() {
        if (semaphore) vSemaphoreDelete(semaphore);
        if (waiters_mutex) vSemaphoreDelete(waiters_mutex);
    }

    condition_variable(const condition_variable&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;

    void notify_one() {
        xSemaphoreTake(waiters_mutex, portMAX_DELAY);
        if (waiting_threads > 0) {
            --waiting_threads;
            xSemaphoreGive(semaphore);
        }
        xSemaphoreGive(waiters_mutex);
    }

    void notify_all() {
        xSemaphoreTake(waiters_mutex, portMAX_DELAY);
        while (waiting_threads > 0) {
            --waiting_threads;
            xSemaphoreGive(semaphore);
        }
        xSemaphoreGive(waiters_mutex);
    }

    void wait(unique_lock<mutex>& lock) {
        xSemaphoreTake(waiters_mutex, portMAX_DELAY);
        ++waiting_threads;
        xSemaphoreGive(waiters_mutex);

        lock.unlock();
        xSemaphoreTake(semaphore, portMAX_DELAY);
        lock.lock();
    }

    template <typename Predicate>
    void wait(unique_lock<mutex>& lock, Predicate pred) {
        while (!pred()) {
            wait(lock);
        }
    }

    template <typename Rep, typename Period>
    cv_status wait_for(unique_lock<mutex>& lock, const std::chrono::duration<Rep, Period>& rel_time) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(rel_time).count();
        return wait_until(lock, std::chrono::steady_clock::now() + rel_time);
    }

    template <typename Rep, typename Period, typename Predicate>
    bool wait_for(unique_lock<mutex>& lock, const std::chrono::duration<Rep, Period>& rel_time, Predicate pred) {
        auto end_time = std::chrono::steady_clock::now() + rel_time;
        while (!pred()) {
            if (wait_until(lock, end_time) == cv_status::timeout) {
                return pred();
            }
        }
        return true;
    }

    template <typename Clock, typename Duration>
    cv_status wait_until(unique_lock<mutex>& lock, const std::chrono::time_point<Clock, Duration>& abs_time) {
        auto now = Clock::now();
        if (now >= abs_time) {
            return cv_status::timeout;
        }

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(abs_time - now).count();

        xSemaphoreTake(waiters_mutex, portMAX_DELAY);
        ++waiting_threads;
        xSemaphoreGive(waiters_mutex);

        lock.unlock();
        BaseType_t result = xSemaphoreTake(semaphore, pdMS_TO_TICKS(ms));
        lock.lock();

        xSemaphoreTake(waiters_mutex, portMAX_DELAY);
        if (result == pdFALSE) {
            --waiting_threads;
        }
        xSemaphoreGive(waiters_mutex);

        return result == pdTRUE ? cv_status::no_timeout : cv_status::timeout;
    }

private:
    SemaphoreHandle_t semaphore;
    SemaphoreHandle_t waiters_mutex;
    int waiting_threads;
};


} // namespace std

#endif // CUSTOM_MUTEX_HPP
