/*
 * Convert.cpp
 *
 *  Static library of useful math functions
 *
 *  Created on: 4Aug.,2017
 *      Author: Dello
 */

#include <Convert.h>

Convert::Convert() {
}



//Normalise to [-180,180)
double Convert::wrapTo180(double angle) {
    angle = fmod(angle + 180, 360);
    if (angle < 0) {
        angle += 360;
    }
    return angle - 180;
}

double Convert::rad2Deg(double angle) {
    return (angle*180)/M_PI;
}

double Convert::deg2Rad(double angle) {
    return (angle*M_PI)/180;
}

Location Convert::rad2Deg(Location location) {
    location.lat = rad2Deg(location.lat);
    location.lon = rad2Deg(location.lon);
    return location;
}

Location Convert::deg2Rad(Location location) {
    location.lat = deg2Rad(location.lat);
    location.lon = deg2Rad(location.lon);
    return location;
}

// Valid method to calculate pixels per meter for Google Satellite images and OSM map data
double Convert::pixelPerMeter(int zoom, double lat) {
    lat = deg2Rad(lat);
    double pixelPerMeter = (256*pow(2,zoom))/(2*M_PI*6378137*cos(lat));
    return pixelPerMeter;
}

double Convert::coordinate2Bearing(Location location1, Location location2) {
    location1 = deg2Rad(location1);
    location2 = deg2Rad(location2);
    double y = sin(location2.lon-location1.lon) * cos(location2.lat);
    double x = cos(location1.lat)*sin(location2.lat) - sin(location1.lat)*cos(location2.lat)*cos(location2.lon-location1.lon);
    return rad2Deg(atan2(y,x));
}

// Rhumb line for coordinate projection
// Based on code from http://www.movable-type.co.uk/scripts/latlong.html
Location Convert::coordinateProjection(Location location1, double distance, double angle){
    Location location2, delta;
    location1 = deg2Rad(location1);
    double dGamma, q, angularDistance;
    angle = deg2Rad(angle);

    angularDistance = distance/re;
    delta.lat = angularDistance * cos(angle);
    location2.lat = location1.lat + delta.lat;

    if (abs(location2.lat) > M_PI/2) {
        if (location2.lat > 0) {
            location2.lat = M_PI-location2.lat;
        }
        else {
            location2.lat = -1*(M_PI-location2.lat);
        }
    }

    dGamma = log(tan(location2.lat/2 + M_PI/4) / tan(location1.lat/2+M_PI/4));
    if (fabs(dGamma) > (pow(10,-12))) {
        q = delta.lat / dGamma;
    }
    else {
        q = cos(location1.lat);
    }

    delta.lon = angularDistance * sin(angle)/q;
    location2.lon = location1.lon + delta.lon;

    location2 = rad2Deg(location2);
    location2.lon = wrapTo180(location2.lon);
    return location2;
}

// Based on code from http://www.movable-type.co.uk/scripts/latlong.html
double Convert::haversine(Location location1, Location location2) {
    location1 = deg2Rad(location1);
    location2 = deg2Rad(location2);
    double distance = (2*asin(sqrt(pow((sin((location2.lat-location1.lat)/2)),2) + cos(location1.lat)*cos(location2.lat)*pow(sin((location2.lon-location1.lon)/2),2))) * re);
    return distance;
}

// Note:: Origin is from image center
Location Convert::matrix2Coordinate(Location origin, Matrix location, double pixelPerMeter) {
    //Convert pixels to meters
    double xMeters = location.x / pixelPerMeter;
    double yMeters = location.y / pixelPerMeter;

    //Calculate bearing and distance
    double distance = sqrt(pow(xMeters,2) + pow(yMeters,2));
    double bearing = 90-rad2Deg(atan2(yMeters,xMeters));

    //Calculate the required location
    return coordinateProjection(origin, distance, bearing);
}
