/*
 * DecisionEngine.h
 *
 *  Created on: 12Jul.,2017
 *      Author: dello
 */

#ifndef DECISIONENGINE_H_
#define DECISIONENGINE_H_

#include <Aircraft.h>
#include <Parameters.h>
#include <Estimator.h>
#include <Scan.h>
#include <Sat.h>
#include <Map.h>
#include <Ter.h>
#include <Selection.h>
#include <Matrix.h>
#include <AircraftLink.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class DecisionEngine {
    void updateLite();
    void updateSubroutines();
    void process(Location origin, int level);
    Location processLargeSearch(Scan scan);

    Aircraft aircraft;
    Parameters param;
    AircraftLink aircraftLink;
    Estimator estimator;
    Sat sat;
    Map map;
    Ter ter;
    int level;

public:
    void run();
    DecisionEngine();
};

#endif /* DECISIONENGINE_H_ */
