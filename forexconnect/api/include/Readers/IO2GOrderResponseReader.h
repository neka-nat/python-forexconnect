#pragma once

class Order2Go2 IO2GOrderResponseReader : public IAddRef
{
 protected:
    IO2GOrderResponseReader();
 public:
    virtual const char* getOrderID() = 0;
    virtual bool isUnderDealerIntervention() = 0;
};

