#include "server.h"
#include "executor.h"
#include "ThreadFactory.h"
#include <string>

int main(int argc, char** argv) {
    ThreadFactory tf;
    executor& exec = tf;
    // create the server to run it.
    server runner(std::stoi(argv[1]), exec);
    runner.start();
}