#include <gtest/gtest.h>
#include "../../DBFile.h"
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "../../SocketMenu.h"

const int server_port = 5555;
int client_socket;
int server_socket;
int sock;
//creates socket for the server
void serverFunction() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    // if (sock < 0) {
    //     perror("error creating socket");
    // }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding to socket");
    }
    if (listen(sock,5)<0)
    {
        perror("error listening to a socket");
    }
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    client_socket = accept(sock, (struct sockaddr*) &client_sin, &addr_len);
    if (client_socket < 0)
    {
        perror("error accepting client");
    }
}

void closeAll() {
    close(server_socket);
    close(client_socket);
    close(sock);
}

void connectToServer() {
    //allow server to start
    sleep(1);

    const char* ip_address = "127.0.0.1";
    //create socket for the client
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("error creating socket");
    }
    //information of the socket address (server)
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(server_port);
    if (connect(server_socket, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
        perror("error connecting to server");
    }
}

//sends data to the server
void sendToServer(const std::string& data) {
    //send data
    int sent_bytes = send(server_socket, data.data(), data.size(), 0);
    if (sent_bytes < 0) {
        // perror("error writing to socket");
    }
}

int readFromServer(char *buffer, int data_len) {
    int read_bytes = recv(server_socket, buffer, data_len, 0);
    buffer[read_bytes] = '\0';
    return read_bytes;
}

//testing scan function
TEST(SocketMenuTesting, Scan) {
    //initialize server and establish connection with the client
    std::thread serverThread(serverFunction);
    std::thread clientThread(connectToServer);

    serverThread.join();
    clientThread.join();


    //initializing socket menu for the server
    SocketMenu sm(client_socket);
    //regular test
    std::string expected = "test 1";
    //send to the server
    sendToServer(expected + "\n");
    //scan using socketmenu
    std::string output = sm.scan();
    EXPECT_EQ(expected, output);
    //check if it's ok if we send another time
    expected = "test 2";
    //send to the server
    sendToServer(expected + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(expected, output);
    //send a message consists of 2 messages
    expected = "test 3\ntest 4";
    //send to the server
    sendToServer(expected + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(expected, output);
    
    //send single char
    expected = "t";
    //send to the server
    sendToServer(expected + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(expected, output);
    
    //send a message larger than 4096 bytes
    std::string str(5000, 'a');
    sendToServer(str + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(str, output);
    
    //send a message of 4096 +- 1 bytes
    std::string str1(4094, 'a');
    sendToServer(str1 + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(str1, output);

    std::string str2(4095, 'a');
    sendToServer(str2 + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(str2, output);

    //send an empty message
    expected = "";
    //send to the server
    sendToServer(expected + "\n");
    //scan using socketmenu
    output = sm.scan();
    EXPECT_EQ(expected, output);

    //close the server
    closeAll();

}
//testing nextCommand function
TEST(SocketMenuTesting, NextCommand) {
    //initialize server and establish connection with the client
    std::thread serverThread(serverFunction);
    std::thread clientThread(connectToServer);

    serverThread.join();
    clientThread.join();


    //initializing socket menu for the server
    SocketMenu sm(client_socket);
    //regular test for nextCommand
    std::string command = "recommend 56 45";
    sendToServer(command + "\n");
    std::vector<std::string> output = sm.nextCommand();
    EXPECT_EQ(output, (std::vector<std::string>{"recommend", "56", "45"}));
    //check if spaces change anything
    command = "add    98 111";
    sendToServer(command + "\n");
    output = sm.nextCommand();
    EXPECT_EQ(output, (std::vector<std::string>{"add", "98", "111"}));
    //check another variation of spaces
    command = "recommand    98          111    c";
    sendToServer(command + "\n");
    output = sm.nextCommand();
    EXPECT_EQ(output, (std::vector<std::string>{"recommand", "98", "111", "c"}));
    //check command with no arguments
    command = "delete";
    sendToServer(command + "\n");
    output = sm.nextCommand();
    EXPECT_EQ(output, (std::vector<std::string>{"delete"}));
    //check empty command
    command = "";
    sendToServer(command + "\n");
    output = sm.nextCommand();
    EXPECT_EQ(output, (std::vector<std::string>{""}));
    //close the server
    closeAll();
}
//testing print function
TEST(SocketMenuTesting, Print) {
    //initialize server and establish connection with the client
    std::thread serverThread(serverFunction);
    std::thread clientThread(connectToServer);

    serverThread.join();
    clientThread.join();


    //initializing socket menu for the server
    SocketMenu sm(client_socket);
    //create buffer for reading
    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    std::string expected;
    std::string output;
    //regular test for print
    expected = "test 1";
    sm.print(expected);
    readFromServer(buffer, expected_data_len);
    output = buffer;
    expected += "\n";
    EXPECT_EQ(expected, output);

    //printing another time
    expected = "test 2";
    sm.print(expected);
    readFromServer(buffer, expected_data_len);
    output = buffer;
    expected += "\n";
    EXPECT_EQ(expected, output);

    //printing two lines
    expected = "test 3\ntest 4";
    sm.print(expected);
    readFromServer(buffer, expected_data_len);
    output = buffer;
    expected += "\n";
    EXPECT_EQ(expected, output);

    //printing empty line
    expected = "";
    sm.print(expected);
    readFromServer(buffer, expected_data_len);
    output = buffer;
    expected += "\n";
    EXPECT_EQ(expected, output);

    //close the server
    closeAll();
}