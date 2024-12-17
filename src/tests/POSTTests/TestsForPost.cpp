#include <gtest/gtest.h>
#include "../../ICommand.h"
#include "../../IDataBase.h"
#include "../../DBFile.h"
#include "../../POSTCommand.h"
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
TEST(POSTTesting, DescriptionTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);
    // checks if the return value of the function is equal to "POST [userid] [movieid1] [movieid2] ..." - as it should because it describes the command
    EXPECT_EQ(post->description(), "POST, arguments: [userid] [movieid1] [movieid2] ...");
}

// Test that checks that the return value of the description function is as it should be 
TEST(POSTTesting, rwStatusTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);
    // checks if the return value of the function is equal to "writer" - as the commands writes to the DB.
    EXPECT_EQ(post->rw_status(), "writer");
}

// test for execute function - simple test that checks POSTing of a user and a movie
TEST(POSTTesting, SimpleExecuteTest) {
    
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    // clear the files before testing
    test.cleanUp();
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);
    std::vector<std::string> args;
    // execute the command POST 101 102
    args.push_back("101");
    args.push_back("102");
     // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    std::string output = captureOutput(post, args);
    // it created a new user and should print "201 Created" and a new line character
    EXPECT_EQ(output, "201 Created\n");
    // get the movies that were saved under the 101 userID in the databse
    std::vector<unsigned long int> movies = test.findUser(101);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies;
    expectedMovies.push_back(102);
    // check if the correct movies were saved under the user
    EXPECT_EQ(expectedMovies, movies);
    // find all the users that watched that movie
    std::vector<unsigned long int> users = test.findMovie(102);
    // UserID's that were supposed to be saved
    std::vector<unsigned long int> expectedUsers;
    expectedUsers.push_back(101);
    EXPECT_EQ(expectedUsers, users);
    // clear the files after the test
    test.cleanUp();
}

// test for execute function -  test that checks POSTing of a user and multiple movie
TEST(POSTTesting, MultipleMoviesOneUserTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);
    std::vector<std::string> args;
    // execute the command POST 2 3 4
    args.push_back("2");
    args.push_back("3");
    args.push_back("4");
     // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    std::string output = captureOutput(post, args);
    // it created a new user and should print "201 Created" and a new line character
    EXPECT_EQ(output, "201 Created\n");
    // get the movies that were saved under the 2 userID in the databse
    std::vector<unsigned long int> movies = test.findUser(2);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies;
    expectedMovies.push_back(3);
    expectedMovies.push_back(4);
    // check if the correct movies were saved under the user
    EXPECT_EQ(expectedMovies, movies);
    // find all the users that watched movie number 3
    std::vector<unsigned long int> users = test.findMovie(3);
    // UserID's that were supposed to be saved(users that watched movie number 3)
    std::vector<unsigned long int> expectedUsers3;
    expectedUsers3.push_back(2);
    EXPECT_EQ(expectedUsers3, users);
    // find all the users that watched movie number 4
    users = test.findMovie(4);
    // UserID's that were supposed to be saved(users that watched movie number 4)
    std::vector<unsigned long int> expectedUsers4;
    expectedUsers4.push_back(2);
    EXPECT_EQ(expectedUsers4, users);
    // clear the files after the test
    test.cleanUp();
}

// test for execute function -  test that checks POSTing of multiple users and multiple movie
TEST(POSTTesting, MultipleMoviesMultipleUsersTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);

    std::vector<std::string> args1;
    // execute the command POST 2 3 4
    args1.push_back("2");
    args1.push_back("3");
    args1.push_back("4");
     // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    std::string output = captureOutput(post, args1);
    // it created a new user and should print "201 Created" and a new line character
    EXPECT_EQ(output, "201 Created\n");

    std::vector<std::string> args2;
    // execute the command POST 3 4 5
    args2.push_back("3");
    args2.push_back("4");
    args2.push_back("5");
     // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    output = captureOutput(post, args2);
    // it created a new user and should print "201 Created" and a new line character
    EXPECT_EQ(output, "201 Created\n");

    // get the movies that were saved under the userID 2 in the databse
    std::vector<unsigned long int> movies2 = test.findUser(2);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies2;
    expectedMovies2.push_back(3);
    expectedMovies2.push_back(4);
    // check if the correct movies were saved under the user
    EXPECT_EQ(expectedMovies2, movies2);

    // get the movies that were saved under the userID 3 in the databse
    std::vector<unsigned long int> movies3 = test.findUser(3);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies3;
    expectedMovies3.push_back(4);
    expectedMovies3.push_back(5);
    // check if the correct movies were saved under the user
    EXPECT_EQ(expectedMovies3, movies3);

    // find all the users that watched movie number 3
    std::vector<unsigned long int> users = test.findMovie(3);
    // UserID's that were supposed to be saved(users that watched movie number 3)
    std::vector<unsigned long int> expectedUsers3;
    expectedUsers3.push_back(2);
    // check if the list of users that watched the movie is good
    EXPECT_EQ(expectedUsers3, users);


    // find all the users that watched movie number 4
    users = test.findMovie(4);
    // UserID's that were supposed to be saved(users that watched movie number 4)
    std::vector<unsigned long int> expectedUsers4;
    expectedUsers4.push_back(2);
    expectedUsers4.push_back(3);
    // check if the list of users that watched the movie is good
    EXPECT_EQ(expectedUsers4, users);

    // find all the users that watched movie number 4
    users = test.findMovie(5);
    // UserID's that were supposed to be saved(users that watched movie number 4)
    std::vector<unsigned long int> expectedUsers5;
    expectedUsers5.push_back(3);
    // check if the list of users that watched the movie is good
    EXPECT_EQ(expectedUsers5, users);

    // clear the files after the test
    test.cleanUp();
}

// test for execute function -  test that checks invalid input options to the DB
TEST(POSTTesting, InvalidInputTest400) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;    
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);
    std::vector<std::string> args;
    // execute the command POST 5 6
    args.push_back("5");
    args.push_back("6");
    // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    std::string output = captureOutput(post, args);
    // it created a new user and should print "201 Created" and a new line character
    EXPECT_EQ(output, "201 Created\n");

    // execute the command POST c 6
    args[0] = "c";
    args[1] = "6";
     // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    output = captureOutput(post, args);
    // it created a new user and should print "400 Bad Request" and a new line character as the userID isn't legal.
    EXPECT_EQ(output, "400 Bad Request\n");

    // get the movies that were saved under the 5 userID in the databse(id 5)
    std::vector<unsigned long int> movies = test.findUser(5);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies;
    expectedMovies.push_back(6);
    // check if the correct movies were saved under the user(id 5)
    EXPECT_EQ(expectedMovies, movies);

    // find all the users that watched that movie(id 6)
    std::vector<unsigned long int> users = test.findMovie(6);
    // UserID's that were supposed to be saved
    std::vector<unsigned long int> expectedUsers;
    expectedUsers.push_back(5);
    // check if the correct users were saved under the movie(id 6)
    EXPECT_EQ(expectedUsers, users);

    // execute the command POST c 6
    args[0] = "5";
    args[1] = "-1";
    // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    output = captureOutput(post, args);
    // it created a new user and should print "400 Bad Request" and a new line character as the movieID isn't legal.
    EXPECT_EQ(output, "400 Bad Request\n");

    // get the movies that were saved under the 5 userID in the databse(id 5)
    std::vector<unsigned long int> movies5 = test.findUser(5);
    // check if the correct movies were saved under the user(id 5) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedMovies, movies5);

    // find all the users that watched that movie(id 6)
    std::vector<unsigned long int> users6 = test.findMovie(6);
    // check if the correct users were saved under the movie(id 6) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedUsers, users6);

    // execute the command POST 28446744073709551615 6 - over the range on unsigned long int
    args[0] = "28446744073709551615";
    args[1] = "6";
    // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    output = captureOutput(post, args);
    // it created a new user and should print "400 Bad Request" and a new line character as the userID isn't legal.
    EXPECT_EQ(output, "400 Bad Request\n");

    // get the movies that were saved under the 5 userID in the databse(id 5)
    movies = test.findUser(5);
    // check if the correct movies were saved under the user(id 5) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedMovies, movies);

    // find all the users that watched that movie(id 6)
    users = test.findMovie(6);
    // check if the correct users were saved under the movie(id 6) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedUsers, users);
    args.resize(1);
    // execute the command PATCH 1 - less then 2 arguments
    args[0] = "1";
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(post, args);
    // check if the correct string was printed - as we entered an invalid userID
    EXPECT_EQ(output, "400 Bad Request\n");

    // get the movies that were saved under the 5 userID in the databse(id 5)
    movies = test.findUser(5);
    // check if the correct movies were saved under the user(id 5) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedMovies, movies);

    // find all the users that watched that movie(id 6)
    users = test.findMovie(6);
    // check if the correct users were saved under the movie(id 6) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedUsers, users);
    // clear the files after the test
    test.cleanUp();
}

TEST(POSTTesting, InvalidInputTest404) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;    
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    ICommand* post = new POSTCommand(test, menu);
    std::vector<std::string> args;
    // execute the command POST 5 6
    args.push_back("5");
    args.push_back("6");
    // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    std::string output = captureOutput(post, args);
    // it created a new user and should print "201 Created" and a new line character
    EXPECT_EQ(output, "201 Created\n");

    // execute the command POST c 6
    args[0] = "5";
    args[1] = "7";
    // execute the PATCH command that should update the databse and print an appropriate message - we capture the output
    output = captureOutput(post, args);
    // it tries to add a movie to a user that already exists - so it is "404 Not Found" as post only adds new users. 
    EXPECT_EQ(output, "404 Not Found\n");

    // get the movies that were saved under the 5 userID in the databse(id 5)
    std::vector<unsigned long int> movies = test.findUser(5);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies;
    expectedMovies.push_back(6);
    // check if the correct movies were saved under the user(id 5)
    EXPECT_EQ(expectedMovies, movies);

    // find all the users that watched that movie(id 6)
    std::vector<unsigned long int> users = test.findMovie(6);
    // UserID's that were supposed to be saved
    std::vector<unsigned long int> expectedUsers;
    expectedUsers.push_back(5);
    // check if the correct users were saved under the movie(id 6)
    EXPECT_EQ(expectedUsers, users);

    // clear the files after the test
    test.cleanUp();
}

