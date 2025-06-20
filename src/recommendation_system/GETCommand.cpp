#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>
#include "GETCommand.h"
#include "IDataBase.h"
#include "IMenu.h"
#include "PublicFunctions.h"

// constructor
GETCommand::GETCommand(IDataBase& db, IMenu& menu) : db(db), menu(menu) {}

// execute, inherited from Command class
void GETCommand::execute(const std::vector<std::string>& args) {
    // suppose to be only user id and movie id which are valid ids with type string
    if (args.size() != 2) {
        // error number - 400 - invalid command
        menu.print("400 Bad Request");
        return;
    }

    // for comfort
    std::string uid = args[0];
    std::string mid = args[1];

    // check if the ids of the user and the movie are valid (as an mongoDB _id object)
    if (!IsValidId(uid) || !IsValidId(mid)) {
        menu.print("400 Bad Request");
        return;
    }

    // if the movie does not exist, execute of GET should prints error code
    if (!db.isMovieExists(mid)) {
        // error number - 404 - with the specific data base!!!, invalid command
        menu.print("404 Not Found");
        return;
    }

    // if the user does not exist, execute of GET should prints error code
    if (!db.isUserExists(uid)) {
        // error number - 404 - with the specific data base!!!, invalid command
        menu.print("404 Not Found");
        return;
    }

    // users who watched the movie 'mid'
    std::vector<std::string> alikeUsers = db.findMovie(mid);

    // movies already watched by 'uid'
    std::vector<std::string> alreadyWatched = db.findUser(uid);

    // movies and their ratings according to the algorithm (rate = relevance)
    std::map<std::string, int> rate;

    // get all movies in the database
    std::vector<std::string> allMovies = db.getAllMovies();

    // initialize the rate map for all eligible movies
    for (std::string movie : allMovies) {
        // the movies that we are not supposed to recommend on is the mid and the movies uid already watched
        if (movie != mid && std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) == alreadyWatched.end()) {
            rate[movie] = 0; // Initialize with 0
        }
    }

    // go through all the users that watched the movie mid
    for (std::string usr : alikeUsers) {
        // skip uid
        if (usr == uid) continue;

        // calculate similarity of that user to uid according to the algorithm
        int similarity = db.getCommonMovies(usr, uid).size();
        // movies watched by current user - 'usr'
        std::vector<std::string> moviesOfAlikeUser = db.findUser(usr);

        // go therough all the movies current 'usr' watched
        for (std::string movie : moviesOfAlikeUser) {
            // again, those are the movies that we are not supposed to recommend on
            if (movie == mid || std::find(alreadyWatched.begin(), alreadyWatched.end(), movie) != alreadyWatched.end()) {
                continue;
            }
            // adding to the rate of that movie, according to the algorithm
            rate[movie] += similarity;
        }
    }

    // sortedMovies will contain the 10 best movies we will recommend on
    std::vector<std::string> sortedMovies;
    // at first, sortedMovies will have all the movies that have a rate
    for (const auto& pair : rate) {
        sortedMovies.push_back(pair.first);
    }

    // sort movies by rating and ID, (cmp function here is according to the algorithm)
    std::sort(sortedMovies.begin(), sortedMovies.end(), [&rate](std::string a, std::string b) {
        // compareHugeNumbers is from PublicFunctions to comapre between _id objects
        return rate[a] > rate[b] || (rate[a] == rate[b] && compareHugeNumbers(a, b));
    });

    // take the top 10 (or less if there are less than 10 movies that have a rate) movies
    sortedMovies.resize(std::min<int>(10, sortedMovies.size()));

    // build recommendation string that we will print via IMenu
    std::ostringstream recommendation;
    // success number - 200 - the command has been successfully answered
    recommendation << "200 Ok\n\n";
    // go through all the movies for recommendation, and chain them to 1 string
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        if (i > 0) recommendation << " ";
        recommendation << sortedMovies[i];
    }

    // finally, prints the recommendation via IMenu field
    menu.print(recommendation.str());
}

// description, inherited from Command class
std::string GETCommand::description() const {
    // simply returns the description of GET
    return "GET, arguments: [userid] [movieid]";
}

// rw_status, inherited from Command class
std::string GETCommand::rw_status() const {
    // simply returns the status of GET (reader, writer or none)
    return "reader";
}

