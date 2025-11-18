#include "AbortedRequest.h"

void AbortRequest::Operate(MarsStation* station)
{
}

void AbortRequest::print() const
{
    cout << "[X,";
    cout << getRequestDay();
    cout << "M" << getID() << "]";

    
}