#include <gtest/gtest.h>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <atomic>
#include <chrono>

// Shared resources and mutex
std::shared_mutex rw_mutex;
std::atomic<int> shared_counter{0};  // Simulates shared resource

void reader(int &active_readers) {
    std::shared_lock lock(rw_mutex);
    active_readers++;  // Track number of active readers
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Simulate work
    active_readers--;
}

void writer() {
    std::unique_lock lock(rw_mutex);
    shared_counter++;  // Modify shared resource
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work
}

TEST(ReaderWriterLock, ConcurrentReaders) {
    int active_readers = 0;
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(reader, std::ref(active_readers));
    }

    for (auto &t : threads) {
        t.join();
    }

    EXPECT_EQ(active_readers, 0);  // Ensure all readers completed
}

TEST(ReaderWriterLock, ExclusiveWriterAccess) {
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(writer);
    }

    for (auto &t : threads) {
        t.join();
    }

    EXPECT_EQ(shared_counter.load(), 5);  // Ensure all writers executed
}

TEST(ReaderWriterLock, WriterBlocksReaders) {
    int active_readers = 0;
    std::atomic<bool> writer_active{false};
    bool reader_blocked = false;

    std::thread writer_thread([&]() {
        writer_active = true;
        writer();  // Writer acquires the lock
        writer_active = false;
    });

    std::thread reader_thread([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Delay to let writer start
        std::shared_lock lock(rw_mutex);
        if (writer_active) {
            reader_blocked = true;  // Detect if reader had to wait
        }
        active_readers++;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        active_readers--;
    });

    writer_thread.join();
    reader_thread.join();

    EXPECT_TRUE(reader_blocked);  // Reader was blocked by the writer
    EXPECT_EQ(active_readers, 0);  // Ensure reader completed
}

TEST(ReaderWriterLock, WriterPriorityAfterReaders) {
    int active_readers = 0;
    bool writer_executed = false;

    std::thread reader_thread([&]() {
        std::shared_lock lock(rw_mutex);
        active_readers++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate prolonged read
        active_readers--;
    });

    std::thread writer_thread([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Wait for reader to start
        std::unique_lock lock(rw_mutex);
        writer_executed = true;  // Writer executes after readers finish
    });

    reader_thread.join();
    writer_thread.join();

    EXPECT_TRUE(writer_executed);  // Ensure writer executed after readers
    EXPECT_EQ(active_readers, 0);  // Ensure no active readers
}
