#ifndef DBFILE_H
#define DBFILE_H
#include "IDataBase.h"
#include <vector>
#include <string>
class DBFile : public IDataBase {
    private:
        std::string path;
        std::string usersP;
        std::string moviesP;
    public:
        //constuctor to the class
        DBFile(std::string path);
        //pure virtual method to find movies that user watched
        virtual std::vector<std::string> findUser(std::string uid) override;
        //pure virtual method to users that watched a movie
        virtual std::vector<std::string> findMovie(std::string mid) override;
        //pure virtual method to add movies a user has watched
        virtual void updateUser(std::string uid, const std::vector<std::string>& mids) override;
        //pure virtual method to add users that watched a movie
        virtual void updateMovie(std::string mid, const std::vector<std::string>& uids) override;
        //pure virtual method to find common watched movies between 2 users
        virtual std::vector<std::string> getCommonMovies(std::string uid1, std::string uid2) override;
        //pure virtual method to find all the movie ids of watched movies
        virtual std::vector<std::string> getAllMovies() override;
        //method to clean the data from the files
        virtual void cleanUp() override;
        //pure virtual method to check if a user exists
        virtual bool isUserExists(std::string uid) override;
        //pure virtual method to check if a movie exists
        virtual bool isMovieExists(std::string mid) override;
        //pure virtual method to delete users from a movie
        virtual void deleteUsers(std::string mid, const std::vector<std::string>& uids) override;
        //pure virtual method to delete movies from a user
        virtual void deleteMovies(std::string uid, const std::vector<std::string>& mids) override;
};

#endif