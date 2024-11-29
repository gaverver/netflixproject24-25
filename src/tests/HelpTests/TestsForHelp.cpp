#include <gtest/gtest.h>
#include "../../help.h"
#include "../../ICommand.h"
#include "../../IMenu.h"
#include "../../ConsoleMenu.h"
#include <string>
#include <vector>

// helper function to capture output from cout
std::string captureOutput(ICommand* Runner) {
    ConsoleMenu console;
    std::ostringstream outputBuffer;
    // Get the current buffer of cout and save it so it will be possible to restore later 
    std::streambuf* originalBuffer = std::cout.rdbuf();
    // Redirect cout to a stringstream so we could get to the output easily
    std::cout.rdbuf(outputBuffer.rdbuf()); 
    // Call the function execute - it prints to the screen the commands that the user can use. 
    Runner -> execute(); 
    // Restore the original buffer, so that the next outputs will go there
    std::cout.rdbuf(originalBuffer);
    // Return the output of the function
    return outputBuffer.str();
}

// Test that checks that the return value of the description function is as it should be 
TEST(HelpTesting, DescriptionTest) {
    Help* h = new helpCommand();
    // checks if the return value of the function is equal to "help" - as it should because it describes the command
    EXPECT_EQ(h->description(), "help");
}

// test for execute function
TEST(HelpTesting, ExecuteTest1) {
    IMenu* menu = new ConsoleMenu();
    std::vector<ICommand*> commands;
    std::vector<std::string> args;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands);
    ICommand* help1 = new helpCommand(commands);
    ICommand* help2 = new helpCommand(commands);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    ICommand* helpRunner = new helpCommand(commands);
    // run the execute function and capture it's output 
    std::string output = captureOutput(helpRunner);
    // the expected output is the description of each command that is in the vector
    std::string expected = "help\nhelp\nhelp\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

// test for execute function
TEST(HelpTesting, ExecuteTest2) {
    IMenu* menu = new ConsoleMenu();
    std::vector<ICommand*> commands;
    std::vector<std::string> args;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands);
    ICommand* help1 = new helpCommand(commands);
    ICommand* help2 = new helpCommand(commands);
    ICommand* help3 = new helpCommand(commands);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    commands.push_back(help3);
    ICommand* helpRunner = new helpCommand(commands);
    // run the execute function and capture it's output 
    std::string output = captureOutput(helpRunner);
    // the expected output is the description of each command that is in the vector
    std::string expected = "help\nhelp\nhelp\nhelp\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

// test for execute function
TEST(HelpTesting, ExecuteFromVector) {
    IMenu* menu = new ConsoleMenu();
    std::vector<ICommand*> commands;
    std::vector<std::string> args;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands);
    ICommand* help1 = new helpCommand(commands);
    ICommand* help2 = new helpCommand(commands);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    // run the execute function with an instance of help that is already in the vector and capture it's output 
    std::string output = captureOutput(help);
    // the expected output is the description of each command that is in the vector
    std::string expected = "help\nhelp\nhelp\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}