#include "ThreadFactory.h"
#include <thread>

void ThreadFactory::execute(Runnable* r, bool clean) {
    // create a thread to run the run function of the runnable object and then destruct it if needed.
    std::thread thread([r, clean]() {
        r->run();
        if(clean) {
            delete r;
        }
    });
    thread.detach();
}
