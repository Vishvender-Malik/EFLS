/*
 * Aircraft.cpp
 *
 *  Aircraft data is stored in this class. Getters are used to retrieve information.
 *
 *  Created on: 4Jul.,2017
 *      Author: Dello
 */
#include <Aircraft.h>
#include <iostream>
#include <fstream>
#include <string>
#include "mavProxyLink.pb.h"

Aircraft::Aircraft() {
    // ToDo Remove defined data, used only for testing purposes
    location.lat = -37;
    location.lon = 147;
    bearing = 0.0;
    speed = 0.0;
    altitude = 0.0;
    windSpeed = 0.0;
    windDirection = 0.0;
    motorCurrent = 0.0;
    motorThrottle = 0.0;
}

void Aircraft::refresh(Location location, double bearing,
        double speed, double altitude, double windSpeed,
        double windDirection, double motorCurrent, double motorThrottle) {
    this->location = location;
    this->bearing = bearing;
    this->speed = speed;
    this->altitude = altitude;
    this->windSpeed = windSpeed;
    this->windDirection = windDirection;
    this->motorCurrent = motorCurrent;
    this->motorThrottle = motorThrottle;
}

Location Aircraft::getLocation() {
    return location;
}

double Aircraft::getBearing() {
    return bearing;
}

double Aircraft::getSpeed() {
    return speed;
}

double Aircraft::getAltitude() {
    return altitude;
}

double Aircraft::getWindSpeed() {
    return windSpeed;
}

double Aircraft::getWindDirection(){
    return windDirection;
}

double Aircraft::getMotorCurrent() {
    return motorCurrent;
}

double Aircraft::getMotorThrottle() {
    return motorThrottle;
}




