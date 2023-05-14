/************************************************************************
 *  Copyright (c) 2003, Arabeyes, Thamer Mahmoud
 *
 *  A full featured Muslim Prayer Times calculator
 *
 *  NOTE: This is a small example that uses the prayer.h. For more info on
 *  filling the structures and on supported methods read "prayer.h"
 *
 * (www.arabeyes.org - under LGPL license - see COPYING file)
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "prayer.h"

static void displayTimes(double lat, double lon, char  *cityName, int day, int month, int year, int gmtDiff, int dst, int method);

int main()
{
    printf("Karachi Hanafi\n");
    displayTimes(41.846625, -88.3111402, "Batavia", 13, 5, 2023, -5, 0, 3);


    printf("\n\nNorth America\n");

    displayTimes(41.846625, -88.3111402, "Batavia", 13, 5, 2023, -5, 0, 4);

    return 0;
}

static void displayTimes(double lat, double lon, char *cityName, int day, int month, int year, int gmtDiff, int dst, int method)
{
    int i, deg, min;
    double sec;
    const char symb = (char)0x00B0;
    double qibla;

    Location loc;
    Method conf;
    Date date;

    Prayer ptList[6];
    Prayer imsaak;
    Prayer nextImsaak;
    Prayer nextFajr;

    /* fill the Date structure */
    date.day = day;
    date.month = month;
    date.year = year;
    /* fill the location info. structure */
    loc.degreeLat = lat;
    loc.degreeLong = lon;
    loc.gmtDiff = gmtDiff;
    loc.dst = dst;
    loc.seaLevel = 0;
    loc.pressure = 1010;
    loc.temperature= 10;


    /* auto fill the method structure. Have a look at prayer.h for a
     * list of supported methods */
    getMethod(method, &conf);
    conf.round = 0;

    /* Call the main function to fill the Prayer times array of
     * structures */
    getPrayerTimes (&loc, &conf, &date, ptList);

    /* Call functions for other prayer times and qibla */
    getImsaak (&loc, &conf, &date, &imsaak);
    getNextDayFajr (&loc, &conf, &date, &nextFajr);
    getNextDayImsaak (&loc, &conf, &date, &nextImsaak);
    qibla = getNorthQibla(&loc);


    /* Show the results */
    printf ("\nPrayer schedule for: %s on %2d/%2d/%4d\n", cityName,
        date.day, date.month, date.year);

    decimal2Dms (loc.degreeLat, &deg, &min, &sec);
    printf("\nLatitude\t=  %d%c %2d\' %4.1f\" %c", abs (deg), symb, abs (min),
       fabs (sec), (loc.degreeLat>=0) ? 'N' : 'S');

    decimal2Dms (loc.degreeLong, &deg, &min, &sec);
    printf ("\nLongitude\t=  %d%c %d\' %4.1f\" %c", abs (deg), symb, abs (min),
        fabs (sec), (loc.degreeLong>=0) ? 'E' : 'W');

    decimal2Dms (qibla, &deg, &min, &sec);
    printf("\nQibla\t\t=  %d%c %d\' %4.1f\" %c of true North\n", abs (deg), symb,
       abs (min), fabs (sec), (qibla>=0) ? 'W' : 'E');

    printf ("\n\n");

    for (i = 0; i < 6; i++)
    printf (" %3d:%02d:%02d%c", ptList[i].hour, ptList[i].minute, ptList[i].second,
        (ptList[i].isExtreme) ? '*' : ' ' );

    printf ("\n\n");
    printf("Tomorrow's Fajr:\t%3d:%02d\n", nextFajr.hour, nextFajr.minute);
    printf("Tomorrow's Imsaak:\t%3d:%02d\n", nextImsaak.hour, nextImsaak.minute);
    printf("Today's Imsaak:\t\t%3d:%02d\n\n", imsaak.hour, imsaak.minute);   
}
