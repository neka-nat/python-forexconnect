#pragma once

class Order2Go2 IO2GLastOrderUpdateResponseReader : public IAddRef
{
 protected:
    IO2GLastOrderUpdateResponseReader();
 public:
    /** Get order update type. */
    virtual O2GTableUpdateType getUpdateType() = 0;
    /** Get order row. */
    virtual IO2GOrderRow *getOrder() = 0;
};
