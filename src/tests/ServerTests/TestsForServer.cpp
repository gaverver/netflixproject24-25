#include <gtest/gtest.h>
#include "../../IDataBase.h"
#include "../../DBFile.h"
#include "../../server.h"
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "../../executor.h"
#include "../../ThreadFactory.h"
const int server_port = 5656;
const char* ip_address = "0.0.0.0";
bool passed = true;


void clientFunction(const std::vector<std::string>& messages, std::string& expectedOutput) {
    int clientSocket;
    struct sockaddr_in serveraddress;

    // create socket to communicate with the server
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        return;
    }

    // define the port and ip that the data will be sent to.
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(server_port);
    serveraddress.sin_addr.s_addr = inet_addr(ip_address);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0) {
        close(clientSocket);
        return;
    }
    char buffer[4096];
    std::string Msg = "";
    // Send each message in the vector and get a response from the server.
    for (const auto& message : messages) {
        int sent_data = send(clientSocket, message.c_str(), message.size(), 0);
        // checks if there was a failure in sending the data
        if (sent_data < 0) {
            close(clientSocket);
            return;
        }
        // checks if there was a failure in receiving the data
        int read_bytes = recv(clientSocket, buffer, 4096, 0);
        if (read_bytes < 0) {
            close(clientSocket);
            return;
        }
        // adding the data into the msg variable which keeps the whole output in it.
        buffer[read_bytes] = '\0';
        Msg += buffer;
    }
    // if the output is not as it needs to be, set the global variable to false.
    if (expectedOutput != Msg) {
        passed = false;
    }

    // Close socket
    close(clientSocket);
}

// test a simple one client connection to server
TEST(ServerTesting, OneClientExecutionTest) {
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    // clean up the data before testing
    data.cleanUp();
    // wait for server to start.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<std::string> inputMessages = {
    "POST 1 100 101 102 103\n", 
    "POST 2 101 102 104 105 106\n", 
    "POST 3 100 104 105 107 108\n", 
    "POST 4 101 105 106 107 109 110\n", 
    "POST 5 100 102 103 105 108 111\n", 
    "POST 6 100 103 104 110 111 112 113\n", 
    "POST 7 102 105 106 107 108 109 110\n", 
    "POST 8 101 104 105 106 109 111 114\n", 
    "POST 9 100 103 105 107 112 113 115\n", 
    "POST 10 100 102 105 106 107 109 110 116\n", 
    "GET 1 104\n"
    };
    std::string expectedOutput = "201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n200 Ok\n\n105 106 111 110 112 113 107 108 109 114\n";
    std::thread thread(clientFunction, inputMessages, std::ref(expectedOutput));
    // waits for the client to finish execution
    thread.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
}

// test a multiple clients connection to server
TEST(ServerTesting, MultipleClientExecutionTest) {
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    // clean up the data before testing
    data.cleanUp();
    passed = true;
    // each client execute the commands the are written inside the list by order and the expected output from them. 
    std::string expectedOutput12 = "201 Created\n204 No Content\n";
    std::string expectedOutput3 = "200 Ok\n\nDELETE, arguments: [userid] [movieid1] [movieid2] ...\nGET, arguments: [userid] [movieid]\nPATCH, arguments: [userid] [movieid1] [movieid2] ...\nPOST, arguments: [userid] [movieid1] [movieid2] ...\nhelp\n";
    std::string expectedOutput4 = "400 Bad Request\n400 Bad Request\n";
    std::vector<std::string> inputClient1 = {"POST 1 100\n", "PATCH 1 101\n"};
    std::vector<std::string> inputClient2 = {"POST 2 100\n", "PATCH 2 101\n"};
    std::vector<std::string> inputClient3 = {"help\n"};
    std::vector<std::string> inputClient4 = {"DELETE 2 100 101 c g f\n", "DELETE 1 100 X 101\n"};
    std::thread thread1(clientFunction, inputClient1, std::ref(expectedOutput12));
    std::thread thread2(clientFunction, inputClient2, std::ref(expectedOutput12));
    std::thread thread3(clientFunction, inputClient3, std::ref(expectedOutput3));
    std::thread thread4(clientFunction, inputClient4, std::ref(expectedOutput4));
    // waits for the clients to finish execution
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
    // checks that the data was inserted correctly.
    std::vector<unsigned long int> users100 = data.findMovie(100);
    EXPECT_EQ(users100, std::vector<unsigned long int>({1, 2}));
    std::vector<unsigned long int> users101 = data.findMovie(101);
    EXPECT_EQ(users101, std::vector<unsigned long int>({1, 2}));
}

// test a multiple clients connection to server using the following commands: POST, PATCH, GET
TEST(ServerTesting, POSTPATCHGETTest) {
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    // clean up the data before testing
    data.cleanUp();
    // restore the global variable before running the tests.
    passed = true;
    // each client execute the commands the are written inside the list by order and the expected output from them. 
    std::string expectedOutput2 = "201 Created\n204 No Content\n";
    std::string expectedOutput3 = "201 Created\n";
    std::string expectedOutput1 = "201 Created\n";
    std::vector<std::string> inputClient1 = {"POST 1 1\n"};
    std::vector<std::string> inputClient2 = {"POST 2 1\n", "PATCH 2 3\n"};
    std::vector<std::string> inputClient3 = {"POST 3 2\n"};
    std::thread thread1(clientFunction, inputClient1, std::ref(expectedOutput1));
    std::thread thread2(clientFunction, inputClient2, std::ref(expectedOutput2));
    std::thread thread3(clientFunction, inputClient3, std::ref(expectedOutput3));
    // waits for the clients to finish execution
    thread1.join();
    thread2.join();
    thread3.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
    passed = true;
    // checks that the data was inserted correctly.
    std::vector<unsigned long int> users1 = data.findMovie(1);
    EXPECT_EQ(users1, std::vector<unsigned long int>({1, 2}));
    expectedOutput1 = "200 Ok\n\n3 2\n";
    expectedOutput2 = "200 Ok\n\n3\n";
    expectedOutput3 = "200 Ok\n\n2\n";
    inputClient1 = {"GET 1 1\n"};
    inputClient2 = {"GET 1 2\n"};
    inputClient3 = {"GET 1 3\n"};
    // execute GET commands - each client execute the commands the are written inside the list by order and the expected output from them. 
    std::thread GetThread1(clientFunction, inputClient1, std::ref(expectedOutput1));
    std::thread GetThread2(clientFunction, inputClient2, std::ref(expectedOutput2));
    std::thread GetThread3(clientFunction, inputClient3, std::ref(expectedOutput3));
    // waits for the clients to finish execution
    GetThread1.join();
    GetThread2.join();
    GetThread3.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
}

// test a multiple clients connection to server using the following commands: POST, PATCH, DELETE
TEST(ServerTesting, POSTPATCHDELETETest) {
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    // clean up the data before testing
    data.cleanUp();
    // restore the global variable before running the tests.
    passed = true;
    // each client execute the commands the are written inside the list by order and the expected output from them. 
    std::string expectedOutput2 = "201 Created\n204 No Content\n";
    std::string expectedOutput3 = "201 Created\n204 No Content\n";
    std::string expectedOutput1 = "201 Created\n204 No Content\n204 No Content\n";
    std::vector<std::string> inputClient1 = {"POST 1 1\n", "PATCH 1 5 6 8 8 9\n", "PATCH 1 70 70 80 90\n"};
    std::vector<std::string> inputClient2 = {"POST 2 1 1\n", "PATCH 2 3\n"};
    std::vector<std::string> inputClient3 = {"POST 3 2\n", "PATCH 3 10 10 10 11 12\n"};
    std::thread thread1(clientFunction, inputClient1, std::ref(expectedOutput1));
    std::thread thread2(clientFunction, inputClient2, std::ref(expectedOutput2));
    std::thread thread3(clientFunction, inputClient3, std::ref(expectedOutput3));
    // waits for the clients to finish execution
    thread1.join();
    thread2.join();
    thread3.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
    passed = true;
    // checks that the data was inserted correctly.
    std::vector<unsigned long int> users1 = data.findMovie(1);
    EXPECT_EQ(users1, std::vector<unsigned long int>({1, 2}));
    std::vector<unsigned long int> movies1 = data.findUser(1);
    EXPECT_EQ(movies1, std::vector<unsigned long int>({1, 5, 6, 8, 9, 70, 80, 90}));
    std::string expectedDeleteOutput = "204 No Content\n204 No Content\n";
    inputClient1 = {"DELETE 1 1 5 8\n", "DELETE 2 1 3\n"};
    inputClient2 = {"DELETE 1 70 80\n", "DELETE 3 10\n"};
    inputClient3 = {"DELETE 3 11\n", "DELETE 3 12\n"};
    // execute DELETE commands - each client execute the commands the are written inside the list by order and the expected output from them. 
    std::thread DeleteThread1(clientFunction, inputClient1, std::ref(expectedDeleteOutput));
    std::thread DeleteThread2(clientFunction, inputClient2, std::ref(expectedDeleteOutput));
    std::thread DeleteThread3(clientFunction, inputClient3, std::ref(expectedDeleteOutput));
    // waits for the clients to finish execution
    DeleteThread1.join();
    DeleteThread2.join();
    DeleteThread3.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
    // checks that the data was deleted correctly.
    movies1 = data.findUser(1);
    EXPECT_EQ(movies1, std::vector<unsigned long int>({6, 9, 90})); 
    std::vector<unsigned long int> movies2 = data.findUser(2);
    EXPECT_EQ(movies2, std::vector<unsigned long int>({}));
    // all of his movies were deleted but he should still exist.
    EXPECT_TRUE(data.isUserExists(2));
    std::vector<unsigned long int> movies3 = data.findUser(3);
    EXPECT_EQ(movies3, std::vector<unsigned long int>({2}));
}

// test a multiple clients connection to server using invalid commands.
TEST(ServerTesting, InvalidInputTest) {
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    // clean up the data before testing
    data.cleanUp();
    // restore the global variable before running the tests.
    passed = true;
    // each client execute the commands the are written inside the list by order and the expected output from them. 
    std::string expectedOutput2 = "404 Not Found\n201 Created\n404 Not Found\n";
    std::string expectedOutput3 = "404 Not Found\n404 Not Found\n400 Bad Request\n200 Ok\n\nDELETE, arguments: [userid] [movieid1] [movieid2] ...\nGET, arguments: [userid] [movieid]\nPATCH, arguments: [userid] [movieid1] [movieid2] ...\nPOST, arguments: [userid] [movieid1] [movieid2] ...\nhelp\n";
    std::string expectedOutput1 = "400 Bad Request\n400 Bad Request\n400 Bad Request\n400 Bad Request\n400 Bad Request\n";
    std::vector<std::string> inputClient1 = {"foo\n", "help x\n", "PATCH 10\n", "post 20\n", "DELETE 4 C\n"};
    std::vector<std::string> inputClient2 = {"PATCH 1 2\n", "POST 1 2\n", "POST 1 3\n"};
    std::vector<std::string> inputClient3 = {"DELETE 1 2 3\n", "GET 2 1\n", "GET 1\n", "help\n"};
    std::thread thread1(clientFunction, inputClient1, std::ref(expectedOutput1));
    std::thread thread2(clientFunction, inputClient2, std::ref(expectedOutput2));
    std::thread thread3(clientFunction, inputClient3, std::ref(expectedOutput3));
    // waits for the clients to finish execution
    thread1.join();
    thread2.join();
    thread3.join();
    // checks that the output that every client got is correct.
    EXPECT_TRUE(passed);
    // checks that the data was inserted correctly.
    std::vector<unsigned long int> movies1 = data.findUser(1);
    EXPECT_EQ(movies1, std::vector<unsigned long int>({2})); 
}
int main(int argc, char **argv) {
    ThreadFactory tf;
    executor& exec = tf;
    server test(server_port, exec);
    std::thread serverThread(&server::start, &test);
    serverThread.detach();
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}