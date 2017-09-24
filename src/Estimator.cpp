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
    return true;
}

void Estimator::update(Aircraft aircraft, Parameters param) {
    this->aircraft = aircraft;
    this->param = param;
}

double Estimator::estimateRange() {
    //ToDo Estimate range equation
    return 100000;
}

