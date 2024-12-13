#ifndef THREADFACTORY_H
#define THREADFACTORY_H


#include "executor.h"

// ThreadFactory interface
class ThreadFactory : public executor {
public:
    // function to execute the runnable object.
    void execute(Runnable& r) override;
};

#endif