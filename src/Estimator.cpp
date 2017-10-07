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
    	return aircraft.getAltitude() * param.glide_slope;
    }
    return 100000;
}

