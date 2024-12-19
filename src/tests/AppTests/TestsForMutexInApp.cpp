#include <gtest/gtest.h>
#include <thread>
#include <shared_mutex>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>


//  SHORT EXPLANATION ABOUT THIS TESTS
//**************************************
//
// I split the functionality of the app and did tests
// separately regarding the mutex and the other things,
// and regarding the mutex I checked here that the
// locking and releasing are exactly as expected,
// and I used the mutex here exactly as I used in the app.
//
//****************************************


// reader writer special mutex
std::shared_mutex rw_mutex;
// mutex to increase and decrease the value of active_workers
std::mutex mtx;
// shared data that contains the number of active readers and writers
int active_workers = 0;

// reader function
void read(int milliseconds) {
    // lock for reader just as I did in the app class
    std::shared_lock<std::shared_mutex> lock(rw_mutex);
    // increase the active_workers
    {
        std::lock_guard<std::mutex> guard(mtx);
        active_workers++;
    }
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    // decrease the active_workers
    {
        std::lock_guard<std::mutex> guard(mtx);
        active_workers--;
    }
}

// writer function
void write(int milliseconds) {
    // lock for writer just as I did in the app class
    std::unique_lock<std::shared_mutex> lock(rw_mutex);
    // increase the active_workers
    {
        std::lock_guard<std::mutex> guard(mtx);
        active_workers++;
    }
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    // decrease the active_workers
    {
        std::lock_guard<std::mutex> guard(mtx);
        active_workers--;
    }
}

// first test for checking if 2 readers can read at the same time
TEST(MutexInApp, 2Readers) {
    // initialize the shared data
    active_workers = 0;
    // creating threads for the 2 readers and make them work for 1 second
    std::thread reader1([&]() { read(1000); });
    std::thread reader2([&]() { read(1000); });

    // waiting for half a second and then check how many active workers are there
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int real_active_workers;
    {
        std::lock_guard<std::mutex> guard(mtx);
        real_active_workers = active_workers;
    }

    // waiting for the workers to finish their work
    reader1.join();
    reader2.join();

    // it should be 2 because 2 readers can read at the same time
    EXPECT_EQ(real_active_workers, 2);
}

// second test for checking if 2 writers can read at the same time
TEST(MutexInApp, 2Writers) {
    // initialize the shared data
    active_workers = 0;
    // creating threads for the 2 writers and make them work for 1 second
    std::thread writer1([&]() { write(1000); });
    std::thread writer2([&]() { write(1000); });

    // waiting for half a second and then check how many active workers are there
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int real_active_workers;
    {
        std::lock_guard<std::mutex> guard(mtx);
        real_active_workers = active_workers;
    }

    // waiting for the workers to finish their work
    writer1.join();
    writer2.join();

    // it should be 1 because 2 writers cannot! write at the same time
    EXPECT_EQ(real_active_workers, 1);
}

// third test for checking if a reader and a writer can read and write at the same time
TEST(MutexInApp, ReaderAndWriter) {
    // initialize the shared data
    active_workers = 0;
    // creating threads for the writer and reader and make them work for 1 second
    std::thread reader([&]() { read(1000); });
    std::thread writer([&]() { write(1000); });

    // waiting for half a second and then check how many active workers are there
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int real_active_workers;
    {
        std::lock_guard<std::mutex> guard(mtx);
        real_active_workers = active_workers;
    }

    // waiting for the workers to finish their work
    reader.join();
    writer.join();

    // it should be 1 because a reader and a writer cannot! read and write at the same time
    EXPECT_EQ(real_active_workers, 1);
}
