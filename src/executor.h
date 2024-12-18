#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Runnable.h"
// executor interface
class executor {
public:
    // pure virtual function: execute - executes the given runnable
    virtual void execute(Runnable* r, bool clean) = 0;
    virtual ~executor() = default;
};

#endif