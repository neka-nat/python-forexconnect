#pragma once
/** Converts a date and time from one time zone to another.*/
class IO2GTimeConverter : public IAddRef
{
 protected:
    IO2GTimeConverter();
 public:
    typedef enum
    {
       UTC,
       Local,
       EST,
       Server
    } TimeZone;

    /** Converts the date and time from/to the UTC, EST, local and server time zones..
        @param dtSource         The date and time to convert.
        @param fromZone         The time zone to convert from..
        @param fromZone         The time zone to convert to.
        @return                 The date and time in the time zone which you select in the OutputZone parameter..
    */
    virtual DATE convert(DATE dtSource, TimeZone fromZone, TimeZone toZone) = 0;
};

