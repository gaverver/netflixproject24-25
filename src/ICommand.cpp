#include <vector>
#include <string>

class ICommand {
    public:
    virtual void execute(const std::vector<std::string>& args) = 0;
    virtual std::string description() const = 0;
    virtual ~ICommand() = default;
};