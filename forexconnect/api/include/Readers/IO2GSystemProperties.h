#pragma once

/** System properties reader.*/
class IO2GSystemPropertiesReader : public IAddRef
{
 protected:
    IO2GSystemPropertiesReader(){};
 public:
    /** Gets property by the name.
        @return   0  in case the property not found.
    */
    virtual const char *findProperty(const char *propertyName) = 0;
    /** Gets property by the index.*/

    virtual const char *getProperty(int index, const char *&value) = 0;

    /** Gets a number of properties*/
    virtual int size() = 0;
};

