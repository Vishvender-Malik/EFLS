/*
 * Site.h
 *
 *  Created on: 1Aug.,2017
 *      Author: dello
 */

#ifndef SITE_H_
#define SITE_H_

#include <Location.h>

struct Site {
    Location location;  //Middle of location
    double height;
    double width;
    double angle;       //Angle for plane to land (opposite to wind direction)
};



#endif /* SITE_H_ */
