#include "DBFile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
//helper function to get the first word of a string
unsigned long int first_number(std::string s) {
    //creating a stream to read from the string
    std::istringstream stream(s);
    unsigned long int firstNum;
    //reading the first word to a string
    stream >> firstNum;
    return firstNum;
}

//helper function to remove duplicates from sorted unsigned long int vector
void removeDupSortVec(std::vector<unsigned long int>& vec) {
    //if it's empty there's nothing to remove
    if (vec.empty()) return;
    std::vector<unsigned long int> result;
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
std::string mergeIds(const std::string& line, std::vector<unsigned long int>& ids) {
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
    unsigned long int lineId;
    stream >> lineId;
    //i is index for iterating over ids
    size_t i=0;
    //id is ids[i]
    unsigned long int id;
    //merging the ids
    while (!stream.eof() && i<ids.size()) {
        id = ids[i];
        if (id < lineId) {
            //add the smaller id to the string
            mergedLine += std::to_string(id) + " ";
            i++;
        } else if (id > lineId) {
            //add the smaller id to the file
            mergedLine += std::to_string(lineId) + " ";
            stream >> lineId;
        } else {
            //add the smaller id to the string
            mergedLine += std::to_string(id) + " ";
            //there are no duplicates in line and in ids
            i++;
            stream >> lineId;
        }
    }
    //add what's left to the merged line
    if (i >= ids.size()) {
        while (!stream.eof()) {
            mergedLine += std::to_string(lineId) + " ";
            stream >> lineId;
        }
    }
    while (i < ids.size()) {
        id = ids[i];
        mergedLine += std::to_string(id) + " ";
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
    cleanUp();
}
//generic function to update a file with primary id and a list of secondary ids
void genericUpdate(unsigned long int id, const std::vector<unsigned long int>& ids, std::string path1) {
    //create a copy of mids because it's a const
    std::vector<unsigned long int> idsCopy;
    idsCopy = std::move(ids);
    //sort the mids first
    std::sort(idsCopy.begin(), idsCopy.end());
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
        for (unsigned long int i: idsCopy) {
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
void DBFile::updateUser(unsigned long int uid, const std::vector<unsigned long int>& mids) {
    //update users.txt
    genericUpdate(uid, mids, usersP);
    std::vector<unsigned long int> uidVec = {uid};
    //for each movie update accordingly movies.txt
    for (unsigned long int mid: mids) {
        genericUpdate(mid, uidVec, moviesP);
    }
}
//implementing updateMovie
void DBFile::updateMovie(unsigned long int mid, const std::vector<unsigned long int>& uids) {
    //update movies.txt
    genericUpdate(mid, uids, moviesP);
    //for each user update users.txt
    std::vector<unsigned long int> midVec = {mid};
    for (unsigned long int uid: uids) {
        genericUpdate(uid, midVec, usersP);
    }
}
//generic function to find lists of ids according to id and a file
std::vector<unsigned long int> genericFind(unsigned long int id, std::string path) {
    std::vector<unsigned long int> ids = {};
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
        unsigned long int idTemp;
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
std::vector<unsigned long int> DBFile::findUser(unsigned long int uid) {
    return genericFind(uid, usersP);
}

//find users that watched movie mid
std::vector<unsigned long int> DBFile::findMovie(unsigned long int mid) {
    return genericFind(mid, moviesP);
}
//return an unsigned long int vector of all the movie ids
std::vector<unsigned long int> DBFile::getAllMovies() {
    std::ifstream moviesInFile(moviesP);
    std::vector<unsigned long int> mids = {};
    std::string line;
    std::istringstream stream;
    unsigned long int mid;
    //read the lines
    while (std::getline(moviesInFile, line)) {
        stream.str(line);
        //read first word of the line
        stream >> mid;
        //push the id to the line
        mids.push_back(mid);
    }
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

std::vector<unsigned long int> DBFile::getCommonMovies(unsigned long int uid1, unsigned long int uid2) {
    return {};
}