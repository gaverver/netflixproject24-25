#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socketMenu.h"
//constructor for the socketMenu
socketMenu::socketMenu(int client_socket) : client_socket(client_socket) {
    bufferSize = sizeof(buffer);
}



