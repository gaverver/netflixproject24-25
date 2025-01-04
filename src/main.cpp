#include "server.h"
#include "executor.h"
#include "ThreadPool.h"
#include <string>

int main(int argc, char** argv) {
    ThreadPool tp(100);
    executor& exec = tp;
    // create the server to run it.
    server runner(std::stoi(argv[1]), exec);
    runner.start();
}
