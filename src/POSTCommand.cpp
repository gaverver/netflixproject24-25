#include "POSTCommand.h"
#include "PublicFunctions.h"

// constructor for addCommand class
POSTCommand::POSTCommand(IDataBase& UMManager, IMenu& menu) : UMManager(UMManager), menu(menu) {

}
// function to return the description of the command
std::string POSTCommand::description() const {
    return "POST, arguments: [userid] [movieid1] [movieid2] ...";
}
// function to return the status of the command. In this case - writer.
std::string POSTCommand::rw_status() const {
    return "writer";
}

void POSTCommand::execute(const std::vector<std::string>& args) {
    // check if enought arguments were entered.
    if (args.size() < 2) {
        menu.print("400 Bad Request");
        return;
    }
    std::vector<unsigned long int> passedMovies;
    std::vector<unsigned long int> userVector;
    bool isValid = true;
    for (size_t i = 0; i < args.size(); i++) {
        unsigned long int convertedNum = fromStringToULI(args[i], isValid);
        // if one of the arguments is invalid so it prints an appropriate message that the arguments aren't good(400 Bad Request)
        if (!isValid) {
            menu.print("400 Bad Request");
            return;
        }
        // the first argument(i=0) is the userID and the other arguments are the moviesID
        if (i == 0) {
            userVector.push_back(convertedNum);
        } else {
            passedMovies.push_back(convertedNum);
        }
    }
    // if the user already exist it is not a legal use of the command - prints an appropriate message
    if (UMManager.isUserExists(userVector[0])) {
        menu.print("404 Not Found");
        return;
    }
    // update the user and movies inside the database
    UMManager.updateUser(userVector[0], passedMovies);
    // print a message that the data has benn updated
    menu.print("201 Created");
}