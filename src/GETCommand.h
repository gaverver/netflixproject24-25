#ifndef GETCOMMAND_H
#define GETCOMMAND_H

#include <string>
#include <vector>
#include "ICommand.h"
#include "IDataBase.h"
#include "IMenu.h"

class GETCommand : public ICommand {
private:
    // reference to the database, to use the data
    IDataBase& db;
    // reference to the menu, to print via IMenu in the execute function
    IMenu& menu;
    
public:
    // constructor
    GETCommand(IDataBase& db, IMenu& menu);

    // methods inherited from Command class
    void execute(const std::vector<std::string>& args) override;
    std::string description() const override;
    std::string rw_status() const override;
};

#endif