#include <gtest/gtest.h>
#include "../../ICommand.h"
#include "../../IDataBase.h"
#include "../../DBFile.h"
#include "../../PATCHCommand.h"

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
TEST(PATCHTesting, DescriptionTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* patch = new PATCHCommand(test);
    // checks if the return value of the function is equal to "PATCH [userid] [movieid1] [movieid2] ..." - as it should because it describes the command
    EXPECT_EQ(patch->description(), "PATCH, arguments: [userid] [movieid1] [movieid2] ...");
    delete patch;
}

// Test that checks that the return value of the description function is as it should be 
TEST(PATCHTesting, rwSTtatusTests) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* patch = new PATCHCommand(test);
    // checks if the return value of the function is equal to "writer'" - as the commands writes to the DB.
    EXPECT_EQ(patch->rw_status(), "writer");
    delete patch;
}

// test for execute function - simple test that checks PATCHing of a user and a movie
TEST(PATCHTesting, SimpleExecuteTest) {
    
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    // clear the files before testing
    test.cleanUp();
    ICommand* patch = new PATCHCommand(test);
    std::vector<std::string> args;
    // first add manually the user 101 to the databse so patch will work.
    test.update_user(101, {100});
    // execute the command PATCH 101 102
    args.push_back("101");
    args.push_back("102");
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(patch, args);
    // check if the correct string was printed
    EXPECT_EQ(output, "204 No Content\n");
    // get the movies that were saved under the 101 userID in the databse
    std::vector<unsigned long int> movies = test.findUser(101);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies;
    expectedMovies.push_back(100);
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
    delete patch;
}

// test for execute function -  test that checks PATCHing of a user and multiple movie
TEST(PATCHTesting, MultipleMoviesOneUserTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* patch = new PATCHCommand(test);
    std::vector<std::string> args;
    // first add manually the user 2 to the databse so patch will work.
    test.update_user(2, {1});
    // execute the command PATCH 2 3 4
    args.push_back("2");
    args.push_back("3");
    args.push_back("4");
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(patch, args);
    // check if the correct string was printed
    EXPECT_EQ(output, "204 No Content\n");
    // get the movies that were saved under the 2 userID in the databse
    std::vector<unsigned long int> movies = test.findUser(2);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies;
    expectedMovies.push_back(1);
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
    delete patch;
}

// test for execute function -  test that checks PATCHing of multiple users and multiple movie
TEST(PATCHTesting, MultipleMoviesMultipleUsersTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* patch = new PATCHCommand(test);

    std::vector<std::string> args1;

    // first add manually the users to the databse so patch will work.
    test.update_user(2, {1});
    test.update_user(3, {1});

    // execute the command PATCH 2 3 4
    args1.push_back("2");
    args1.push_back("3");
    args1.push_back("4");
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(patch, args1);
    // check if the correct string was printed
    EXPECT_EQ(output, "204 No Content\n");
    std::vector<std::string> args2;
    // execute the command PATCH 3 4 5
    args2.push_back("3");
    args2.push_back("4");
    args2.push_back("5");
    // execute the PATCH command that should update the databse and print an appropriate message
    output = captureOutput(patch, args2);
    // check if the correct string was printed
    EXPECT_EQ(output, "204 No Content\n");

    // get the movies that were saved under the userID 2 in the databse
    std::vector<unsigned long int> movies2 = test.findUser(2);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies2;
    expectedMovies2.push_back(1);
    expectedMovies2.push_back(3);
    expectedMovies2.push_back(4);
    // check if the correct movies were saved under the user
    EXPECT_EQ(expectedMovies2, movies2);

    // get the movies that were saved under the userID 3 in the databse
    std::vector<unsigned long int> movies3 = test.findUser(3);
    // movieID's that were supposed to be saved
    std::vector<unsigned long int> expectedMovies3;
    expectedMovies3.push_back(1);
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
    delete patch;
}

// test for execute function -  test that checks invalid input options to the DB
TEST(PATCHTesting, InvalidInputTest400) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;    
    ICommand* patch = new PATCHCommand(test);
    std::vector<std::string> args;
    // first add manually the users to the databse so patch will work.
    test.update_user(5, {6});

    // execute the command PATCH c 6
    args.push_back("c");
    args.push_back("6");
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(patch, args);
    // check if the correct string was printed - as we entered an invalid userID
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

    // execute the command PATCH c 6
    args[0] = "5";
    args[1] = "-1";
    // execute the PATCH command that should update the databse and print an appropriate message
    output = captureOutput(patch, args);
    // check if the correct string was printed - as we entered an invalid movieID
    EXPECT_EQ(output, "400 Bad Request\n");

    // get the movies that were saved under the 5 userID in the databse(id 5)
    std::vector<unsigned long int> movies5 = test.findUser(5);
    // check if the correct movies were saved under the user(id 5) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedMovies, movies5);

    // find all the users that watched that movie(id 6)
    std::vector<unsigned long int> users6 = test.findMovie(6);
    // check if the correct users were saved under the movie(id 6) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedUsers, users6);

    // execute the command PATCH 28446744073709551615 6 - over the range on unsigned long int
    args[0] = "28446744073709551615";
    args[1] = "6";
    // execute the PATCH command that should update the databse and print an appropriate message
    output = captureOutput(patch, args);
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

    // execute the command PATCH 1 - less then 2 arguments
    args[0] = "1";
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(patch, args);
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
    delete patch;
}

TEST(PATCHTesting, InvalidInputTest404) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;    
    ICommand* patch = new PATCHCommand(test);
    std::vector<std::string> args;
    // execute the command PATCH 5 6
    args.push_back("5");
    args.push_back("6");
    // execute the PATCH command that should update the databse and print an appropriate message
    std::string output = captureOutput(patch, args);
    // check if the correct string was printed - the user doesn't exist so it is logically incorrect usage.
    EXPECT_EQ(output, "404 Not Found\n");
    // check that the user wasn't added to the db.
    EXPECT_EQ(db.isUserExists(5), false);

    // clear the files after the test
    test.cleanUp();
    delete patch;
}

