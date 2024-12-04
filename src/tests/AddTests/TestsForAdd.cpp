#include <gtest/gtest.h>
#include "../../ICommand.h"
#include "../../IDataBase.h"
#include "../../DBFile.h"
#include "../../addCommand.h"

#include <fstream>
#include <vector>
#include <string>



// Test that checks that the return value of the description function is as it should be 
TEST(AddTesting, DescriptionTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* h = new addCommand(test);
    // checks if the return value of the function is equal to "add [userid] [movieid1] [movieid2] ..." - as it should because it describes the command
    EXPECT_EQ(h->description(), "add [userid] [movieid1] [movieid2] ...");
}

// test for execute function - simple test that checks adding of a user and a movie
TEST(AddTesting, SimpleExecuteTest) {
    
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    // clear the files before testing
    test.cleanUp();
    ICommand* add = new addCommand(test);
    std::vector<std::string> args;
    // execute the command add 101 102
    args.push_back("101");
    args.push_back("102");
    // execute the add command that should update the databse and add the user/movie
    add -> execute(args);
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

// test for execute function -  test that checks adding of a user and multiple movie
TEST(AddTesting, MultipleMoviesOneUserTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* add = new addCommand(test);
    std::vector<std::string> args;
    // execute the command add 2 3 4
    args.push_back("2");
    args.push_back("3");
    args.push_back("4");
    // execute the add command that should update the databse and add the user/movie
    add -> execute(args);
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

// test for execute function -  test that checks adding of multiple users and multiple movie
TEST(AddTesting, MultipleMoviesMultipleUsersTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;
    ICommand* add = new addCommand(test);

    std::vector<std::string> args1;
    // execute the command add 2 3 4
    args1.push_back("2");
    args1.push_back("3");
    args1.push_back("4");
    // execute the add command that should update the databse and add the user/movie
    add -> execute(args1);

    std::vector<std::string> args2;
    // execute the command add 3 4 5
    args2.push_back("3");
    args2.push_back("4");
    args2.push_back("5");
    // execute the add command that should update the databse and add the user/movie
    add -> execute(args2);

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
TEST(AddTesting, InvalidInputTest) {
    DBFile dbFile("../data");
    IDataBase& test = dbFile;    
    ICommand* add = new addCommand(test);
    std::vector<std::string> args;
    // execute the command add 5 6
    args.push_back("5");
    args.push_back("6");
    // execute the add command that should update the databse and add the user/movie
    add -> execute(args);

    // execute the command add c 6
    args[0] = "c";
    args[1] = "6";
    // execute the add command with invalid input
    add -> execute(args);

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

    // execute the command add c 6
    args[0] = "5";
    args[1] = "-1";
    // execute the add command with invalid input
    add -> execute(args);

    // get the movies that were saved under the 5 userID in the databse(id 5)
    std::vector<unsigned long int> movies5 = test.findUser(5);
    // check if the correct movies were saved under the user(id 5) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedMovies, movies5);

    // find all the users that watched that movie(id 6)
    std::vector<unsigned long int> users6 = test.findMovie(6);
    // check if the correct users were saved under the movie(id 6) - we check this to see if the invalid input didn't effect this
    EXPECT_EQ(expectedUsers, users6);

    // execute the command add 28446744073709551615 6 - over the range on unsigned long int
    args[0] = "28446744073709551615";
    args[1] = "6";
    // execute the add command with invalid input
    add -> execute(args);

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

