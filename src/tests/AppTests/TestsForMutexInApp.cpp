#include <gtest/gtest.h>
#include <thread>
#include <shared_mutex>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>

// Shared resources and mutexes
std::shared_mutex rw_mutex;
std::mutex mtx
int active_workers = 0

void read(int milliseconds) {
    std::shared_lock lock(rw_mutex);
    mtx.lock();
    active_workers++;  // Modify shared resource
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));  // Simulate work
    mtx.lock();
    active_workers--;
    mtx.unlock();
}

void write(int milliseconds) {
    std::unique_lock lock(rw_mutex);
    mtx.lock();
    active_workers++;  // Modify shared resource
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));  // Simulate work
    mtx.lock();
    active_workers--;
    mtx.unlock();
}

TEST(MutexInApp, 2Readers) {
    active_workers = 0;
    std::thread reader1(() { read(1000); });
    std::thread reader2(() { read(1000); });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int real_active_workers = active_workers;

    reader1.join();
    reader2.join();

    EXPECT_EQ(real_active_workers, 2);  // Ensure all readers completed
}

TEST(MutexInApp, 2Writers) {
    active_workers = 0;
    std::thread reader1(() { write(1000); });
    std::thread reader2(() { write(1000); });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int real_active_workers = active_workers;

    reader1.join();
    reader2.join();

    EXPECT_EQ(real_active_workers, 1);  // Ensure all readers completed
}

TEST(MutexInApp, ReaderAndWriter) {
    active_workers = 0;
    std::thread reader1(() { read(1000); });
    std::thread reader2(() { write(1000); });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int real_active_workers = active_workers;

    reader1.join();
    reader2.join();

    EXPECT_EQ(real_active_workers, 1);  // Ensure all readers completed
}
