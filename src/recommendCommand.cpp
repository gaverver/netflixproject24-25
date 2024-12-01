#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include "recommendCommand.h"
#include "IDataBase.h"
#include "IMenu.h"

// a helper private function to help casting from string to int
int recommendCommand::castStringToInt(const std::string& str, bool& isValid) {
    try {
        // tracks the position after the last parsed character
        std::size_t pos;
        // casting...
        int num = std::stoi(str, &pos);
        // check if the whole string was parsed
        if (pos != str.length()) {
            // if not, str is not convertable
            isValid = false;
            return 0;
        }
        // casting completed successfully 
        isValid = true;
        return num;
    // if catching anything, it's just not convertable
    } catch (...) {
        isValid = false;
        return 0;
    }
}

// constructor
recommendCommand::recommendCommand(IDataBase& db, IMenu& menu) : db(db), menu(menu) {}

void recommendCommand::execute(const std::vector<std::string>& args) override {
    if (args.size() != 2) {
        return;
    }

    bool isValid = true;
    int uid = castStringToInt(args[0], isValid);
    int mid = castStringToInt(args[1], isValid);
    if (!isValid || uid < 0 || mid < 0) {
        return;
    }

    // Users who watched the movie `mid`
    std::vector<int> alikeUsers = db.findMovie(mid);
    // Movies already watched by `uid`
    std::vector<int> alreadyWatched = db.findUser(uid);
    // Movies and their ratings
    std::map<int, int> rate;

    // Get all movies in the database
    std::vector<int> allMovies = db.getAllMovies();

    // Initialize the rate map for all eligible movies
    for (int movie : allMovies) {
        if (movie != mid && std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) == alreadyWatched.end()) {
            rate[movie] = 0; // Initialize with 0
        }
    }

    for (int usr : alikeUsers) {
        if (usr == uid) continue;

        // Calculate similarity
        int similarity = db.getCommonMovies(usr, uid).size();
        // Movies watched by `usr`
        std::vector<int> moviesOfAlikeUser = db.findUser(usr);

        for (int movie : moviesOfAlikeUser) {
            if (movie == mid || std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) != alreadyWatched.end()) {
                continue;
            }
            rate[movie] += similarity;
        }
    }

    // Sort movies by rating and ID
    std::vector<int> sortedMovies;
    for (const auto& pair : rate) {
        sortedMovies.push_back(pair.first);
    }

    std::sort(sortedMovies.begin(), sortedMovies.end(), [&rate](int a, int b) {
        return rate[a] > rate[b] || (rate[a] == rate[b] && a < b);
    });

    // take the top 10 movies
    sortedMovies.resize(std::min<int>(10, sortedMovies.size()));

    // Build recommendation string
    std::ostringstream recommendation;
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        if (i > 0) recommendation << " ";
        recommendation << sortedMovies[i];
    }

    menu.print(recommendation.str());
}

std::string recommendCommand::description() const override {
    return "recommend [userid] [movieid]";
}

