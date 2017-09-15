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

        //EFLcheck parameters
        int throttle_threshold;
        double current_min;

        //Priority parameters
        double priority_range;   //Converts meters of range into a priority level


public:

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
