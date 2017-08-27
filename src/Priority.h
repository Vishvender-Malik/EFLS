/*
 * Priority.h
 *
 *  Created on: 12Jul.,2017
 *      Author: dello
 */

#ifndef PRIORITY_H_
#define PRIORITY_H_

#include <Parameters.h>
#include <Aircraft.h>
#include <DecisionEngine.h>
#include <Estimator.h>

class Priority : public DecisionEngine {
    Aircraft aircraft;
    Parameters param;
    Estimator estimate;
    int iteration = 0;
    int level = 0;

public:
    void updateLevel(int range);
    int getLevel();
    void reset();
    Priority();
};

#endif /* PRIORITY_H_ */
