/*
 * Convert.h
 *
 *  Created on: 4Aug.,2017
 *      Author: dello
 */

#ifndef CONVERT_H_
#define CONVERT_H_

#include <Aircraft.h>
#include <Parameters.h>
#include <Scan.h>
#include <Sat.h>
#include <Map.h>
#include <Ter.h>
#include <Site.h>
#include <Waypoint.h>
#include <math.h>

class Convert {
public:
    Convert();
    static double haversine(Location location1, Location location2);
    static double coordinate2Bearing(Location location1, Location location2);
    static Location coordinateProjection(Location origin, double distance, double angle);
    static Location matrix2Coordinate(Location origin, Matrix dimension, double pixelPerMeter);

    static double wrapTo180(double angle);
    static double deg2Rad(double angle);
    static double rad2Deg(double angle);
    static Location deg2Rad(Location location);
    static Location rad2Deg(Location location);
    static double pixelPerMeter(int zoom, double lat);
    static const double re = 6371000; //Earth radius in meters
};

#endif /* CONVERT_H_ */
