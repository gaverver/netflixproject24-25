#ifndef PUBLICFUNCTIONS_H
#define PUBLICFUNCTIONS_H

#include <string>

class PublicFunctions {   
public:
    // helper function to convert from string to unsigned long int and know if it failed
    static unsigned long int fromStringToULI(const std::string& str, bool& isValid);
};

#endif