#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include <vector>

// ICommand interface
class ICommand {
public:
    // Pure virtual function to execute the command
    virtual void execute(const std::vector<std::string>& args) = 0;

    // Pure virtual function to provide a description of the command
    virtual std::string description() const = 0;

    // Virtual destructor to ensure proper cleanup in derived classes
    virtual ~ICommand() = default;
};

#endif // ICOMMAND_H
