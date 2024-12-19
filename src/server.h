#ifndef SERVER_H
#define SERVER_H

// server class
class server {
public:
    server(int port);
    // pure virtual function: run
    void start();
private:
    int port;
};

#endif
