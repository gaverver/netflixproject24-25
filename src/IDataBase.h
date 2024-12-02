#ifndef IDATABASE_H
#define IDATABASE_H

#include <vector>


class IDataBase { 
    public:
        //pure virtual method to find movies that user watched
        virtual std::vector<unsigned long int> findUser(unsigned long int uid) = 0;
        //pure virtual method to users that watched a movie
        virtual std::vector<unsigned long int> findMovie(unsigned long int mid) = 0;
        //pure virtual method to add movies a user has watched
        virtual void updateUser(unsigned long int uid, const std::vector<unsigned long int>& mids) = 0;
        //pure virtual method to add users that watched a movie
        virtual void updateMovie(unsigned long int mid, const std::vector<unsigned long int>& uids) = 0;
        //pure virtual method to find common watched movies between 2 users
        virtual std::vector<unsigned long int> getCommonMovies(unsigned long int uid1, unsigned long int uid2) = 0;
        //pure virtual method to find all the movie ids of watched movies
        virtual std::vector<unsigned long int> getAllMovies() = 0;
        //virtual destructor to ensure proper cleanup in derived classes
        virtual ~IDataBase() = default;

};

#endif