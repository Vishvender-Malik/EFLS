/*
 * Estimator.cpp
 *
 *  Created on: 11Jul.,2017
 *      Author: dello
 */

#include <Estimator.h>

Estimator::Estimator() {
}

bool Estimator::sufficentThrust() {
	return false; // To trick the sim, DO NOT USE!!! ToDo remove this
    if (aircraft.getMotorThrottle() > param.getThrottle_threshold() && aircraft.getMotorCurrent() < param.getCurrent_min()) {
        return false;
    }
    return true;
}

bool Estimator::sufficentRange(double rangeReq) {
    return (rangeReq < estimateRange());
}

void Estimator::update(Aircraft aircraft, Parameters param) {
    this->aircraft = aircraft;
    this->param = param;
}

double Estimator::estimateRange() {
    if (!sufficentThrust()) {
    	std::cout << "Alt: " << aircraft.getAltitude() << std::endl;
    	double range = aircraft.getAltitude() * param.glide_slope;
    	std::cout << "Range2: " << range << std::endl;
    	range -= param.estimated_process_time * aircraft.getSpeed() / param.glide_slope;
    	std::cout << "Range3: " << (range) << std::endl;
    	std::cout << "Range4: " << (range / param.glide_safetyFactor) << std::endl;
    	return range / param.glide_safetyFactor;
    }
    std::cout << "No limits" << std::endl;
    return 100000;
}

