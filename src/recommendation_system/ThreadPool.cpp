#include "ThreadPool.h"

void ThreadPool::workerFunction() {
    while (true) {
        Runnable* r;
        bool clean;
        {
            // lock the queue mutex because the queue a shared object between all the thred
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });
            // if we continued because stop=tru but there are no tasks so there is nothing to handle
            if (stop && tasks.empty())
                return;
            // take out the next task(runnable) and the boolean for the thread to handle
            r = tasks.front();
            clean = cleanVars.front();
            cleanVars.pop();
            tasks.pop(); // Mutex is unlocked here
        }
        r->run();
        // delete the runnable object if the boolean var that fits to the task is true
        if (clean) {
            delete r;
        }
    }
}
ThreadPool::ThreadPool(size_t numWorkers) : stop(false) {
    for (size_t i = 0; i < numWorkers; ++i) {
        // create threads that will run the worker function and push them into the queue
        workers.emplace_back([this]() { workerFunction(); });
    }
}

void ThreadPool::execute(Runnable* r, bool clean) {
    {
        // lock because we touch the queue which is shared with all threads
        std::unique_lock<std::mutex> lock(queueMutex);
        // push the runnable object and the boolean variable to the fitting queues
        tasks.push(r);
        cleanVars.push(clean);
    }
    // notify the condition variable that a task has entered to the queue
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    {
        // change stop to true, so this will affect the condition variable and every thread will be able to finish his job
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    // notify all thread and update the condition variable 
    condition.notify_all();
    for (std::thread &worker : workers) {
        // wait for all worker threads to finish their job
        if (worker.joinable()) {
            worker.join();
        }
    }
}
