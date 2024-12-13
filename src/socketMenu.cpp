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
    int read_bytes =  recv(client_socket, buffer, bufferSize,0);
    //connection never closed so read_bytes=0 dosen't close it
    if (read_bytes < 0) {
        //continue the program
        return "";
    }
    //add \0 so the string knows where to stop
    buffer[read_bytes] = '\0';
    std::string s(buffer);
    //return the string
    return s;
}

void SocketMenu::print(std::string output) {
    send(client_socket, output.data(), output.size(), 0);
}