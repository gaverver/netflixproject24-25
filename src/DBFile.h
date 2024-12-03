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
        virtual std::vector<unsigned long int> findUser(unsigned long int uid) override;
        //pure virtual method to users that watched a movie
        virtual std::vector<unsigned long int> findMovie(unsigned long int mid) override;
        //pure virtual method to add movies a user has watched
        virtual void updateUser(unsigned long int uid, const std::vector<unsigned long int>& mids) override;
        //pure virtual method to add users that watched a movie
        virtual void updateMovie(unsigned long int mid, const std::vector<unsigned long int>& uids) override;
        //pure virtual method to find common watched movies between 2 users
        virtual std::vector<unsigned long int> getCommonMovies(unsigned long int uid1, unsigned long int uid2) override;
        //pure virtual method to find all the movie ids of watched movies
        virtual std::vector<unsigned long int> getAllMovies() override;
        //method to clean the data from the files
        virtual void cleanUp() override;
};

#endif