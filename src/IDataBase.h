#ifndef IDATABASE_H
#define IDATABASE_H
#include <string>
#include <vector>


class IDataBase { 
    public:
        //pure virtual method to find movies that user watched
        virtual std::vector<std::string> findUser(std::string uid) = 0;
        //pure virtual method to users that watched a movie
        virtual std::vector<std::string> findMovie(std::string mid) = 0;
        //pure virtual method to add movies a user has watched
        virtual void updateUser(std::string uid, const std::vector<std::string>& mids) = 0;
        //pure virtual method to add users that watched a movie
        virtual void updateMovie(std::string mid, const std::vector<std::string>& uids) = 0;
        //pure virtual method to find common watched movies between 2 users
        virtual std::vector<std::string> getCommonMovies(std::string uid1, std::string uid2) = 0;
        //pure virtual method to find all the movie ids of watched movies
        virtual std::vector<std::string> getAllMovies() = 0;
        //virtual destructor to ensure proper cleanup in derived classes
        virtual ~IDataBase() = default;
        //pure virtual method to cleans all data
        virtual void cleanUp() = 0;
        //pure virtual method to check if a user exists
        virtual bool isUserExists(std::string uid) = 0;
        //pure virtual method to check if a movie exists
        virtual bool isMovieExists(std::string mid) = 0;
        //pure virtual method to delete users from a movie
        virtual void deleteUsers(std::string mid, const std::vector<std::string>& uids) = 0;
        //pure virtual method to delete movies from a user
        virtual void deleteMovies(std::string uid, const std::vector<std::string>& mids) = 0;

};

#endif