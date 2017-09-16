/*
 * AircraftLink.h
 *
 *  Created on: 22Aug.,2017
 *      Author: Dello
 */

#ifndef AIRCRAFTLINK_H_
#define AIRCRAFTLINK_H_

#include <Location.h>
#include <Aircraft.h>
#include <Waypoint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mavProxyLink.pb.h"

class AircraftLink {
    mav::AircraftLink aircraftLink;
    const std::string fileName_in = "res/protobuf/aircraftLink_medium_aircraft";
    const std::string fileName_out = "res/protobuf/aircraftLink_medium_waypoints";

public:
    AircraftLink();
    void read(Aircraft* aircraft);
    void send(std::vector<Waypoint> waypoints);
};

#endif /* AIRCRAFTLINK_H_ */
