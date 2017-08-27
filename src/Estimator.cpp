/*
 * Estimator.cpp
 *
 *  Created on: 11Jul.,2017
 *      Author: dello
 */

#include <Estimator.h>

Estimator::Estimator() {
}

bool Estimator::sufficent() {
    if (aircraft.getMotorThrottle() > param.getThrottle_threshold() && aircraft.getMotorCurrent() < param.getCurrent_min()) {
        return false;
    }
    return true;
}

int Estimator::range() {
    if (sufficent()) {
        return -1;
    }
    //TODO function for range calculation
    return 100;
}

void Estimator::update(Aircraft aircraft, Parameters param) {
    this->aircraft = aircraft;
    this->param = param;
}

