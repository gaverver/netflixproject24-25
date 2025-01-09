#ifndef THREADFACTORY_H
#define THREADFACTORY_H


#include "executor.h"

// ThreadFactory class
class ThreadFactory : public executor {
public:
    // function to execute the runnable object.
    void execute(Runnable* r, bool clean) override;
};

#endif