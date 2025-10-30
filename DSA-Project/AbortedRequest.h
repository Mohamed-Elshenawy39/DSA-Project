#pragma once
#include "Requests.h"
class AbortRequest : public Requests {
public:
    AbortRequest(int Rd, int id)
        : Requests(Rd, id) {
    }

    // Implementation of the pure virtual function from the base class.
    virtual void Operate(MarsStation* station);
};