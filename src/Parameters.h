/*
 * Parameters.h
 *
 *  Created on: 5Jul.,2017
 *      Author: dello
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <Matrix.h>

class Parameters {
    //Todo Make a better system for sharing parameters


public:
    //Scan parameters
            int chunk_size;  //Number of pixels in a chunk
            double chunk_meters;    //Size of each pixel in meters for a chunk

            //Satellite scan parameters
            int satellite_sensitivity_init;   //Sensitivity of scan, initial value
            int satellite_sensitivity;  //Sensitivity of scan

            //Map scan parameters
            int map_sensitivity_init;   //Sensitivity of scan, initial value
            int map_sensitivity;  //Sensitivity of scan

            //Generic scan parameters
            Matrix resolution;

            //Land site parameters
            double runway_length_init;  //Length of runway in meters, initial value
            double runway_width_init;     //Width of runway in meters, initial value
            double runway_length;   //Length of runway in meters
            double runway_width;    //Length of runway in meters

            //Aircraft parameters
            double glide_slope;  //Meters forward for every meter of altitude lost

            //Aircraft landing approach parameters
            double landFinalApproachDistance;
            double landFinalApproachAltitude;
            double landEndTurnDistance;
            double landEndTurnAltitude;

            double landArcViewTurn;                 // The total viewing angle that a waypoint can be accepted within. If a waypoint is not seen, 1/2 of this value is used for the angle of the next waypoint to be plotted at.
            double landArcViewFinal;                // The total viewing angle that a waypoint can be accepted within, if its on final approach.
            double landArcDistance;                 // Distance between consecutive waypoint during the turn
            double landArcDistanceFinal;            // Distance between first and second turn waypoints
            double landArcAcceptDistance;           // Not required with landing algorithm
            double landArcPartialAcceptDistance;    // May not be required with landing algorithm
            double landArcPartialAcceptAngle;       // May not be required with landing algorithm
            double landArcRateOfDescent;            // Rate of descent of the aircraft during approach
            int landArcMinWaypoint;                 // Minimum number of turn waypoints to plot (total number of waypoints = landArcMinWaypoint + 3)


            //EFLcheck parameters
            int throttle_threshold;
            double current_min;

            //Priority parameters
            double priority_range;   //Converts meters of range into a priority level

    //Getters
    int getChunk_size();
    double getChunk_meters();
    double getRunway_length();
    double getRunway_width();
    double getGlide_slope();
    int getThrottle_threshold();
    double getCurrent_min();
    double getPriority_range();
    Matrix getResolution();

    void refresh();
    void setRunway(double runway_length_new, double runway_width_new);


    Parameters();
};

#endif /* PARAMETERS_H_ */
