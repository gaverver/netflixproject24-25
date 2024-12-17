#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "SocketMenu.h"
//constructor for the SocketMenu
SocketMenu::SocketMenu(int client_socket) : client_socket(client_socket) {
    bufferSize = sizeof(buffer);
}

std::string SocketMenu::scan() {
    std::string s = "";
    while (true) {
        int read_bytes =  recv(client_socket, buffer, bufferSize, 0);

        if (read_bytes <= 0) {
            //continue the program
            return "";
        }
        //add the buffer to the final string
        s.append(buffer, read_bytes);
        //assuming the message ends with \n
        if (read_bytes < bufferSize) {
            //removing \n at the end
            s.pop_back();
            return s;
        }
        if (read_bytes == bufferSize && s.back() == '\n') {
            s.pop_back();
            return s;
        }

    }
}

void SocketMenu::print(std::string output) {
    output += '\n';
    send(client_socket, output.data(), output.size(), 0);
}

bool SocketMenu::isConnected() {
    char buffer1[1];
    int read_bytes = recv(client_socket, buffer1, sizeof(buffer1), MSG_PEEK);
    if (read_bytes == 0) {
        //connection closed
        return false;
    } else if (read_bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            //still open
            return true;
        } else {
            //other errors occurred
            return false;
        }
    }
    return true;

}