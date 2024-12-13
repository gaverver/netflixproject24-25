#include "ThreadFactory.h"
#include <thread>

void ThreadFactory::execute(Runnable& r) {
    std::thread thread([&r]() { r.run(); });
}