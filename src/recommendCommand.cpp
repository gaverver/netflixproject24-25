#include <gtest/gtest.h>
#include <string>
#include <sstream>  // For std::ostringstream
#include <vector>
#include <map>
#include <iostream> // For std::cout
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include "../../recommendCommand.h"
#include "../../IDataBase.h"
#include "../../IMenu.h"
#include "ConsoleMenu.h"

class recommendCommand : public ICommand {
private:
    // Use references for interface types
    IdataBase& db;
    IMenu& menu;

    // a helper private function to help casting from string to int
    int castStringToInt(std::string str, bool& isValid) {
        try {
        // Tracks the position after the last parsed character
        std::size_t pos;
        // casting...
        int num = std::stoi(str, &pos);

        // Check if the whole string was parsed
        if (pos != str.length()) {
            isValid = false;
            return 0;
        }

        // try to catch if the input to stoi is invalid (not a number)
        } catch (const std::invalid_argument &e) {
            isValid = false;
            return 0;
        }
        // try to catch if the input is out of the range of int (too big or too small)
        catch (const std::out_of_range &e) {
            isValid = false;
            return 0;
        }

        // casting completed successfully
        isValid = true;
        return num;
    }

    // this function will swap between two variables, for selectionSort algorithm
    void swap(int& x, int& y) {
        // putting the value of x in other variable
        int temp = x;
        // putting the value of y in x
        x = y;
        // finally, putinng the value of temp (which is the first value of x) in y
        y = temp;
    }

    // TODO - fix this function
    // a helper private function to help sorting the movies by their rate
    // this function will sort by rate and return the top 10 elements (by selectionSort algorithm)
    std::vector<int> sortUntilTen(const std::map<int, int>& rate) {
        std::vector<int> arr;

        // fill the vector with movie IDs
        for (const auto& pair : rate) {
            arr.push_back(pair.first);
        }

        // gets the size of the vector arr
        int n = arr.size();

        // perform selectionSort to find the top 10 elements
        for (int i = 0; i < std::min(10, n); i++) {
            int maxArg = i;

            for (int j = i + 1; j < n; j++) {
                // determine if current movie is "better" (higher rate or lower ID on tie)
                if (rate[arr[j]] > rate[arr[maxArg]] ||
                    (rate[arr[j]] == rate[arr[maxArg]] && arr[j] < arr[maxArg])) {
                    maxArg = j;
                }
            }

            // Swap the current element with the best element found
            swap(arr[maxArg], arr[i]);
        }

        // Resize to ensure only the top 10 elements are returned
        if (n > 10) {
            arr.resize(10);
        }

        return arr;
    }

public:
    // Constructor: Initialize with references to IDataBase and IMenu
    recommendCommand(IdataBase& db2, IMenu& menu2) : db(db2), menu(menu2) {}

    // Implement the execute method
    void execute(const std::vector<std::string>& args)  not - returns.
        if (args.size() != 2) {
            return;
        }

        // isValid will help to recognize if the input is invalid, initialized to true
        bool isValid = true;
        // creates the user id and the movie id that I want to get from that casting
        int uid, int mid;
        // casting first argument
        uid = this->castStringToInt(arg[0], isValid);override {
        // check if there are exactly 2 strings in args, and if
        // input is invalid, and so I returned (the id of a user should not be negative)
        if (isValid == false || uid < 0) {
            return;
        }
        // casting second argument
        mid = this->castStringToInt(arg[1], isValid);
        // input is invalid, and so I returned (the id of a movie should not be negative)
        if (isValid == false || mid < 0) {
            return;
        }
        
        // all the users that watched the movie with id: mid
        std::vector<int> alikeUsers = this->db.findMovie(mid);
        // this will match a user to the number of common movies watched with user uid
        std::map<int, int> similarity;
        // usr is an iterator going through all alikeUsers
        std::vector<int>::iterator usr;
        // gets all the movies into that vector
        std::vector<int> allMovies = this->db.getAllMovies;
        // this will match a movie (except mid) to his rating according to the algorithm
        // a movie's rate is the sum of the similarity of the alikeUsers who watched that movie
        std::map<int, int> rate;
        // movie is an iterator going through all movies
        std::vector<int>::iterator movie;
        // going through all movies except mid and initializing the rate map
        for (movie = allMovies.begin(); movie != allMovies.end(); ++movie) {
            // avoiding the mid movie
            if (*movie == mid) {
                continue;
            }
            rate[*movie] = 0;
        }

        // a vector to go throught all movies for each alikeUser, for calculating the rate
        std::vector<int> moviesOfAlikeUser;
        // going through all alikeUsers for calculating the rate of all movies
        for (usr = alikeUsers.begin(); usr != alikeUsers.end(); ++usr) {
            // skip user uid since he's not suppose to be included
            if (*usr == uid) {
                continue;
            }
            // insert the current user to the similarity map.
            similarity[*usr] = (this->db.getCommonMovies(*usr, uid)).size;
            // finding all the movies that current usr watched
            moviesOfAlikeUser = this->db.findUser(*usr);
            // go through every movie that current usr watched
            for (movie = moviesOfAlikeUser.begin(); movie != moviesOfAlikeUser.end(); ++movie) {
            // avoiding the mid movie
            if (*movie == mid) {
                continue;
            }
            // adding to the rate of that movie according to the alogrithm...
            // ...because that movie was watched by an alikeUser
            rate[*movie] += similarity[*usr];
            }
        }

        // sorting the movies and gets the 10 best movies for the recommendation
        std::vector<int> tenBestMovies = this->sortUntilTen(rate);

        // the final recommendation represented by a string
        std::string recommendation = "";
        // go through all best movies and combine them to the final result
        for (movie = tenBestMovies.begin(); movie != tenBestMovies.end(); ++movie) {
            recommendation += std::to_string(*movie) + " ";
        }

        // if reccomendation isn't empty, it ends with a space char: " ", and it shouldn't
        if (recommendation.size > 0) {
            recommendation.pop_back();
        }

        // finally, printing to the user his recommendation through the IMenu
        this->menu.print(recommendation);
    }

    // Implement the description method
    std::string description() const override {
        return "recommend [userid] [movieid]";
    }
};
