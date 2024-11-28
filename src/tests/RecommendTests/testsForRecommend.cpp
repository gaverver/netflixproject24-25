#include <gtest/gtest.h>
#include <string>
#include <sstream>  // For std::ostringstream
#include <vector>
#include <iostream> // For std::cout
#include "../../recommendCommand.h"
#include "../../DBFile.h"
#include "../../IMenu.h"
#include "ConsoleMenu.h"

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

// Test case 1: Check invalid input
TEST(RecommendTest, ExecuteInvalidInput) {
    // create DBFile's instance (using the File System data base)
    IDataBase db = new DBFile("/usr/src/Netflix/data");
    // create ConsoleMenu's instance (print through screen)
    IMenu menu = new ConsoleMenu();
    // create recommandCommand's instance
    recommendCommand recommend(db, menu);

    // check what if user id is empty (should print nothing)
    auto output = captureOutput([&]() { recommend.execute({"", "104"}); });
    EXPECT_EQ(output, "");

    // check what if user id is not an int (should print nothing)
    output = captureOutput([&]() { recommend.execute({"c", "123"}); });
    EXPECT_EQ(output, "");

    // check what if movie id is not an int (should print nothing)
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

    // check what if user id is above the maximum int (should print nothing)
    output = captureOutput([&]() { recommend.execute({"2147483648", "104"}); });
    EXPECT_EQ(output, "");

    // check what if user id is below the minimum int (should print nothing)
    output = captureOutput([&]() { recommend.execute({"1", "-2147483649"}); });
    EXPECT_EQ(output, "");
}

// Test case 2: Check only 1 movie: 1
TEST(RecommendTest, Execute_1_Movie) {
    // create the instances, and make sure the data directory contains the right data
    IDataBase db = new DBFile("/usr/src/Netflix/data");
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result (should print nothing because there is no other movies to recommend on)
    auto output = captureOutput([&]() { recommend.execute({"1", "1"}); });
    EXPECT_EQ(output, "");  // Adjust expected output
}

// Test case 3: Check only 3 movies: 1, 2, 3
TEST(RecommendTest, Execute_3_Movies) {
    // create the instances, and make sure the data directory contains the right data
    IDataBase db = new DBFile("/usr/src/Netflix/data");
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result
    auto output = captureOutput([&]() { recommend.execute({"1", "1"}); });
    EXPECT_EQ(output, "3 2");

    // check the result
    output = captureOutput([&]() { recommend.execute({"1", "2"}); });
    EXPECT_EQ(output, "1 3");

    // check the result
    output = captureOutput([&]() { recommend.execute({"1", "3"}); });
    EXPECT_EQ(output, "1 2");
}

// Test case 4: Check regular case
TEST(RecommendTest, ExecuteRegularCase) {
    // create the instances, and make sure the data directory contains the right data
    IDataBase db = new DBFile("/usr/src/Netflix/data");
    IMenu menu = new ConsoleMenu();
    recommendCommand recommend(db, menu);

    // check the result (by the exercise1's pdf)
    auto output = captureOutput([&]() { recommend.execute({"1", "104"}); });
    EXPECT_EQ(output, "105 106 111 110 112 113 107 108 109 114");
}

// Test case 5: Check description function
TEST(RecommendTest, DescriptionFunction) {
    // create the instances (should not depend on the data)
    IDataBase db = new DBFile("/usr/src/Netflix/data");
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