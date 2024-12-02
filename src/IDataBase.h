#ifndef IDATABASE_H
#define IDATABASE_H

#include <vector>


class IDataBase { 
    public:
        //pure virtual method to find movies that user watched
        virtual std::vector<int> findUser(int uid) = 0;
        //pure virtual method to users that watched a movie
        virtual std::vector<int> findMovie(int mid) = 0;
        //pure virtual method to add movies a user has watched
        virtual void updateUser(int uid, const std::vector<int>& mids) = 0;
        //pure virtual method to add users that watched a movie
        virtual void updateMovie(int mid, const std::vector<int>& uids) = 0;
        //pure virtual method to find common watched movies between 2 users
        virtual std::vector<int> getCommonMovies(int uid1, int uid2) = 0;
        //pure virtual method to find all the movie ids of watched movies
        virtual std::vector<int> getAllMovies() = 0;
        //virtual destructor to ensure proper cleanup in derived classes
        virtual ~IDataBase() = default;

};

#endif