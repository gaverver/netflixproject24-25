#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include "DELETECommand.h"
#include "IDataBase.h"
#include "IMenu.h"
#include "PublicFunctions.h"

// constructor
DELETECommand::DELETECommand(IDataBase& db, IMenu& menu) : db(db), menu(menu) {}

// execute, inherited from Command class
void DELETECommand::execute(const std::vector<std::string>& args) {
    // suppose to be at least 2 arguments: user id and at least 1 movie id
    if (args.size() < 2) {
        // error number - 400 - invalid command
        menu.print("400 Bad Request");
        return;
    }

    // casting from string to unsigned long int, and returns if there was a problem doing so
    bool isValid = true;

    // casting the first argument from string to unsigned long int (the user id)
    unsigned long int uid = fromStringToULI(args[0], isValid);
    // if the casting has failed, execute of DELETE should prints error code
    if (!isValid) {
        // error number - 400 - invalid command
        menu.print("400 Bad Request");
        return;
    }

    // two bools to check the validity and the error number that should be printed
    bool error_400 = false;
    bool error_404 = false;

    // if the user does not exist, execute of DELETE should prints error code
    if (!db.isUserExists(uid)) {
        // error number - 404 - with the specific data base!!!, invalid command
        // waiting to see if error number 400 will occur in the next iterations
        error_404 = true;
    }

    // this will contain all the movies we want to delete from the user watched list
    std::vector<unsigned long int> movies_to_delete;
    // all the movies that user watched
    std::vector<unsigned long int> watched_movies = db.findUser(uid);
    // go through all the arguments (except the first one) to constract the movies
    for (int i = 1; i < args.size(); i++) {
        // casting the current argument from string to unsigned long int (a movie id)
        unsigned long int mid = fromStringToULI(args[i], isValid);
        // if the casting has failed, execute of DELETE should prints error code
        if (!isValid) {
            // error number - 400 - invalid command, that's the final error number
            error_400 = true;
            break;
        }
        // else, the casting succeed
        // if the current movie does not exist, execute of DELETE should prints error code
        if (!db.isMovieExists(mid)) {
            // error number - 404 - with the specific data base!!!, invalid command
            // waiting to see if error number 400 will occur in the next iterations
            error_404 = true;
            continue;
        }
        // else, if the movie hasn't been watched by the user, execute of DELETE should prints error code
        if (std::find(watched_movies.begin(), watched_movies.end(), mid) == watched_movies.end()) {
            // error number - 404 - with the specific data base!!!, invalid command
            // waiting to see if error number 400 will occur in the next iterations
            error_404 = true;
            continue;
        }
        // else, this movie is good to go, and we push him to the vector of movies_to_delete
        movies_to_delete.push_back(mid);
    }

    // check validity
    if (error_400) {
        menu.print("400 Bad Request");
        return;
    }
    // else, if error 404 should be printed
    if (error_404) {
        menu.print("404 Not Found");
        return;
    }

    // if we reached here, it means that everything is OK and we should execute the delete
    db.deleteMovies(uid, movies_to_delete);

    // prints a success code
    menu.print("204 No Content");
}

// description, inherited from Command class
std::string DELETECommand::description() const {
    // simply returns the description of DELETE
    return "DELETE, arguments: [userid] [movieid1] [movieid2] ...";
}

// rw_status, inherited from Command class
std::string DELETECommand::rw_status() const {
    // simply returns the status of DELETE (reader, writer or none)
    return "writer";
}
