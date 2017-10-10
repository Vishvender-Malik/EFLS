/*
 * AircraftLink.cpp
 *
 *  Created on: 22Aug.,2017
 *      Author: Dello
 *
 *  Allows aircraft data to be read and EFLS to send
 *  waypoint information to the UAV flight controller.
 */

#include <AircraftLink.h>

AircraftLink::AircraftLink() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

// Read aircraft data from ProtoBuf
void AircraftLink::read(Aircraft* aircraft) {
    //Connect to the ProtoBuf file
    std::fstream input(fileName_in.c_str(), std::ios::in | std::ios::binary);
    if (!aircraftLink.ParseFromIstream(&input)) {
        std::cerr << "Failed to connect to ProtoBuf file" << std::endl;
        return;
    }

    //Select the most recent Aircraft data
    const mav::Aircraft& data = aircraftLink.aircrafts(aircraftLink.aircrafts_size()-1);
    Location loc;
    loc.lat = data.lat();
    loc.lon = data.lon();
    aircraft->refresh(loc,
            data.bearing(),
            data.speed(),
            data.altitude(),
            data.wind_speed(),
            data.wind_direction(),
            data.motor_current(),
            data.motor_throttle());
}

// Sends waypoint information using ProtoBug. Previous waypoint data is overwritten
void AircraftLink::send(std::vector<Waypoint> waypoints) {
    //Add the waypoints to ProtoBuf
    mav::Waypoints* data = aircraftLink.add_waypoints();
    for (int i=0; i<waypoints.size(); i++) {
        mav::Waypoint* way = data->add_waypoint();
        way->set_lat(waypoints.at(i).location.lat);
        way->set_lon(waypoints.at(i).location.lon);
        way->set_altitude(waypoints.at(i).altitude);
        way->set_speed(0.0);
        way->set_type(1);
    }

    //Write to file
    std::fstream output(fileName_out.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
    if (!aircraftLink.SerializeToOstream(&output)) {
        std::cerr << "Failed to write to ProtoBuf file." << std::endl;
        return;
    }
    output.flush();
}

