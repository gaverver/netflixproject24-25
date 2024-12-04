#include "addCommand.h"

// constructor for addCommand class
addCommand::addCommand(IDataBase& UMManager) : UMManager(UMManager) {

}

std::string addCommand::description() const {
    return "add [userid] [movieid1] [movieid2] ...";
}
// helper fuction that checks if the string is constructed from digits.
bool isBuiltDigits(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        if (!('0' <= str[i] <= '9')) {
            return false;
        }
    }
    return true;
}

void addCommand::execute(const std::vector<std::string>& args) {
    // check if the user entered an invalid args size
    if (args.size() < 2) {
        return;
    }
    size_t pos;
    bool valid = true;
    std::vector<unsigned long int> passedMovies;
    std::vector<unsigned long int> userVector;
    for (size_t i = 0; i < args.size(); i++) {
        try {
            // try to convert the argument into unsigned long int
            unsigned long int convertedNum = std::stoul(args[i], &pos);
            // if the string isn't constructed from digits then it is invalid input.
            if (!isBuiltDigits(args[i])) {
                return;
            }
            // check if the whole string was parsed and is not negative
            if (!(args[i][0] == '-' || pos != args[i].length())) {
                // the first argument(i=0) is the userID and the other arguments are the moviesID
                if (i == 0) {
                    userVector.push_back(convertedNum);
                } else {
                    passedMovies.push_back(convertedNum);
                }
            }
        // if an exception is caught than just exit the function without doing anything
        } catch (...) {
            return;
        }
    }
    // update the user and movies inside the database
    UMManager.updateUser(userVector[0], passedMovies);
}