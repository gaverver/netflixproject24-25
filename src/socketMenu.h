#ifndef SOCKETMENU_H
#define SOCKETMENU_H
#include "IMenu.h"

#include <string>
#include <vector>
// socketMenu class
class socketMenu : public IMenu {
    private:
        int client_socket;
        char buffer[4096];
        int bufferSize;
    public:
        //constructor for the class
        socketMenu(int client_socket);
        // method to send a given input to the client through the socket.
        void print(std::string output) override;
        // method to scan user command line from the socket.
        std::string scan() override;
        // method to get the command that the user entered.
        std::vector<std::string>nextCommand () override;
    };
#endif