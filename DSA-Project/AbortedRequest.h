#pragma once
#include "Requests.h"
class AbortRequest : public Requests {
public:
    AbortRequest(int Rd, int id, ReqType type)
        : Requests(Rd, id , type) {
    }

    // Implementation of the pure virtual function from the base class.
    virtual void Operate(MarsStation* station);
	virtual void print() const;
};

//inline ostream& operator<<(ostream& os, const AbortRequest* request) {
//    {
//        cout << "[ X";
//        cout << request->getRequestDay();
//        cout << "M" << request->getID() << " ] ,";
//
//        return os;
//    }
//}