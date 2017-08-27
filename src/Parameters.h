/*
 * Parameters.h
 *
 *  Created on: 5Jul.,2017
 *      Author: dello
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

class Parameters {
    //Scan parameters
    static const int chunk_size = 1000;  //Number of pixels in a chunk
    static const double chunk_meters = 1;    //Size of each pixel in meters for a chunk

    //Satellite scan parameters
    static const int satellite_sensitivity_init = 10;   //Sensitivity of scan, initial value
    int satellite_sensitivity;  //Sensitivity of scan

    //Map scan parameters
    static const int map_sensitivity_init = 10;   //Sensitivity of scan, initial value
    int map_sensitivity;  //Sensitivity of scan


    //Land site parameters
    static const double runway_length_init = 300;  //Length of runway in meters, initial value
    static const double runway_width_init = 100;     //Width of runway in meters, initial value
    double runway_length;   //Length of runway in meters
    double runway_width;    //Length of runway in meters

    //Aircraft parameters
    static const double glide_slope = 10;  //Meters forward for every meter of altitude lost

    //Aircraft landing approach parameters

    //EFLcheck parameters
    static const int throttle_threshold = 30;
    static const double current_min = 3.0;

    //Priority parameters
    static const double priority_range = 0.0001;   //Converts meters of range into a priority level


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

    void refresh();
    void setRunway(double runway_length_new, double runway_width_new);


    Parameters();
};

#endif /* PARAMETERS_H_ */
