#ifndef PUBLICFUNCTIONS_H
#define PUBLICFUNCTIONS_H

#include <string>
#include "ICommand.h"
// helper function to convert from string to unsigned long int and know if it failed
unsigned long int fromStringToULI(const std::string& str, bool& isValid);
std::string captureCommandOutput(ICommand* Runner, const std::vector<std::string>& args);
#endif