/*
 * Estimator.h
 *
 *  Created on: 11Jul.,2017
 *      Author: dello
 */

#ifndef ESTIMATOR_H_
#define ESTIMATOR_H_

#include <Aircraft.h>
#include <Parameters.h>

class Estimator {
    Aircraft aircraft;
    Parameters param;

public:
    bool sufficentThrust();
    bool sufficentRange(double range);
    void update(Aircraft aircraft, Parameters param);
    double estimateRange();

    Estimator();
};

#endif /* ESTIMATOR_H_ */
