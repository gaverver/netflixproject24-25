#include <gtest/gtest.h>
#include "../../helpCommand.h"
#include "../../ICommand.h"
#include "../../IMenu.h"
#include "../../ConsoleMenu.h"
#include <string>
#include <vector>

// helper function to capture output from cout
std::string captureOutput(ICommand* Runner, const std::vector<std::string>& args) {
    ConsoleMenu console;
    std::ostringstream outputBuffer;
    // Get the current buffer of cout and save it so it will be possible to restore later 
    std::streambuf* originalBuffer = std::cout.rdbuf();
    // Redirect cout to a stringstream so we could get to the output easily
    std::cout.rdbuf(outputBuffer.rdbuf()); 
    // Call the function execute - it prints to the screen the commands that the user can use. 
    Runner -> execute(args); 
    // Restore the original buffer, so that the next outputs will go there
    std::cout.rdbuf(originalBuffer);
    // Return the output of the function
    return outputBuffer.str();
}

// Test that checks that the return value of the description function is as it should be 
TEST(HelpTesting, DescriptionTest) {
    std::vector<ICommand*> commands;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    helpCommand* h = new helpCommand(commands, menu);
    // checks if the return value of the function is equal to "help" - as it should because it describes the command
    EXPECT_EQ(h->description(), "help");
}

// Test that checks that the return value of the rw_status function
TEST(HelpTesting, rw_statusTest) {
    std::vector<ICommand*> commands;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    helpCommand* h = new helpCommand(commands, menu);
    // checks if the return value of the function is equal to "none" - as it should because help doesn't read/write
    EXPECT_EQ(h->rw_status(), "none");
}

// test for execute function
TEST(HelpTesting, ExecuteTest1) {
    std::vector<std::string> args;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    std::vector<ICommand*> commands;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands, menu);
    ICommand* help1 = new helpCommand(commands, menu);
    ICommand* help2 = new helpCommand(commands, menu);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    ICommand* helpRunner = new helpCommand(commands, menu);
    // run the execute function and capture it's output 
    std::string output = captureOutput(helpRunner, args);
    // the expected output is the description of each command that is in the vector
    std::string expected = "200 Ok\n\nhelp\nhelp\nhelp\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

// test for execute function
TEST(HelpTesting, ExecuteTest2) {
    std::vector<std::string> args;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    std::vector<ICommand*> commands;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands, menu);
    ICommand* help1 = new helpCommand(commands, menu);
    ICommand* help2 = new helpCommand(commands, menu);
    ICommand* help3 = new helpCommand(commands, menu);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    commands.push_back(help3);
    ICommand* helpRunner = new helpCommand(commands, menu);
    // run the execute function and capture it's output 
    std::string output = captureOutput(helpRunner, args);
    // the expected output is the description of each command that is in the vector
    std::string expected = "200 Ok\n\nhelp\nhelp\nhelp\nhelp\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

// test for execute function
TEST(HelpTesting, ExecuteFromVector) {
    std::vector<std::string> args;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    std::vector<ICommand*> commands;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands, menu);
    ICommand* help1 = new helpCommand(commands, menu);
    ICommand* help2 = new helpCommand(commands, menu);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    // run the execute function with an instance of help that is already in the vector and capture it's output 
    std::string output = captureOutput(help, args);
    // the expected output is the description of each command that is in the vector
    std::string expected = "200 Ok\n\nhelp\nhelp\nhelp\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

// test for execute function
TEST(HelpTesting, InvalidInputTest) {
    std::vector<std::string> args;
    args.push_back("x");
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    std::vector<ICommand*> commands;
    // create instances of helpCommand class
    ICommand* help = new helpCommand(commands, menu);
    ICommand* help1 = new helpCommand(commands, menu);
    ICommand* help2 = new helpCommand(commands, menu);
    // push them into the vector of available commands
    commands.push_back(help);
    commands.push_back(help1);
    commands.push_back(help2);
    // run the execute function with an instance of help that is already in the vector and capture it's output 
    std::string output = captureOutput(help, args);
    // it is an invalid command - so the expected output is 400 Bad Request.
    std::string expected = "400 Bad Request\n";
    // compare between the outputs - we check if the output is as it should be
    EXPECT_EQ(output, expected);
}

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }