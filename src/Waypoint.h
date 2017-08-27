/*
 * Waypoint.h
 *
 *  Created on: 1Aug.,2017
 *      Author: dello
 */

#ifndef WAYPOINT_H_
#define WAYPOINT_H_

#include <Location.h>

struct Waypoint {
    double angle;
    double altitude;
    Location location;
};



#endif /* WAYPOINT_H_ */
