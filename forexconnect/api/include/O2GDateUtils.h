#pragma once

#if !defined(SYSTEMTIME_DEFINED) && !defined(WIN32)
typedef struct _SYSTEMTIME {
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
    unsigned short wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#define SYSTEMTIME_DEFINED
#endif

#ifdef WIN32
    #include <windows.h>
    #if !defined WINPHONE8
        #include <oleauto.h>
    #endif
#endif

class Order2Go2 CO2GDateUtils
{
public:
    static bool CTimeToOleTime(const struct tm *t, double *dt);
    static void CTimeToWindowsTime(const struct tm *t, SYSTEMTIME *st);
    static bool OleTimeToWindowsTime(const double dt, SYSTEMTIME *st);
    static bool OleTimeToCTime(const double dt, struct tm *t);
    static bool WindowsTimeToOleTime(const SYSTEMTIME *st, double *dt);
    static void WindowsTimeToCTime(const SYSTEMTIME *st, struct tm *t);
    
};

