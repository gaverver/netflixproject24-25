#include <gtest/gtest.h>
#include "../../ICommand.h"
#include "../../IDataBase.h"
#include "../../DBFile.h"
#include "../../DELETECommand.h"
#include "../../IMenu.h"
#include "../../ConsoleMenu.h"
#include <fstream>
#include <vector>
#include <string>

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
TEST(DELETETesting, DescriptionTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* del = new DELETECommand(test, menu);
    // checks description output
    EXPECT_EQ(del->description(), "DELETE, arguments: [userid] [movieid1] [movieid2] ...");
    delete del;
}

// Test rwstatus
TEST(DELETETesting, rwSTtatusTests) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* del = new DELETECommand(test, menu);
    // checks status
    EXPECT_EQ(del->rw_status(), "writer");
    delete del;
}

// regular test
TEST(DELETETesting, ExecuteTest) {
    DBFile dbf("../data");
    IDataBase& test = dbf;
    // clear the files before testing
    test.cleanUp();
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* del = new DELETECommand(test, menu);
    std::string output;
    std::string s204 = "204 No Content\n";
    std::string s404 = "404 Not Found\n";
    std::string s400 = "400 Bad Request\n";
    //initialize
    dbf.updateUser(1, {101,102,103,104,105,106,107,108,109,110});
    //remove last two
    // dbf.deleteMovies(1,{110,109});
    output = captureOutput(del, {"1", "110", "109"});
    EXPECT_EQ(output, s204);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,102,103,104,105,106,107,108}));
    //remove first three
    // dbf.deleteMovies(1,{101,103,102});
    output = captureOutput(del, {"1", "101", "103", "102"});
    EXPECT_EQ(output, s204);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{104,105,106,107,108}));
    //remove middle three
    // dbf.deleteMovies(1,{106,107,105});
    output = captureOutput(del, {"1", "106", "107", "105"});
    EXPECT_EQ(output, s204);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{104,108}));
    //add some ids
    dbf.updateUser(1,{100,106,105,101,109,110});
    //remove the ones in odd index
    // dbf.deleteMovies(1,{100,104,106,109});
    output = captureOutput(del, {"1", "100", "104", "106", "109"});
    EXPECT_EQ(output, s204);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,105,108,110}));
    //delete nothing
    // dbf.deleteMovies(1,{});
    output = captureOutput(del, {"1"});
    EXPECT_EQ(output, s400);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,105,108,110}));
    //delete something that doesn't exists
    // dbf.deleteMovies(1,{100,103,102});
    output = captureOutput(del, {"1", "100", "103", "102"});
    EXPECT_EQ(output, s404);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,105,108,110}));
    //delete everything
    // dbf.deleteMovies(1,{101,105,108,110});
    output = captureOutput(del, {"1", "101", "105", "108", "110"});
    EXPECT_EQ(output, s204);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{}));
    //add some ids
    dbf.updateUser(1, {101,102,103,104});
    //delete everything + things that doesn't exists
    // dbf.deleteMovies(1,{109,107,103,104,105,102,106,101});
    output = captureOutput(del, {"1", "109","107","103","104","105","102","106","101"});
    EXPECT_EQ(output, s404);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,102,103,104}));
    //deleting movies + movie that doesn't exists
    output = captureOutput(del, {"1", "101", "103", "200", "102"});
    EXPECT_EQ(output, s404);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,102,103,104}));
    //deleting non existing user
    // dbf.deleteMovies(2,{1,2,3});
    output = captureOutput(del, {"2", "1", "2", "3"});
    EXPECT_EQ(output, s404);
    EXPECT_EQ(dbf.findUser(2), (std::vector<unsigned long int>{}));
    //userid isnt a number
    output = captureOutput(del, {"a", "101", "103", "102"});
    EXPECT_EQ(output, s400);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,102,103,104}));
    //one of the movie ids ins't a number
    output = captureOutput(del, {"1", "101", "103d", "102"});
    EXPECT_EQ(output, s400);
    EXPECT_EQ(dbf.findUser(1), (std::vector<unsigned long int>{101,102,103,104}));

    // clear the files after the test
    test.cleanUp();
    delete del;
}
