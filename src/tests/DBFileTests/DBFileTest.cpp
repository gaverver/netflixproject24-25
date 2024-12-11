#include <gtest/gtest.h>
#include "../../DBFile.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

//helper function to return the first word of a string
std::string first_word(std::string s) {
    //creating a stream to read from the string
    std::istringstream stream(s);
    std::string firstWord;
    //reading the first word to a string
    stream >> firstWord;
    return firstWord;
}

//helper function to find the last N words of a strings
std::vector<std::string> LastNWords(std::string s, size_t N) {
    std::stringstream stream(s);
    std::string word;
    std::vector<std::string> words;
    
    // reading the words to string vector
    while (stream >> word) {
        words.push_back(word);
    }
    
    // if there are less than N word return all
    if (words.size() <= N) {
        return words;
    }
    
    // return the last n words
    return std::vector<std::string>(words.end() - N, words.end());
}

//helper function to get last N words of a movie line in movies.txt
std::vector<std::string> LastNWordsMovies(unsigned long int mid, int N) {
        std::ifstream moviesFile("../data/movies.txt");
        //searching for movie id i in the movies.txt file
        std::string line;
        while (std::getline(moviesFile, line)) {
            if (first_word(line) == std::to_string(mid)) {
                break;
            }
        }
        //check if movie i exists
        EXPECT_EQ(first_word(line), std::to_string(mid));
        moviesFile.close();
        //return the last N words of the specified line
        return LastNWords(line, N);

}

//helper function to get last N words of a user line in users.txt
std::vector<std::string> LastNWordsUsers(unsigned long int uid, int N) {
        std::ifstream moviesFile("../data/users.txt");
        //searching for user id i in the users.txt file
        std::string line;
        while (std::getline(moviesFile, line)) {
            // std::cout << std::endl << line << std::endl << first_word(line) << std::endl << std::endl;
            if (first_word(line) == std::to_string(uid)) {
                // std::cout << "im here" << line << std::endl << first_word(line) << std::endl;
                break;
            }
        }
        //std::cout << std::endl << std::endl << std::endl;
        //check if user i exists
        EXPECT_EQ(first_word(line), std::to_string(uid));
        moviesFile.close();
        //return the last N words of the specified line
        return LastNWords(line, N);

}

//testing updating user with list of watched movies (check users.txt)
TEST(DBFileTesting, UpdateUsr1) {
    //creaing instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //adding some movies to 3 different users
    for (unsigned long int i=1; i<4; ++i) {
        std::vector<unsigned long int> mids = {100+i,101+i,102+i};
        //updating some user with some ids
        dbf.updateUser(i,mids);
        //checking if the users file exists
        std::ifstream usersFile("../data/users.txt");
        EXPECT_TRUE(usersFile.is_open());
        //check if the movies added successfully
        std::vector<std::string> mids2 = LastNWordsUsers(i, 3);
        std::vector<std::string> midString = {std::to_string(100+i),std::to_string(101+i),std::to_string(102+i)};
        EXPECT_EQ(mids2,midString);

        usersFile.close();
    }
    //add some movies and check if changed
    std::vector<unsigned long int> mids1 = {101, 200};
    dbf.updateUser(1,mids1);
    //last two movies in user 1
    std::vector<std::string> mids3 = LastNWordsUsers(1,2);
    //expected last two movies in user 1
    std::vector<std::string> mids4 = {"103","200"};
    EXPECT_EQ(mids3,mids4);
    //clean
    dbf.cleanUp();
}

//testing updating user with list of watched movies (checking movies.txt)
TEST(DBFileTesting, UpdateUsr2) {
    //creaing instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //adding some movies to 3 different users
    for (int i=1; i<4; ++i) {
        std::vector<unsigned long int> mids = {101,102,103};
        //updating some user with some ids
        dbf.updateUser(i, mids);
    }
    //checking if the movies.txt exists
    std::ifstream moviesFile1("../data/movies.txt");
    EXPECT_TRUE(moviesFile1.is_open());
    moviesFile1.close();
    //for each movie check if the users added succesfully
    for (int i=101; i<104; i++) {
        //check if the users added successfully
        //last 3 users of movie i
        std::vector<std::string> uids = LastNWordsMovies(i, 3);
        //expected last 3 users
        std::vector<std::string> uidString = {"1","2","3"};
        EXPECT_EQ(uids,uidString);
    }
    //adding again movie to some users
    std::vector<unsigned long int> mid1 = {101};
    dbf.updateUser(1,mid1);
    dbf.updateUser(2,mid1);
    dbf.updateUser(30,mid1);
    //check if the movies.txt is updated
    std::vector<std::string> uids = LastNWordsMovies(101, 3);
    std::vector<std::string> uidString = {"2","3","30"};
    EXPECT_EQ(uids,uidString);

    //clean
    dbf.cleanUp();
}
//testing updating movie with list of users (checking movies.txt)
TEST(DBFileTesting, UpdateMovie1) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //adding some users to 3 different movies
    for (unsigned long int i=1; i<4; ++i) {
        std::vector<unsigned long int> uids = {100+i,101+i,102+i};
        //updating some movie with some users
        dbf.updateMovie(i, uids);
        //checking if the movies.txt exists
        std::ifstream MoviesFile("../data/movies.txt");
        EXPECT_TRUE(MoviesFile.is_open());
        MoviesFile.close();
        //check if the users added successfully
        std::vector<std::string> uids2 = LastNWordsMovies(i, 3);
        std::vector<std::string> uidString = {std::to_string(100+i),std::to_string(101+i),std::to_string(102+i)};
        EXPECT_EQ(uids2,uidString);

    }
    //add some users and check if changed
    std::vector<unsigned long int> uids1 = {101, 200};
    dbf.updateMovie(1,uids1);
    //last two users in movie 1
    std::vector<std::string> uids3 = LastNWordsMovies(1,2);
    //expected last two movies in user 1
    std::vector<std::string> uids4 = {"103","200"};
    EXPECT_EQ(uids3,uids4);
    //clean
    dbf.cleanUp();
}

//testing updating movie with list of users (checking user.txt)
TEST(DBFileTesting, UpdateMovie2) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //adding some user to 3 different movies
    for (int i=1; i<4; ++i) {
        std::vector<unsigned long int> uids = {101,102,103};
        //updating some movie with some users
        dbf.updateMovie(i, uids);
    }
    //checking if the users.txt exists
    std::ifstream usersFile1("../data/users.txt");
    EXPECT_TRUE(usersFile1.is_open());
    usersFile1.close();
    //for each user check if the movies added succesfully
    for (int i=101; i<104; i++) {
        //check if the movies added successfully
        //last 3 movies of user i
        std::vector<std::string> mids = LastNWordsUsers(i, 3);
        //expected last 3 users
        std::vector<std::string> midString = {"1","2","3"};
        EXPECT_EQ(mids,midString);
    }
    //adding again movie to some users
    std::vector<unsigned long int> uid1 = {101};
    dbf.updateMovie(1,uid1);
    dbf.updateMovie(2,uid1);
    dbf.updateMovie(30,uid1);
    //check if the movies.txt is updated
    std::vector<std::string> mids = LastNWordsUsers(101, 3);
    std::vector<std::string> midString = {"2","3","30"};
    EXPECT_EQ(mids,midString);

    //clean
    dbf.cleanUp();
}

//testing findUser method
TEST(DBFileTesting, FindUser) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //check for non-existing user
    std::vector<unsigned long int> emptyMids = {};
    std::vector<unsigned long int> midsCheck0 = dbf.findUser(1);
    EXPECT_EQ(emptyMids,midsCheck0);
    //check if the user exists and the list is empty
    dbf.updateUser(1,emptyMids);
    std::vector<unsigned long int> midsCheck01 = dbf.findUser(1);
    EXPECT_EQ(emptyMids,midsCheck01);

    //add some movies to user 1
    std::vector<unsigned long int> mids1 = {100,101,102};
    dbf.updateUser(1,mids1);
    
    //check if users.txt exists
    std::ifstream usersFile("../data/users.txt");
    EXPECT_TRUE(usersFile.is_open());
    usersFile.close();
    //check if the findUser works
    std::vector<unsigned long int> midsCheck1 = dbf.findUser(1);
    EXPECT_EQ(midsCheck1,mids1);
    //add some more movies
    std::vector<unsigned long int> mids2 = {100,201,202};
    dbf.updateUser(1,mids2);
    //check if the findUser works
    std::vector<unsigned long int> midsCheck2 = dbf.findUser(1);
    std::vector<unsigned long int> expectedMids = {100, 101, 102, 201, 202};
    EXPECT_EQ(midsCheck2,expectedMids);


    //clean
    dbf.cleanUp();
}

//testing findMovie method
TEST(DBFileTesting, FindMovie) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //check for non-existing movie
    std::vector<unsigned long int> emptyUids = {};
    std::vector<unsigned long int> uidsCheck0 = dbf.findMovie(1);
    EXPECT_EQ(emptyUids,uidsCheck0);
    //check if the movie exists and the list is empty
    dbf.updateMovie(1,emptyUids);
    std::vector<unsigned long int> uidsCheck01 = dbf.findMovie(1);
    EXPECT_EQ(emptyUids,uidsCheck01);

    //add some users to movie 1
    std::vector<unsigned long int> uids1 = {100,101,102};
    dbf.updateMovie(1,uids1);
    
    //check if movies.txt exists
    std::ifstream moviesFile("../data/movies.txt");
    EXPECT_TRUE(moviesFile.is_open());
    moviesFile.close();
    //check if the findMovie works
    std::vector<unsigned long int> uidsCheck1 = dbf.findMovie(1);
    EXPECT_EQ(uidsCheck1,uids1);
    //add some more users
    std::vector<unsigned long int> uids2 = {100,201,202};
    dbf.updateMovie(1,uids2);
    //check if the findMovie works
    std::vector<unsigned long int> uidsCheck2 = dbf.findMovie(1);
    std::vector<unsigned long int> expectedUids = {100, 101, 102, 201, 202};
    EXPECT_EQ(uidsCheck2,expectedUids);

    //clean
    dbf.cleanUp();
}

//testing getCommonMovies method (no intersection)
TEST(DBFileTesting, GetCommonMovies1) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //all the intersections should be empty
    std::vector<unsigned long int> empty = {};
    //list of movies from users 1,2
    std::vector<unsigned long int> mids1 = {};
    std::vector<unsigned long int> mids2 = {100,200,300};
    //add the movies
    dbf.updateUser(1,mids1);
    dbf.updateUser(2,mids2);
    //check if the intersection is empty
    EXPECT_EQ(empty,dbf.getCommonMovies(1,2));
    //list of movies from users 3,4
    std::vector<unsigned long int> mids3 = {50,150,250};
    std::vector<unsigned long int> mids4 = {400,500,600};
    //add the movies
    dbf.updateUser(3,mids3);
    dbf.updateUser(4,mids4);
    //check if the intersection is empty
    EXPECT_EQ(empty,dbf.getCommonMovies(3,4));

    //clean
    dbf.cleanUp();
}

//testing getCommonMovies method (half intersection)
TEST(DBFileTesting, GetCommonMovies2) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //first intersection result
    std::vector<unsigned long int> intersection1 = {100};
    //list of movies from users 1,2
    std::vector<unsigned long int> mids1 = {100,400,500};
    std::vector<unsigned long int> mids2 = {100,200,300};
    //add the movies
    dbf.updateUser(1,mids1);
    dbf.updateUser(2,mids2);
    //check intersection
    EXPECT_EQ(intersection1,dbf.getCommonMovies(1,2));
    //second intersection result
    std::vector<unsigned long int> intersection2 = {300};
    //list of movies from users 3,4
    std::vector<unsigned long int> mids3 = {100,200,300};
    std::vector<unsigned long int> mids4 = {300,400,500};
    //add the movies
    dbf.updateUser(3,mids3);
    dbf.updateUser(4,mids4);
    //check intersection
    EXPECT_EQ(intersection2,dbf.getCommonMovies(3,4));
    //third intersection result
    std::vector<unsigned long int> intersection3 = {200,300};
    //list of movies from users 5,6
    std::vector<unsigned long int> mids5 = {100,300,200,600};
    std::vector<unsigned long int> mids6 = {200,300,500,700};
    //add the movies
    dbf.updateUser(5,mids5);
    dbf.updateUser(6,mids6);
    //check intersection
    EXPECT_EQ(intersection3,dbf.getCommonMovies(5,6));
    //clean
    dbf.cleanUp();
}

//testing getCommonMovies method (full intersection)
TEST(DBFileTesting, GetCommonMovies3) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //first intersection result
    std::vector<unsigned long int> intersection1 = {100};
    //list of movies from users 1,2
    std::vector<unsigned long int> mids1 = {100};
    std::vector<unsigned long int> mids2 = {100};
    //add the movies
    dbf.updateUser(1,mids1);
    dbf.updateUser(2,mids2);
    //check intersection
    EXPECT_EQ(intersection1,dbf.getCommonMovies(1,2));

    //second intersection result
    std::vector<unsigned long int> intersection2 = {100,200,300,400,500};
    //list of movies from users 3,4
    std::vector<unsigned long int> mids3 = {100,500,300,400,200};
    std::vector<unsigned long int> mids4 = {200,100,400,500,300};
    //add the movies
    dbf.updateUser(3,mids3);
    dbf.updateUser(4,mids4);
    //check intersection
    EXPECT_EQ(intersection2,dbf.getCommonMovies(3,4));
    //clean
    dbf.cleanUp();
}

//testing getAllMovies method
TEST(DBFileTesting, GetAllMovies) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //expected first output
    std::vector<unsigned long int> movies1ex = {};
    //first output
    std::vector<unsigned long int> movies1out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies1ex,movies1out);
    
    //add some movies
    std::vector<unsigned long int> mids1 = {100, 200, 300, 400};
    dbf.updateUser(1, mids1);
    //expected second output
    std::vector<unsigned long int> movies2ex = {100, 200, 300, 400};
    //second output
    std::vector<unsigned long int> movies2out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies2ex,movies2out);
    
    //add some movies to the same user
    std::vector<unsigned long int> mids2 = {300, 400, 500, 600};
    dbf.updateUser(1, mids2);
    //expected third output
    std::vector<unsigned long int> movies3ex = {100, 200, 300, 400, 500, 600};
    //third output
    std::vector<unsigned long int> movies3out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies3ex,movies3out);

    //add some movies to different user
    std::vector<unsigned long int> mids3 = {800, 700, 100};
    dbf.updateUser(2, mids3);
    //expected fourth output
    std::vector<unsigned long int> movies4ex = {100, 200, 300, 400, 500, 600, 700, 800};
    //foourth output
    std::vector<unsigned long int> movies4out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies4ex,movies4out);


    //clean
    dbf.cleanUp();
}

//testing isUserExists method
TEST(DBFileTesting, isUserExists) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //1 user: 1, watched 1 movie: 1
    std::vector<unsigned long int> movies = {1};
    dbf.updateMovie(1,movies);

    //check if user 1 exists (supposed to get true)
    bool isExist = dbf.isUserExists(1);
    EXPECT_EQ(true,isExist);

    //check if user 2 exists (supposed to get false)
    isExist = dbf.isUserExists(2);
    EXPECT_EQ(false,isExist);

    //clean
    dbf.cleanUp();
}

//testing isMovieExists method
TEST(DBFileTesting, isMovieExists) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    //1 user: 1, watched 1 movie: 1
    std::vector<unsigned long int> movies = {1};
    dbf.updateMovie(1,movies);

    //check if movie 1 exists (supposed to get true)
    bool isExist = dbf.isMovieExists(1);
    EXPECT_EQ(true,isExist);

    //check if movie 2 exists (supposed to get false)
    isExist = dbf.isMovieExists(2);
    EXPECT_EQ(false,isExist);

    //clean
    dbf.cleanUp();
}
//testing deleteMovies function
TEST(DBFileTesting, DeleteMovies) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();

    //initialize
    dbf.updateUser(1, {101,102,103,104,105,106,107,108,109,110});
    //remove last two
    dbf.deleteMovies(1,{110,109});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{101,102,103,104,105,106,107,108});
    //remove first three
    dbf.deleteMovies(1,{101,103,102});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{104,105,106,107,108});
    //remove middle three
    dbf.deleteMovies(1,{106,107,105});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{104,108});
    //add some ids
    dbf.updateUser(1,{100,106,105,101,109,110});
    //remove the ones in odd index
    dbf.deleteMovies(1,{100,104,106,109});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{101,105,108,110});
    //delete nothing
    dbf.deleteMovies(1,{});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{101,105,108,110});
    //delete something that doesn't exists
    dbf.deleteMovies(1,{100,103,102});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{101,105,108,110});
    //delete everything
    dbf.deleteMovies(1,{101,105,108,110});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{});
    //add some ids
    dbf.updateUser(1, {101,102,103,104});
    //delete everything + things that doesn't exists
    dbf.deleteMovies(1,{109,107,103,104,105,102,106,101});
    EXPECT_EQ(dbf.findUser(1), std::vector<unsigned long int>{});
    //deleting non existing user
    dbf.deleteMovies(2,{1,2,3});
    EXPECT_EQ(dbf.findUser(2), std::vector<unsigned long int>{});
    //clean
    dbf.cleanUp();
}

//testing deleteUsers function
TEST(DBFileTesting, DeleteUsers) {
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../data");
    //clean the files;
    dbf.cleanUp();
    
    //initialize
    dbf.updateMovie(1, {101,102,103,104,105,106,107,108,109,110});
    //remove last two
    dbf.deleteUsers(1,{110,109});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{101,102,103,104,105,106,107,108});
    //remove first three
    dbf.deleteUsers(1,{101,103,102});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{104,105,106,107,108});
    //remove middle three
    dbf.deleteUsers(1,{106,107,105});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{104,108});
    //add some ids
    dbf.updateMovie(1,{100,106,105,101,109,110});
    //remove the ones in odd index
    dbf.deleteUsers(1,{100,104,106,109});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{101,105,108,110});
    //delete nothing
    dbf.deleteUsers(1,{});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{101,105,108,110});
    //delete something that doesn't exists
    dbf.deleteUsers(1,{100,103,102});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{101,105,108,110});
    //delete everything
    dbf.deleteUsers(1,{101,105,108,110});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{});
    //add some ids
    dbf.updateMovie(1, {101,102,103,104});
    //delete everything + things that doesn't exists
    dbf.deleteUsers(1,{109,107,103,104,105,102,106,101});
    EXPECT_EQ(dbf.findMovie(1), std::vector<unsigned long int>{});
    //deleting non existing movie
    dbf.deleteUsers(2,{1,2,3});
    EXPECT_EQ(dbf.findMovie(2), std::vector<unsigned long int>{});
    //clean
    dbf.cleanUp();
}
// // Main function for running tests
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
