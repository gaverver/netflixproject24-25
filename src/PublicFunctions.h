#ifndef PUBLICFUNCTIONS_H
#define PUBLICFUNCTIONS_H

#include <string>
#include "ICommand.h"
// helper function to convert from string to unsigned long int and know if it failed
unsigned long int fromStringToULI(const std::string& str, bool& isValid);
std::string captureCommandOutput(ICommand* Runner, const std::vector<std::string>& args);
//helper functio to remove the zeros at the beginning
std::string removeZeros(const std::string& num);
//helper function to compare strings which represents two huge numbers
bool compareHugeNumbers(const std::string& num1, const std::string& num2);
#endif