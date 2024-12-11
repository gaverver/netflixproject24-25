#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>
#include "recommendCommand.h"
#include "IDataBase.h"
#include "IMenu.h"
#include "PublicFunctions.h"

// constructor
recommendCommand::recommendCommand(IDataBase& db, IMenu& menu) : db(db), menu(menu) {}

// execute, inherited from Command class
void recommendCommand::execute(const std::vector<std::string>& args) {
    // suppose to be only user id and movie id which are unsigned long int
    if (args.size() != 2) {
        return;
    }

    // casting from string to unsigned long int, and returns if there was a problem doing so
    bool isValid = true;

    // casting the first argument from string to unsigned long int
    unsigned long int uid = PublicFunctions::fromStringToULI(args[0], isValid);
    // if the casting has failed, execute of recommend should prints nothing
    if (!isValid) {
        return;
    }

    // casting the second argument from string to unsigned long int
    unsigned long int mid = PublicFunctions::fromStringToULI(args[1], isValid);
    // if the casting has failed, execute of recommend should prints nothing
    if (!isValid) {
        return;
    }

    // if the movie does not exist, execute of recommend should prints nothing
    if (!db.isMovieExists(mid)) {
        return;
    }

    // if the user does not exist, execute of recommend should prints nothing
    if (!db.isUserExists(uid)) {
        return;
    }

    // users who watched the movie 'mid'
    std::vector<unsigned long int> alikeUsers = db.findMovie(mid);

    // movies already watched by 'uid'
    std::vector<unsigned long int> alreadyWatched = db.findUser(uid);

    // movies and their ratings according to the algorithm (rate = relevance)
    std::map<unsigned long int, int> rate;

    // get all movies in the database
    std::vector<unsigned long int> allMovies = db.getAllMovies();

    // initialize the rate map for all eligible movies
    for (unsigned long int movie : allMovies) {
        // the movies that we are not supposed to recommend on is the mid and the movies uid already watched
        if (movie != mid && std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) == alreadyWatched.end()) {
            rate[movie] = 0; // Initialize with 0
        }
    }

    // go through all the users that watched the movie mid
    for (unsigned long int usr : alikeUsers) {
        // skip uid
        if (usr == uid) continue;

        // calculate similarity of that user to uid according to the algorithm
        int similarity = db.getCommonMovies(usr, uid).size();
        // movies watched by current user - 'usr'
        std::vector<unsigned long int> moviesOfAlikeUser = db.findUser(usr);

        // go therough all the movies current 'usr' watched
        for (unsigned long int movie : moviesOfAlikeUser) {
            // again, those are the movies that we are not supposed to recommend on
            if (movie == mid || std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) != alreadyWatched.end()) {
                continue;
            }
            // adding to the rate of that movie, according to the algorithm
            rate[movie] += similarity;
        }
    }

    // sortedMovies will contain the 10 best movies we will recommend on
    std::vector<unsigned long int> sortedMovies;
    // at first, sortedMovies will have all the movies that have a rate
    for (const auto& pair : rate) {
        sortedMovies.push_back(pair.first);
    }

    // sort movies by rating and ID, (cmp function here is according to the algorithm)
    std::sort(sortedMovies.begin(), sortedMovies.end(), [&rate](unsigned long int a, unsigned long int b) {
        return rate[a] > rate[b] || (rate[a] == rate[b] && a < b);
    });

    // take the top 10 (or less if there are less than 10 movies that have a rate) movies
    sortedMovies.resize(std::min<int>(10, sortedMovies.size()));

    // build recommendation string that we will print via IMenu
    std::ostringstream recommendation;
    // go through all the movies for recommendation, and chain them to 1 string
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        if (i > 0) recommendation << " ";
        recommendation << sortedMovies[i];
    }

    // finally, prints the recommendation via IMenu field
    menu.print(recommendation.str());
}

// description, inherited from Command class
std::string recommendCommand::description() const {
    // simply returns the description of recommend
    return "recommend [userid] [movieid]";
}

