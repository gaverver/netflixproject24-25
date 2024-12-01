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

// a helper private function to help casting from string to unsigned long int
unsigned long int recommendCommand::castStringToULI(const std::string& str, bool& isValid) {
    try {
        // tracks the position after the last parsed character
        std::size_t pos;
        // casting...
        unsigned long int num = std::stoul(str, &pos);
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
    unsigned long int uid = castStringToULI(args[0], isValid);
    unsigned long int mid = castStringToULI(args[1], isValid);
    if (!isValid) {
        return;
    }

    // Users who watched the movie `mid`
    std::vector<unsigned long int> alikeUsers = db.findMovie(mid);
    // Movies already watched by `uid`
    std::vector<unsigned long int> alreadyWatched = db.findUser(uid);
    // Movies and their ratings
    std::map<unsigned long int, int> rate;

    // Get all movies in the database
    std::vector<unsigned long int> allMovies = db.getAllMovies();

    // Initialize the rate map for all eligible movies
    for (unsigned long int movie : allMovies) {
        if (movie != mid && std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) == alreadyWatched.end()) {
            rate[movie] = 0; // Initialize with 0
        }
    }

    for (unsigned long int usr : alikeUsers) {
        if (usr == uid) continue;

        // Calculate similarity
        int similarity = db.getCommonMovies(usr, uid).size();
        // Movies watched by `usr`
        std::vector<unsigned long int> moviesOfAlikeUser = db.findUser(usr);

        for (unsigned long int movie : moviesOfAlikeUser) {
            if (movie == mid || std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) != alreadyWatched.end()) {
                continue;
            }
            rate[movie] += similarity;
        }
    }

    // Sort movies by rating and ID
    std::vector<unsigned long int> sortedMovies;
    for (const auto& pair : rate) {
        sortedMovies.push_back(pair.first);
    }

    std::sort(sortedMovies.begin(), sortedMovies.end(), [&rate](unsigned long int a,  b) {
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

