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
std::vector<std::string> LastNWordsMovies(int mid, int N) {
        std::ifstream moviesFile("../../../data/movies.txt");
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
std::vector<std::string> LastNWordsUsers(int uid, int N) {
        std::ifstream moviesFile("../../../data/users.txt");
        //searching for user id i in the users.txt file
        std::string line;
        while (std::getline(moviesFile, line)) {
            if (first_word(line) == std::to_string(uid)) {
                break;
            }
        }
        //check if user i exists
        EXPECT_EQ(first_word(line), std::to_string(uid));
        moviesFile.close();
        //return the last N words of the specified line
        return LastNWords(line, N);

}

//setup before every test, to ensure that the files are empty
void setup() {
    //clean the users.txt
    std::ofstream users("../../../data/users.txt", std::ios::trunc);
    users.close();
    std::ofstream movies("../../../data/movies.txt", std::ios::trunc);
    movies.close();
}







//testing updating user with list of watched movies (check users.txt)
TEST(DBFileTesting, UpdateUsr1) {
    //clean the files;
    setup();
    //creaing instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //adding some movies to 3 different users
    for (int i=1; i<4; ++i) {
        std::vector<int> mids = {100+i,101+i,102+i};
        //updating some user with some ids
        dbf.updateUser(i,mids);
        //checking if the users file exists
        std::ifstream usersFile("../../../data/users.txt");
        EXPECT_TRUE(usersFile.is_open());
        //check if the movies added successfully
        std::vector<std::string> mids2 = LastNWordsUsers(i, 3);
        std::vector<std::string> midString = {std::to_string(100+i),std::to_string(101+i),std::to_string(102+i)};
        EXPECT_EQ(mids2,midString);

        usersFile.close();
    }
    //add some movies and check if changed
    std::vector<int> mids1 = {101, 200};
    dbf.updateUser(1,mids1);
    //last two movies in user 1
    std::vector<std::string> mids3 = LastNWordsUsers(1,2);
    //expected last two movies in user 1
    std::vector<std::string> mids4 = {"103","200"};
    EXPECT_EQ(mids3,mids4);
    //clean
    setup();
}

//testing updating user with list of watched movies (checking movies.txt)
TEST(DBFileTesting, UpdateUsr2) {
    //clean the files;
    setup();
    //creaing instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //adding some movies to 3 different users
    for (int i=1; i<4; ++i) {
        std::vector<int> mids = {101,102,103};
        //updating some user with some ids
        dbf.updateUser(i, mids);
    }
    //checking if the movies.txt exists
    std::ifstream moviesFile1("../../../data/movies.txt");
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
    std::vector<int> mid1 = {101};
    dbf.updateUser(1,mid1);
    dbf.updateUser(2,mid1);
    dbf.updateUser(30,mid1);
    //check if the movies.txt is updated
    std::vector<std::string> uids = LastNWordsMovies(101, 3);
    std::vector<std::string> uidString = {"2","3","30"};
    EXPECT_EQ(uids,uidString);

    //clean
    setup();
}
//testing updating movie with list of users (checking movies.txt)
TEST(DBFileTesting, UpdateMovie1) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //adding some users to 3 different movies
    for (int i=1; i<4; ++i) {
        std::vector<int> uids = {100+i,101+i,102+i};
        //updating some movie with some users
        dbf.updateMovie(i, uids);
        //checking if the movies.txt exists
        std::ifstream MoviesFile("../../../data/movies.txt");
        EXPECT_TRUE(MoviesFile.is_open());
        MoviesFile.close();
        //check if the users added successfully
        std::vector<std::string> uids2 = LastNWordsMovies(i, 3);
        std::vector<std::string> uidString = {std::to_string(100+i),std::to_string(101+i),std::to_string(102+i)};
        EXPECT_EQ(uids2,uidString);

    }
    //add some users and check if changed
    std::vector<int> uids1 = {101, 200};
    dbf.updateMovie(1,uids1);
    //last two users in movie 1
    std::vector<std::string> uids3 = LastNWordsMovies(1,2);
    //expected last two movies in user 1
    std::vector<std::string> uids4 = {"103","200"};
    EXPECT_EQ(uids3,uids4);
    //clean
    setup();
}

//testing updating movie with list of users (checking user.txt)
TEST(DBFileTesting, UpdateMovie2) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //adding some user to 3 different movies
    for (int i=1; i<4; ++i) {
        std::vector<int> uids = {101,102,103};
        //updating some movie with some users
        dbf.updateMovie(i, uids);
    }
    //checking if the users.txt exists
    std::ifstream usersFile1("../../../data/users.txt");
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
    std::vector<int> uid1 = {101};
    dbf.updateMovie(1,uid1);
    dbf.updateMovie(2,uid1);
    dbf.updateMovie(30,uid1);
    //check if the movies.txt is updated
    std::vector<std::string> mids = LastNWordsUsers(101, 3);
    std::vector<std::string> midString = {"2","3","30"};
    EXPECT_EQ(mids,midString);

    //clean
    setup();
}

//testing findUser method
TEST(DBFileTesting, FindUser) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //check for non-existing user
    std::vector<int> emptyMids = {};
    std::vector<std::string> midsCheck0 = dbf.findUser(1);
    EXPECT_EQ(emptyMids,midsCheck0);
    //check if the user exists and the list is empty
    dbf.updateUser(1,emptyMids);
    std::vector<std::string> midsCheck01 = dbf.findUser(1);
    EXPECT_EQ(emptyMids,midsCheck01);

    //add some movies to user 1
    std::vector<int> mids1 = {100,101,102};
    dbf.updateUser(1,mids1);
    
    //check if users.txt exists
    std::ifstream usersFile("../../../data/users.txt");
    EXPECT_TRUE(usersFile.is_open());
    usersFile.close();
    //check if the findUser works
    std::vector<std::string> midsCheck1 = dbf.findUser(1);
    EXPECT_EQ(midsCheck1,mids1);
    //add some more movies
    std::vector<int> mids2 = {100,201,202};
    dbf.updateUser(1,mids2);
    //check if the findUser works
    std::vector<std::string> midsCheck2 = dbf.findUser(1);
    std::vector<std::string> expectedMids = {100, 101, 102, 201, 202};
    EXPECT_EQ(midsCheck2,expectedMids);


    //clean
    setup();
}
//testing findMovie method
TEST(DBFileTesting, FindMovie) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    
    //check for non-existing movie
    std::vector<int> emptyUids = {};
    std::vector<std::string> uidsCheck0 = dbf.findMovie(1);
    EXPECT_EQ(emptyUids,uidsCheck0);
    //check if the movie exists and the list is empty
    dbf.updateUser(1,emptyUids);
    std::vector<std::string> uidsCheck01 = dbf.findMovie(1);
    EXPECT_EQ(emptyUids,uidsCheck01);

    //add some users to movie 1
    std::vector<int> uids1 = {100,101,102};
    dbf.updateUser(1,uids1);
    
    //check if movies.txt exists
    std::ifstream moviesFile("../../../data/movies.txt");
    EXPECT_TRUE(moviesFile.is_open());
    moviesFile.close();
    //check if the findMovie works
    std::vector<std::string> uidsCheck1 = dbf.findMovie(1);
    EXPECT_EQ(uidsCheck1,uids1);
    //add some more users
    std::vector<int> uids2 = {100,201,202};
    dbf.updateUser(1,uids2);
    //check if the findMovie works
    std::vector<std::string> uidsCheck2 = dbf.findUser(1);
    std::vector<std::string> expectedUids = {100, 101, 102, 201, 202};
    EXPECT_EQ(uidsCheck2,expectedUids);

    //clean
    setup();
}

//testing getCommonMovies method (no intersection)
TEST(DBFileTesting, GetCommonMovies1) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //all the intersections should be empty
    std::vector<std::int> empty = {};
    //list of movies from users 1,2
    std::vector<std::int> mids1 = {};
    std::vector<std::int> mids2 = {100,200,300};
    //add the movies
    dbf.updateUser(1,mids1);
    dbf.updateUser(2,mids2);
    //check if the intersection is empty
    EXPECT_EQ(empty,dbf.getCommonMovies(1,2));
    //list of movies from users 3,4
    std::vector<std::int> mids3 = {50,150,250};
    std::vector<std::int> mids4 = {400,500,600};
    //add the movies
    dbf.updateUser(3,mids3);
    dbf.updateUser(4,mids4);
    //check if the intersection is empty
    EXPECT_EQ(empty,dbf.getCommonMovies(3,4));

    //clean
    setup();
}

//testing getCommonMovies method (half intersection)
TEST(DBFileTesting, GetCommonMovies2) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //first intersection result
    std::vector<std::int> intersection1 = {100};
    //list of movies from users 1,2
    std::vector<std::int> mids1 = {100,400,500};
    std::vector<std::int> mids2 = {100,200,300};
    //add the movies
    dbf.updateUser(1,mids1);
    dbf.updateUser(2,mids2);
    //check intersection
    EXPECT_EQ(intersection1,dbf.getCommonMovies(1,2));
    //second intersection result
    std::vector<std::int> intersection2 = {300};
    //list of movies from users 3,4
    std::vector<std::int> mids3 = {100,200,300};
    std::vector<std::int> mids4 = {300,400,500};
    //add the movies
    dbf.updateUser(3,mids3);
    dbf.updateUser(4,mids4);
    //check intersection
    EXPECT_EQ(intersection2,dbf.getCommonMovies(3,4));
    //third intersection result
    std::vector<std::int> intersection3 = {200,300};
    //list of movies from users 5,6
    std::vector<std::int> mids5 = {100,300,200,600};
    std::vector<std::int> mids6 = {200,300,500,700};
    //add the movies
    dbf.updateUser(5,mids5);
    dbf.updateUser(6,mids6);
    //check intersection
    EXPECT_EQ(intersection3,dbf.getCommonMovies(5,6));
    //clean
    setup();
}

//testing getCommonMovies method (full intersection)
TEST(DBFileTesting, GetCommonMovies3) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    //first intersection result
    std::vector<std::int> intersection1 = {100};
    //list of movies from users 1,2
    std::vector<std::int> mids1 = {100};
    std::vector<std::int> mids2 = {100};
    //add the movies
    dbf.updateUser(1,mids1);
    dbf.updateUser(2,mids2);
    //check intersection
    EXPECT_EQ(intersection1,dbf.getCommonMovies(1,2));

    //second intersection result
    std::vector<std::int> intersection2 = {100,200,300,400,500};
    //list of movies from users 3,4
    std::vector<std::int> mids3 = {100,500,300,400,200};
    std::vector<std::int> mids4 = {200,100,400,500,300};
    //add the movies
    dbf.updateUser(3,mids3);
    dbf.updateUser(4,mids4);
    //check intersection
    EXPECT_EQ(intersection2,dbf.getCommonMovies(3,4));
    //clean
    setup();
}

//testing getAllMovies method
TEST(DBFileTesting, GetAllMovies) {
    //clean the files;
    setup();
    //creating instance of the DBFile initializing it with the data path
    DBFile dbf("../../../data");
    
    //expected first output
    std::vector<int> movies1ex = {};
    //first output
    std::vector<int> movies1out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies1ex,movies1out);
    
    //add some movies
    std::vector<int> mids1 = {100, 200, 300, 400};
    dbf.addUser(1, mids1);
    //expected second output
    std::vector<int> movies2ex = {100, 200, 300, 400};
    //second output
    std::vector<int> movies2out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies2ex,movies2out);
    
    //add some movies to the same user
    std::vector<int> mids2 = {300, 400, 500, 600};
    dbf.addUser(1, mids2);
    //expected third output
    std::vector<int> movies3ex = {100, 200, 300, 400, 500, 600};
    //third output
    std::vector<int> movies3out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies3ex,movies3out);

    //add some movies to the same user
    std::vector<int> mids3 = {700, 800, 100};
    dbf.addUser(2, mids3);
    //expected fourth output
    std::vector<int> movies4ex = {100, 200, 300, 400, 500, 600, 700, 800};
    //foourth output
    std::vector<int> movies4out = dbf.getAllMovies();
    //check movie list equality
    EXPECT_EQ(movies4ex,movies4out);


    //clean
    setup();
}

// Main function for running tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}