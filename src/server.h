#ifndef SERVER_H
#define SERVER_H
#include "executor.h"
// server class
class server {
public:
    server(int port, executor& exec);
    // virtual function: start
    void start();
private:
    int port;
    executor& exec;
};

#endif
