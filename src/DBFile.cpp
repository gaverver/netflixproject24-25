#include "DBFile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "PublicFunctions.h"

//operator overloading for the strings
bool operator<(const std::string& num1, const std::string& num2) {
    return compareHugeNumbers(num1, num2);
}
bool operator>(const std::string& num1, const std::string& num2) {
    return num2 < num1;
}
bool operator<=(const std::string& num1, const std::string& num2) {
    return !(num1 > num2);
}
bool operator>=(const std::string& num1, const std::string& num2) {
    return !(num1 < num2);
}
bool operator==(const std::string& num1, const std::string& num2) {
    return !(num1 < num2) && !(num1 > num2);
}
bool operator!=(const std::string& num1, const std::string& num2) {
    return !(num1 == num2);
}


//helper function to get the first word of a string
std::string first_number(std::string s) {
    //creating a stream to read from the string
    std::istringstream stream(s);
    std::string firstNum;
    //reading the first word to a string
    stream >> firstNum;
    return firstNum;
}

//helper function to remove duplicates from sorted unsigned long int vector
void removeDupSortVec(std::vector<std::string>& vec) {
    //if it's empty there's nothing to remove
    if (vec.empty()) return;
    std::vector<std::string> result;
    result.push_back(vec[0]);
    //put only non-duplicate items
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] != vec[i - 1]) {
            result.push_back(vec[i]);
        }
    }
    //move the result to the vec
    vec = std::move(result);
}

//helper function to merge a list of sorted ids to a line of sorted ids in file
std::string mergeIds(const std::string& line, std::vector<std::string>& ids) {
    //remove duplicates from ids
    removeDupSortVec(ids);
    //define a stream for reading from the line
    std::istringstream stream(line);
    std::string mergedLine;
    //reading the first word to the merged line (primary key)
    stream >> mergedLine;
    //add the space after the first word
    mergedLine += " ";
    //reading the other numbers in the line
    std::string lineId;
    stream >> lineId;
    //i is index for iterating over ids
    size_t i=0;
    //id is ids[i]
    std::string id;
    //merging the ids
    while (!stream.eof() && i<ids.size()) {
        id = ids[i];
        if (id < lineId) {
            //add the smaller id to the string
            mergedLine += id + " ";
            i++;
        } else if (id > lineId) {
            //add the smaller id to the file
            mergedLine += lineId + " ";
            stream >> lineId;
        } else {
            //add the smaller id to the string
            mergedLine += id + " ";
            //there are no duplicates in line and in ids
            i++;
            stream >> lineId;
        }
    }
    //add what's left to the merged line
    if (i >= ids.size()) {
        while (!stream.eof()) {
            mergedLine += lineId + " ";
            stream >> lineId;
        }
    }
    while (i < ids.size()) {
        id = ids[i];
        mergedLine += id + " ";
        i++;
    }


    return mergedLine;
}

//modifiy file line to a new line
void modifyFileLine(const std::string& filePath, unsigned long int lineNumber, const std::string& newLine) {
    std::ifstream inputFile(filePath);

    //read all the lines unsigned long into a vector
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    //modifiy the line
    lines[lineNumber - 1] = newLine;
    //write all the lines to the file
    std::ofstream outputFile(filePath);
    for (std::string mLine : lines) {
        outputFile << mLine << "\n";
    }
    outputFile.close();

}

//creating constructor
DBFile::DBFile(std::string path) : path(path) {
    //userP is path to users.txt and moviesP is path to movies.txt
    this->usersP = path + "/users.txt";
    this->moviesP = path + "/movies.txt";
    //create the files if they're not exists
    std::ofstream users(usersP, std::ios::app);
    users.close();
    std::ofstream movies(moviesP, std::ios::app);
    movies.close();
}
//generic function to update a file with primary id and a list of secondary ids
void genericUpdate(std::string id, const std::vector<std::string>& ids, std::string path1) {
    //create a copy of mids because it's a const
    std::vector<std::string> idsCopy;
    idsCopy = std::move(ids);
    //sort the mids first
    std::sort(idsCopy.begin(), idsCopy.end());
    //remove duplicates
    removeDupSortVec(idsCopy);
    //input file to read
    std::ifstream inFile(path1);
    std::string line = "garbage";
    //counter for the line
    unsigned long int k=0;
    //represents if we found the wanted line
    bool found = false;
    while (std::getline(inFile, line)) {
        k++;
        if (first_number(line) == id) {
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) {
        //write the new line in the end of the file
        std::ofstream outFile(path1, std::ios::app);
        outFile << id << " ";
        for (std::string i: idsCopy) {
            outFile << i << " ";
        }
        //add \n to end the line
        outFile << std::endl;
        outFile.close();
    } else {
        //modify the line
        std::string modifiedLine = mergeIds(line, idsCopy);
        modifyFileLine(path1, k, modifiedLine);
    }
}
//implementing updateUser
void DBFile::updateUser(std::string uid, const std::vector<std::string>& mids) {
    //update users.txt
    genericUpdate(uid, mids, usersP);
    std::vector<std::string> uidVec = {uid};
    //for each movie update accordingly movies.txt
    for (std::string mid: mids) {
        genericUpdate(mid, uidVec, moviesP);
    }
}
//implementing updateMovie
void DBFile::updateMovie(std::string mid, const std::vector<std::string>& uids) {
    //update movies.txt
    genericUpdate(mid, uids, moviesP);
    //for each user update users.txt
    std::vector<std::string> midVec = {mid};
    for (std::string uid: uids) {
        genericUpdate(uid, midVec, usersP);
    }
}
//generic function to find lists of ids according to id and a file
std::vector<std::string> genericFind(std::string id, std::string path) {
    std::vector<std::string> ids = {};
    std::ifstream inFile(path);
    std::string line = "garbage";
    bool found = false;
    //find the line
    while (std::getline(inFile, line)) {
        if (first_number(line) == id) {
            found = true;
            break;
        }
    }
    if (found) {
        std::string idTemp;
        std::istringstream stream(line);
        //read the uneccesary first id (users/movies id)
        stream >> idTemp;
        //add the ids to the vector
        while (stream >> idTemp) {
            ids.push_back(idTemp);
        }
        return ids;
    } else {
        //return empty vector
        return ids;
    }
}

//find movies user uid watched
std::vector<std::string> DBFile::findUser(std::string uid) {
    return genericFind(uid, usersP);
}

//find users that watched movie mid
std::vector<std::string> DBFile::findMovie(std::string mid) {
    return genericFind(mid, moviesP);
}
//return an unsigned long int vector of all the movie ids
std::vector<std::string> DBFile::getAllMovies() {
    std::ifstream moviesInFile(moviesP);
    std::vector<std::string> mids = {};
    std::string line;
    std::istringstream stream;
    std::string mid;
    //read the lines
    while (std::getline(moviesInFile, line)) {
        stream.str(line);
        //read first word of the line
        stream >> mid;
        //push the id to the line
        mids.push_back(mid);
    }
    //
    std::sort(mids.begin(), mids.end());
    removeDupSortVec(mids);
    return mids;
}

//clean the files in data
void DBFile::cleanUp() {
    //open with trunc the delete all the files content
    std::ofstream users(usersP, std::ios::trunc);
    users.close();
    std::ofstream movies(moviesP, std::ios::trunc);
    movies.close();
}

// this function gets 2 user ids, and return there common movies in a vector
std::vector<std::string> DBFile::getCommonMovies(std::string uid1, std::string uid2) {
    // get the movies of user 1
    std::vector<std::string> mvs1 = this->findUser(uid1);
    // get the movies of user 2
    std::vector<std::string> mvs2 = this->findUser(uid2);
    // this will keep all the common movies of user 1 and user 2
    std::vector<std::string> commonMovies;
    // initialize indices to traverse both vectors
    int m1 = 0, m2 = 0;
    
    // since both vectors are sorted, we can use two pointers to find common elements
    while (m1 < mvs1.size() && m2 < mvs2.size()) {
        if (mvs1[m1] == mvs2[m2]) {
            // add to commonMovies, then increment both pointers
            commonMovies.push_back(mvs1[m1]);
            m1++;
            m2++;
        }
        else if (mvs1[m1] < mvs2[m2]) {
            // increment m1 if the movie in mvs1 is smaller
            m1++;
        }
        else {
            // increment m2 if the movie in mvs2 is smaller
            m2++;
        }
    }
    return commonMovies;
}
//generic function to check if a user exists
bool genericExists(std::string id, std::string path) {
    std::vector<std::string> ids = {};
    std::ifstream inFile(path);
    std::string line = "garbage";
    bool found = false;
    //find the line
    while (std::getline(inFile, line)) {
        if (first_number(line) == id) {
            found = true;
            break;
        }
    }
    return found;
}
//checks if a user exists
bool DBFile::isUserExists(std::string uid) {
    return genericExists(uid, usersP);
}
//checks if a movie exists
bool DBFile::isMovieExists(std::string mid) {
    return genericExists(mid, moviesP);
}

std::string deleteIds(const std::string& line, std::vector<std::string>& ids) {
    //remove duplicates from ids
    removeDupSortVec(ids);
    //define a stream for reading from the line
    std::istringstream stream(line);
    std::string deletedLine;
    //reading the first word to the merged line (primary key)
    stream >> deletedLine;
    //add the space after the first word
    deletedLine += " ";
    //reading the other numbers in the line
    std::string lineId;
    stream >> lineId;
    //i is index for iterating over ids
    size_t i=0;
    //id is ids[i]
    std::string id;
    //merging the ids
    while (!stream.eof() && i<ids.size()) {
        id = ids[i];
        if (id < lineId) {
            //id won't be in the line
            i++;
        } else if (id > lineId) {
            //add the smaller id to the file
            deletedLine += lineId + " ";
            stream >> lineId;
        } else {
            //there are no duplicates in line and in ids, so just increment the indexes by 1
            i++;
            stream >> lineId;
        }
    }
    //add what's left to the merged line
    if (i >= ids.size()) {
        while (!stream.eof()) {
            deletedLine += lineId + " ";
            stream >> lineId;
        }
    }
    return deletedLine;
}

void genericDelete(std::string id, const std::vector<std::string>& ids, std::string path1) {
    //create a copy of mids because it's a const
    std::vector<std::string> idsCopy;
    idsCopy = std::move(ids);
    //sort the mids first
    std::sort(idsCopy.begin(), idsCopy.end());
    //remove duplicates
    removeDupSortVec(idsCopy);
    //input file to read
    std::ifstream inFile(path1);
    std::string line = "garbage";
    //counter for the line
    unsigned long int k=0;
    //represents if we found the wanted line
    bool found = false;
    while (std::getline(inFile, line)) {
        k++;
        if (first_number(line) == id) {
            found = true;
            break;
        }
    }
    inFile.close();
    if (found) {
        //modify the line
        std::string modifiedLine = deleteIds(line, idsCopy);
        modifyFileLine(path1, k, modifiedLine);
    }
}
//deletes movies from a given user
void DBFile::deleteMovies(std::string uid, const std::vector<std::string>& mids) {
    //delete the movies from users.txt
    genericDelete(uid, mids, usersP);
    std::vector<std::string> uidVec = {uid};
    //for each movie delete the user in movies.txt
    for (std::string mid: mids) {
        genericDelete(mid, uidVec, moviesP);
    }
}

//deletes users from a given movie
void DBFile::deleteUsers(std::string mid, const std::vector<std::string>& uids) {
    //delete the users from movies.txt
    genericDelete(mid, uids, moviesP);
    std::vector<std::string> midVec = {mid};
    //for each user delete the movie in users.txt
    for (std::string uid: uids) {
        genericDelete(uid, midVec, usersP);
    }
}