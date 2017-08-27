/*
 * Aircraft.h
 *
 *  Created on: 4Jul.,2017
 *      Author: Dello
 */

#ifndef AIRCRAFT_H_
#define AIRCRAFT_H_

#include <Location.h>

class Aircraft {
    Location location;
    double bearing, speed, altitude, windSpeed, windDirection, motorCurrent, motorThrottle;
public:
    Location getLocation();
    double getBearing();
    double getSpeed();
    double getAltitude();
    double getWindSpeed();
    double getWindDirection();
    double getMotorCurrent();
    double getMotorThrottle();

    void refresh(Location location, double bearing,
            double speed, double altitude, double windSpeed,
            double windDirection, double motorCurrent, double motorThrottle);
    Aircraft();
};

#endif /* AIRCRAFT_H_ */

