#include <gtest/gtest.h>
#include "../../ICommand.h"
#include "../../addCommand.h"
#include "../../GETCommand.h"
#include "../../helpCommand.h"
#include "../../IDataBase.h"
#include "../../DBFile.h"
#include "../../ConsoleMenu.h"
#include "../../IMenu.h"
#include "AppTester.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

// global DB variable so that the data will clean up only in the beggining and everyone will know it.
DBFile dbfile("../data");
IDataBase& data = dbfile;


// helper function to simulate the user's output and capture the output from cout
std::string captureOutput(std::vector<std::string> commands) {
    std::ostringstream outputBuffer;
    // Get the current buffer of cout and save it so it will be possible to restore later 
    std::streambuf* originalOutputBuffer = std::cout.rdbuf();
    // Redirect cout to a stringstream so we could get to the output easily
    std::cout.rdbuf(outputBuffer.rdbuf()); 
    // Create a stringstream to hold all input data as if they were typed in order
    std::string x = "";
    for (std::string input : commands) {
        // simulate pressing enter after every input
        x += input + "\n";
    }

    // Redirect `std::cin` to read from the combined input buffer so we could get to the input easily
    std::istringstream inputBuffer(x);
    // Save the original std::cin buffer
    std::streambuf* originalInputBuffer = std::cin.rdbuf(); 
    // Redirect cin to a stringstream so we could get to the input easily
    std::cin.rdbuf(inputBuffer.rdbuf()); 
    
    // construct the app to be able to run it.
    std::map<std::string, ICommand*> commands2;
    std::vector<ICommand*> helpCommands;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* add = new addCommand(data);
    ICommand* GET = new GETCommand(data, menu);
    ICommand* help = new helpCommand(helpCommands, menu);
    helpCommands.push_back(add);
    helpCommands.push_back(GET);
    helpCommands.push_back(help);
    // map the commands to the appropriate instances of commands
    commands2["add"] = add;
    commands2["help"] = help;
    commands2["GET"] = GET;
    AppTester app(commands2, menu);
    // Call the function that runs the list of commands from the user
    app.run(commands.size());


    // Restore the original output buffer, so that the next outputs will go there
    std::cout.rdbuf(originalOutputBuffer);
    // Restore the original input buffer, so that the next inputs will go there
    std::cin.rdbuf(originalInputBuffer);
    // Return the output of the function
    return outputBuffer.str();
}


// test that checks the help command used from app
TEST(AppTests, HelpCommandTest) {
    // run the help command
    std::vector<std::string> commands = {"help"};
    // the expected output from running help - getting a list of all commands available
    std::string expectedOutput = "add [userid] [movieid1] [movieid2] ...\nGET [userid] [movieid]\nhelp\n";
    std::string printedOutput = captureOutput(commands);
    // comparing the output with the expected one
    EXPECT_EQ(expectedOutput, printedOutput);
}


TEST(AppTests, AddAndGETTest1) {
    // clear the databse before usage so there will not be data that is not needed there.
    data.cleanUp();
    // run the example from class about the add and GET command.
    std::vector<std::string> commands = {"add 1 100 101 102 103", "add 2 101 102 104 105 106", "add 3 100 104 105 107 108", "add 4 101 105 106 107 109 110", "add 5 100 102 103 105 108 111", "add 6 100 103 104 110 111 112 113", "add 7 102 105 106 107 108 109 110", "add 8 101 104 105 106 109 111 114", "add 9 100 103 105 107 112 113 115", "add 10 100 102 105 106 107 109 110 116", "GET 1 104"};
    // the expected output as shown in the exercise
    std::string expectedOutput = "105 106 111 110 112 113 107 108 109 114\n";
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    EXPECT_EQ(expectedOutput, printedOutput);
    // clear the data for next usages
    data.cleanUp();
}

TEST(AppTests, AddAndHelpTest) {
    // clear the databse before usage so there will not be data that is not needed there.
    data.cleanUp();
    std::vector<std::string> commands = {"add 1 100 101 102 103", "add 2 101 102 104 105 106", "add 3 100 104 105 107 108", "add 4 101 105 106", "add 4 107 109 110", "help", "add 5 100 102 103 105 108 111", "add 6 100 103 104 110 111 112 113", "add 7 102 105 106 107 108 109 110", "add 8 101 104 105 106 109 111 114"};
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    // the only command that outputs something here is help - we check that it does it correctly
    std::string expectedOutput = "add [userid] [movieid1] [movieid2] ...\nGET [userid] [movieid]\nhelp\n";
    EXPECT_EQ(expectedOutput, printedOutput);
    // get the movies from the database of user 4.
    std::vector<unsigned long int> movies4 = data.findUser(4);
    std::vector<unsigned long int> expectedMovies4 = {101, 105, 106, 107, 109, 110};
    // we check that the user was succefully registered inside the database
    EXPECT_EQ(movies4, expectedMovies4);
}

TEST(AppTests, NonVolatileDataTest) {
    // we didn't clear the database - we want to check that the data from last commands were saved even if we stopped running the program.
    std::vector<unsigned long int> movies4 = data.findUser(4);
    std::vector<unsigned long int> expectedMovies4 = {101, 105, 106, 107, 109, 110};
    // we check that the user was succefully registered inside the database
    EXPECT_EQ(movies4, expectedMovies4);
    // run other commands to complete the example from class
    std::vector<std::string> commands = {"add 9 100 103 105 107 112 113 115", "add 10 100 102 105 106 107 109 110 116", "GET 1 104"};
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    // check that the output is as it should be - meaning that the data was saved succefully inside the database
    std::string expectedOutput = "105 106 111 110 112 113 107 108 109 114\n";
    EXPECT_EQ(expectedOutput, printedOutput);
    // clear the data for next usages
    data.cleanUp();
}

TEST(AppTests, MultipleSpacesTest) {
    // clear the databse before usage so there will not be data that is not needed there.

    data.cleanUp();
    // run commands with multiple spaces between words - this is the same example from the exercise.
    std::vector<std::string> commands = {"add 1 100     101 102    103", "add   2 101   102   104 105 106", "add 3 100 104 105   107 108", "add 4 101 105   106 107 109 110", "add 5   100 102   103 105 108 111", "add 6 100 103 104 110 111    112 113", "add 7 102 105 106 107 108   109 110", "add 8 101 104 105   106 109 111 114", "add 9 100 103   105 107 112 113   115", "add 10 100 102 105 106   107 109 110   116", "GET 1    104"};
    std::string expectedOutput = "105 106 111 110 112 113 107 108 109 114\n";
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    EXPECT_EQ(expectedOutput, printedOutput);
    // clear the data for next usages
    data.cleanUp();
}

TEST(AppTests, AddAndGETTest2) {
    // clear the databse before usage so there will not be data that is not needed there.
    data.cleanUp();
    // enter some add commands and GET command to simulate possible input.
    std::vector<std::string> commands = {"add 1 1", "add 2 1 3", "add 3 2", "GET 1 2"};
    std::string expectedOutput = "3\n";
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    EXPECT_EQ(expectedOutput, printedOutput);
    // clear the data for next usages
    data.cleanUp();
}

TEST(AppTests, DuplicateMoviesTest) {
    // clear the databse before usage so there will not be data that is not needed there.
    data.cleanUp();
    // this is the example from the exercise but we add duplicte movies to users in the add command - we want to check that it doesn't affect the GETation and the saving in the db
    std::vector<std::string> commands = {"add 1 100 101 102 103", "add 2 101 102 104 104 104 104 105 106 105", "add 3 100 104 105 105 107 108", "add 4 101 105 106 107 107 109 110", "add 5 100 102 103 105 108 111", "add 6 100 103 104 110 111 112 112 112 113", "add 7 102 105 106 106 106 107 108 109 110", "add 8 101 104 105 106 109 111 114", "add 9 100 103 103 103 105 107 112 113 115", "add 10 100 102 105 106 107 109 110 116 116 116 116", "GET 1 104"};
    std::string expectedOutput = "105 106 111 110 112 113 107 108 109 114\n";
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    EXPECT_EQ(expectedOutput, printedOutput);
    // check that the movies that user 2 has watched were saved properly in the db - without duplicates
    std::vector<unsigned long int> movies2 = data.findUser(2);
    std::vector<unsigned long int> expectedMovies2 = {101, 102, 104, 105, 106};
    EXPECT_EQ(movies2, expectedMovies2);
    // clear the data for next usages
    data.cleanUp();
}

TEST(AppTests, InvalidInputTest1) {
    // clear the databse before usage so there will not be data that is not needed there.
    data.cleanUp();
    // enter some illegal commands and add and GET commands
    std::vector<std::string> commands = {"add 1 1", "add 2 1 3", "add 3 2", "add 1 2 3 5 4 c", "add 2 p 3 c", "add f 1 1 1 2 2 2 3 3 3", "GET 1 2", "GET 1 c"};
    std::string expectedOutput = "3\n";
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    EXPECT_EQ(expectedOutput, printedOutput);
    // check that the movies that user 1 has watched were saved properly in the db - we check that the invalid output didn't affect it
    std::vector<unsigned long int> movies1 = data.findUser(1);
    std::vector<unsigned long int> expectedMovies1 = {1};
    EXPECT_EQ(movies1, expectedMovies1);
    // check that the users that watched movie 5 were saved properly in the db - we check that the invalid output didn't affect it
    std::vector<unsigned long int> users5 = data.findMovie(5);
    std::vector<unsigned long int> expectedUsers5 = {};
    EXPECT_EQ(users5, expectedUsers5);

    // clear the data for next usages
    data.cleanUp();
}

TEST(AppTests, InvalidInputTest2) {
    // clear the databse before usage so there will not be data that is not needed there.
    data.cleanUp();
    // enter some commands that doesn't exist and add and help command 
    std::vector<std::string> commands = {"foo", "bar", "help", "add 1  2", "itzik", "GET 1 2 3"};
    std::string expectedOutput = "add [userid] [movieid1] [movieid2] ...\nGET [userid] [movieid]\nhelp\n";
    // capture the output from the cout
    std::string printedOutput = captureOutput(commands);
    EXPECT_EQ(expectedOutput, printedOutput);
    // check that the movies that user 1 has watched were saved properly in the db - we check that the invalid output didn't affect it
    std::vector<unsigned long int> movies = data.findUser(1);
    std::vector<unsigned long int> expectedMovies = {2};
    EXPECT_EQ(movies, expectedMovies);
    // check that the users that watched movie 2 were saved properly in the db - we check that the invalid output didn't affect it
    std::vector<unsigned long int> users = data.findMovie(2);
    std::vector<unsigned long int> expectedUser = {1};
    EXPECT_EQ(users, expectedUser);
    // clear the data for next usages
    data.cleanUp();
}