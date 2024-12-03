#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <limits>
#include <functional>
#include "../../recommendCommand.h"
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
void setup_for_test_2(const DBFile &db) {
    // clean up the data in db if there is
    db.cleanUp();

    // update all data
    db.updateUser(1, {1});
    db.updateUser(2, {1});
}

// Helper function to setup the data before testing test3
void setup_for_test_3(const DBFile &db) {
    // clean up the data in db if there is
    db.cleanUp();

    // update all data
    db.updateUser(1, {1});
    db.updateUser(2, {1, 3});
    db.updateUser(3, {2});
}

// Helper function to setup the data before testing test1, test4, test5
void setup_for_tests_1_4_5(const DBFile &db) {
    // clean up the data in db if there is
    db.cleanUp();

    // update all data
    db.updateUser(1, {100, 101, 102, 103});
    db.updateUser(2, {101, 102, 104, 105, 106});
    db.updateUser(3, {100, 104, 105, 107, 108});
    db.updateUser(4, {101, 105, 106, 107, 109, 110});
    db.updateUser(5, {100, 102, 103, 105, 108, 111});
    db.updateUser(6, {100, 103, 104, 110, 111, 112, 113});
    db.updateUser(7, {102, 105, 106, 107, 108, 109, 110});
    db.updateUser(8, {101, 104, 105, 106, 109, 111, 114});
    db.updateUser(9, {100, 103, 105, 107, 112, 113, 115});
    db.updateUser(10, {100, 102, 105, 106, 107, 109, 110, 116});
}

// Test case 1: Check invalid input in execute function
TEST(RecommendTest, ExecuteInvalidInput) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_for_tests_1_4_5(db);
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check what if user id is empty (should print nothing)
    auto output = captureOutput([&]() { recommend.execute({"", "104"}); });
    EXPECT_EQ(output, "");

    // check what if user id is not an unsigned long int (should print nothing)
    output = captureOutput([&]() { recommend.execute({"c", "123"}); });
    EXPECT_EQ(output, "");

    // check what if movie id is not an unsigned long int (should print nothing)
    output = captureOutput([&]() { recommend.execute({"1", "12x"}); });
    EXPECT_EQ(output, "");

    // check what if user id is negative (should print nothing)
    output = captureOutput([&]() { recommend.execute({"-1", "104"}); });
    EXPECT_EQ(output, "");

    // check what if movie id is negative (should print nothing)
    output = captureOutput([&]() { recommend.execute({"1", "-104"}); });
    EXPECT_EQ(output, "");

    // check what if only 1 argument (no movie id) (should print nothing)
    output = captureOutput([&]() { recommend.execute({"4"}); });
    EXPECT_EQ(output, "");

    // check what if 3 arguments (too match) (should print nothing)
    output = captureOutput([&]() { recommend.execute({"1", "104", "105"}); });
    EXPECT_EQ(output, "");

    // check what if user id is above the maximum unsigned long int (should print nothing)
    unsigned long int maxULongInt = std::numeric_limits<unsigned long int>::max();
    std::string maxPlusOne = std::to_string(static_cast<unsigned long long>(maxULongInt) + 1);

    output = captureOutput([&]() { recommend.execute({maxPlusOne, "104"}); });
    EXPECT_EQ(output, "");

    // check what if movie id is above the maximum unsigned long int (should print nothing)
    output = captureOutput([&]() { recommend.execute({"1", maxPlusOne}); });
    EXPECT_EQ(output, "");

    // check what if user don't exist (should print nothing)
    output = captureOutput([&]() { recommend.execute({"11", "104"}); });
    EXPECT_EQ(output, "");

    // check what if movie don't exist (should print nothing)
    output = captureOutput([&]() { recommend.execute({"1", "99"}); });
    EXPECT_EQ(output, "");
}

// Test case 2: Check only 1 movie: 1, in execute function
TEST(RecommendTest, Execute_1_Movie) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_for_test_2(db);
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result (should print nothing because there is no other movies to recommend on)
    auto output = captureOutput([&]() { recommend.execute({"1", "1"}); });
    EXPECT_EQ(output, "");  // Adjust expected output
}

// Test case 3: Check only 3 movies: 1, 2, 3, in execute function
TEST(RecommendTest, Execute_3_Movies) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_for_test_3(db);
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result
    auto output = captureOutput([&]() { recommend.execute({"1", "1"}); });
    EXPECT_EQ(output, "3 2");

    // check the result
    output = captureOutput([&]() { recommend.execute({"1", "2"}); });
    EXPECT_EQ(output, "3");

    // check the result
    output = captureOutput([&]() { recommend.execute({"1", "3"}); });
    EXPECT_EQ(output, "2");
}

// Test case 4: Check regular case in execute function
TEST(RecommendTest, ExecuteRegularCase) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_for_tests_1_4_5(db);
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result (by the exercise1's pdf)
    auto output = captureOutput([&]() { recommend.execute({"1", "104"}); });
    EXPECT_EQ(output, "105 106 111 110 112 113 107 108 109 114");
}

// Test case 5: Check description function
TEST(RecommendTest, DescriptionFunction) {
    // create the instances, and make sure the data directory contains the right data
    DBFile dbFile("../data");
    IDataBase& db = dbFile;
    // you could see the data in the setup function fitted to this test
    setup_for_tests_1_4_5(db);
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result
    EXPECT_EQ(recommend.description(), "recommend [userid] [movieid]");
}

// Main function to run the tests
int main(int argc, char** argv) {
    // run all tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}