#ifndef THREADPOOL_H
#define THREADPOOL_h


#include "executor.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <queue>
#include <condition_variable>
#include <string>
#include <cstring>
#include "Runnable.h"
// ThreadPool class
class ThreadPool : public executor {
private:
    std::vector<std::thread> workers; // Queue where all the threads are stored in
    std::queue<Runnable*> tasks; // Queue of runnable objects to run
    std::queue<bool> cleanVars; // Queue of boolean variables that determine wether to clean up or not.
    std::mutex queueMutex; // mutex to handle access to the queue
    std::condition_variable condition;
    std::atomic<bool> stop;
    void workerFunction();
public:
    ThreadPool(size_t numWorkers);
    ~ThreadPool();
    // function to execute the runnable object.
    void execute(Runnable* r, bool clean) override;
};

#endif