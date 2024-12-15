#include "ThreadFactory.h"
#include <thread>

void ThreadFactory::execute(Runnable& r) {
    // create a thread to run the run function of the runnable object.
    std::thread thread([&r]() { r.run(); });
}
