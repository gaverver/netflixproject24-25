#include <gtest/gtest.h>
#include "../../ConsoleMenu.h"
#include "../../IMenu.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

// helper function to capture input from cout
std::string captureOutput(const std::string& output) {
    ConsoleMenu console;
    std::ostringstream outputBuffer;
    // Get the current buffer of cout and save it so it will be possible to restore later 
    std::streambuf* originalBuffer = std::cout.rdbuf();
    // Redirect cout to a stringstream so we could get to the output easily
    std::cout.rdbuf(outputBuffer.rdbuf()); 
    // Call the function that prints to the console
    console.print(output); 
    // Restore the original buffer, so that the next outputs will go there
    std::cout.rdbuf(originalBuffer);
    // Return the output of the function
    return outputBuffer.str();
}

// helper function to capture input from cin
std::string captureInput(const std::string& input) {
    ConsoleMenu console;
    // simulate user's input - it will be input
    std::istringstream inputBuffer(input);
    // Get the current buffer of cin and save it so it will be possible to restore later 
    std::streambuf* originalBuffer = std::cin.rdbuf();
    // Redirect cin to a stringstream so we could get to the input easily
    std::cin.rdbuf(inputBuffer.rdbuf());
    // Call the function scan to get the input
    std::string returnedVal = console.scan();
    // Restore the original buffer, so that the next inputs will go there
    std::cin.rdbuf(originalBuffer);
    // Return the output of the function
    return returnedVal;
}

// helper function to get the vector that represents the user's input
 std::vector<std::string> captureVector(const std::string& input) {
    ConsoleMenu console;
    // simulate user's input - it will be simulation
    std::istringstream inputBuffer(input);
    // Get the current buffer of cin and save it so it will be possible to restore later 
    std::streambuf* originalBuffer = std::cin.rdbuf();
    std::cin.rdbuf(inputBuffer.rdbuf());
    // Get the vector of commands(the command name and args)
    std::vector<std::string> returnedVal = console.nextCommand();
    // Restore the original buffer, so that the next inputs will go there
    std::cin.rdbuf(originalBuffer);
    return returnedVal;

 }
// test print function of ConsoleMenu class.
TEST(ConsoleMenuTests, PrintTest1) {
    const std::string expected = "Hello World!\n";
    std::string output = captureOutput("Hello World!");
    // print should print the given string to the buffer - so it is checked here.
    EXPECT_EQ(expected, output);
}

// test print function of ConsoleMenu class.
TEST(ConsoleMenuTests, PrintTest2) {
    const std::string expected = "Netflix is the best app in the world!\n";
    std::string output = captureOutput("Netflix is the best app in the world!");
    // print should print the given string to the buffer - so it is checked here.
    EXPECT_EQ(expected, output);
}

// test print function of ConsoleMenu class.
TEST(ConsoleMenuTests, PrintTest3) {
    const std::string expected = "Shlomi, no nervous\n";
    std::string output = captureOutput("Shlomi, no nervous");
    // print should print the given string to the buffer - so it is checked here.
    EXPECT_EQ(expected, output);
}

// test print function of ConsoleMenu class.
TEST(ConsoleMenuTests, PrintEmptyString) {
    const std::string expected = "\n";
    std::string output = captureOutput("");
    // print should print the given string to the buffer - so it is checked here.
    EXPECT_EQ(expected, output);
}

// test scan function of ConsoleMenu class.
TEST(ConsoleMenuTests, ScanTest1) {
    const std::string expected = "help";
    std::string returnedVal = captureInput("help");
    // check if the returned value from scan function is equal to the input
    EXPECT_EQ(expected, returnedVal);
}

// test scan function of ConsoleMenu class.
TEST(ConsoleMenuTests, ScanTest2) {
    const std::string expected = "add 23 5 6 8 9";
    std::string returnedVal = captureInput("add 23 5 6 8 9");
    // check if the returned value from scan function is equal to the input
    EXPECT_EQ(expected, returnedVal);
}

// test scan function of ConsoleMenu class.
TEST(ConsoleMenuTests, ScanTest3) {
    const std::string expected = "recommand 56 5 6 8 45 1";
    std::string returnedVal = captureInput("recommand 56 5 6 8 45 1");
    // check if the returned value from scan function is equal to the input
    EXPECT_EQ(expected, returnedVal);
}

// test scan function of ConsoleMenu class.
TEST(ConsoleMenuTests, ScanEmptyTest) {
    const std::string simulation = "";
    std::string returnedVal = captureInput("");
    // check if the returned value from scan function is equal to the input
    EXPECT_EQ(simulation, returnedVal);
}

// test NextCommand function of ConsoleMenu class.
TEST(ConsoleMenuTests, RegularNextCommandTest) {
    const std::string simulation = "recommand 56 45";
    // Simulate user's input and get the vector that represents his input
    std::vector<std::string> returnedVal = captureVector(simulation);
    // Construct the expected return value as the function should split the string by spaces into a vector
    std::vector<std::string> expectedOutput(3);
    expectedOutput[0] = "recommand";
    expectedOutput[1] = "56";
    expectedOutput[2] = "45";
    // check if the returned value from the function is equal to what expected
    EXPECT_EQ(returnedVal, expectedOutput);
}

// test NextCommand function of ConsoleMenu class.
TEST(ConsoleMenuTests, MultipleSpaceTest1) {
    const std::string simulation = "add    98 111";
    // Simulate user's input and get the vector that represents his input
    std::vector<std::string> returnedVal = captureVector(simulation);
    // Construct the expected return value as the function should split the string and not include spaces into a vector
    std::vector<std::string> expectedOutput(3);
    expectedOutput[0] = "add";
    expectedOutput[1] = "98";
    expectedOutput[2] = "111";
    // check if the returned value from the function is equal to what expected
    EXPECT_EQ(returnedVal, expectedOutput);
}

// test NextCommand function of ConsoleMenu class.
TEST(ConsoleMenuTests, MultipleSpaceTest2) {
    const std::string simulation = "recommand    98          111    c";
    // Simulate user's input and get the vector that represents his input
    std::vector<std::string> returnedVal = captureVector(simulation);
    // Construct the expected return value as the function should split the string and not include spaces into a vector
    std::vector<std::string> expectedOutput(4);
    expectedOutput[0] = "recommand";
    expectedOutput[1] = "98";
    expectedOutput[2] = "111";
    expectedOutput[3] = "c";
    // check if the returned value from the function is equal to what expected
    EXPECT_EQ(returnedVal, expectedOutput);
}

// test NextCommand function of ConsoleMenu class.
TEST(ConsoleMenuTests, EmptyCommandTest) {
    const std::string simulation = "";
    // Simulate user's input and get the vector that represents his input
    std::vector<std::string> returnedVal = captureVector(simulation);
    // Construct the expected return value as the function should split the string by spaces into a vector
    std::vector<std::string> expectedOutput(1);
    expectedOutput[0] = "";
    // check if the returned value from the function is equal to what expected
    EXPECT_EQ(returnedVal, expectedOutput);
}

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
