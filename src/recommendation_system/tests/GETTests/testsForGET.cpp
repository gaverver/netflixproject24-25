#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <limits>
#include <functional>
#include "../../GETCommand.h"
#include "../../DBFile.h"
#include "../../IMenu.h"
#include "../../IDataBase.h"
#include "../../ConsoleMenu.h"

// Helper function to capture the string that execute prints via IMenu
std::string captureOutput(const std::function<void()>& func) {
    std::ostringstream captured;
    // Save original buffer
    std::streambuf* original = std::cout.rdbuf();
    // Redirect std::cout to the string stream
    std::cout.rdbuf(captured.rdbuf());
    // Execute the function
    func();
    // Restore original buffer
    std::cout.rdbuf(original);
    // Return captured output as a string
    return captured.str();
}

// Helper function to setup the data before testing test2
void setup_pos1(IDataBase &db) {
    // clean up the data in db if there is
    db.cleanUp();

    // update all data
    db.updateUser("1", {"1"});
    db.updateUser("2", {"1"});
}

// Helper function to setup the data before testing test3
void setup_pos2(IDataBase &db) {
    // clean up the data in db if there is
    db.cleanUp();

    // update all data
    db.updateUser("1", {"1"});
    db.updateUser("2", {"1", "3"});
    db.updateUser("3", {"2"});
}

// Helper function to setup the data before testing test1, test4, test5
void setup_pos3(IDataBase &db) {
    // clean up the data in db if there is
    db.cleanUp();

    // update all data
    db.updateUser("1", {"100", "101", "102", "103"});
    db.updateUser("2", {"101", "102", "104", "105", "106"});
    db.updateUser("3", {"100", "104", "105", "107", "108"});
    db.updateUser("4", {"101", "105", "106", "107", "109", "110"});
    db.updateUser("5", {"100", "102", "103", "105", "108", "111"});
    db.updateUser("6", {"100", "103", "104", "110", "111", "112", "113"});
    db.updateUser("7", {"102", "105", "106", "107", "108", "109", "110"});
    db.updateUser("8", {"101", "104", "105", "106", "109", "111", "114"});
    db.updateUser("9", {"100", "103", "105", "107", "112", "113", "115"});
    db.updateUser("10", {"100", "102", "105", "106", "107", "109", "110", "116"});
}

// Test case 1: Check invalid input in execute function
TEST(GETTest, ExecuteInvalidInput) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_pos3(db);
    ConsoleMenu cm;
    IMenu& menu = cm;
    GETCommand GET(db, menu);

    // check what if user id is empty (should print error)
    auto output = captureOutput([&]() { GET.execute({"", "104"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if user id is invalid (should print error)
    output = captureOutput([&]() { GET.execute({"t", "123"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if movie id is invalid (should print error)
    output = captureOutput([&]() { GET.execute({"1", "12x"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if user id is negative (invalid) (should print error)
    output = captureOutput([&]() { GET.execute({"-1", "104"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if movie id is negative (invalid) (should print error)
    output = captureOutput([&]() { GET.execute({"1", "-104"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if only 1 argument (no movie id) (should print error)
    output = captureOutput([&]() { GET.execute({"4"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if 3 arguments (too match) (should print error)
    output = captureOutput([&]() { GET.execute({"1", "104", "105"}); });
    EXPECT_EQ(output, "400 Bad Request\n");

    // check what if user don't exist (should print error)
    output = captureOutput([&]() { GET.execute({"11", "104"}); });
    EXPECT_EQ(output, "404 Not Found\n");

    // check what if movie don't exist (should print error)
    output = captureOutput([&]() { GET.execute({"1", "99"}); });
    EXPECT_EQ(output, "404 Not Found\n");
}

// Test case 2: Check only 1 movie: 1, in execute function
TEST(GETTest, Execute_1_Movie) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_pos1(db);
    ConsoleMenu cm;
    IMenu& menu = cm;
    GETCommand GET(db, menu);

    // check the result (should print nothing because there is no other movies to GET)
    auto output = captureOutput([&]() { GET.execute({"1", "1"}); });
    EXPECT_EQ(output, "200 Ok\n\n\n");  // Adjust expected output
}

// Test case 3: Check only 3 movies: 1, 2, 3, in execute function
TEST(GETTest, Execute_3_Movies) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_pos2(db);
    ConsoleMenu cm;
    IMenu& menu = cm;
    GETCommand GET(db, menu);

    // check the result
    auto output = captureOutput([&]() { GET.execute({"1", "1"}); });
    EXPECT_EQ(output, "200 Ok\n\n3 2\n");

    // check the result
    output = captureOutput([&]() { GET.execute({"1", "2"}); });
    EXPECT_EQ(output, "200 Ok\n\n3\n");

    // check the result
    output = captureOutput([&]() { GET.execute({"1", "3"}); });
    EXPECT_EQ(output, "200 Ok\n\n2\n");
}

// Test case 4: Check regular case in execute function
TEST(GETTest, ExecuteRegularCase) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_pos3(db);
    ConsoleMenu cm;
    IMenu& menu = cm;
    GETCommand GET(db, menu);

    // check the result (by the exercise1's pdf)
    auto output = captureOutput([&]() { GET.execute({"1", "104"}); });
    EXPECT_EQ(output, "200 Ok\n\n105 106 111 110 112 113 107 108 109 114\n");
}

// Test case 5: Check description function
TEST(GETTest, DescriptionFunction) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_pos3(db);
    ConsoleMenu cm;
    IMenu& menu = cm;
    GETCommand GET(db, menu);

    // check the result
    EXPECT_EQ(GET.description(), "GET, arguments: [userid] [movieid]");
}

// Test case 6: Check rw_status function
TEST(GETTest, rw_statusFunction) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_pos3(db);
    ConsoleMenu cm;
    IMenu& menu = cm;
    GETCommand GET(db, menu);

    // check the result
    EXPECT_EQ(GET.rw_status(), "reader");
}


