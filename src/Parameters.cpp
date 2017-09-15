/*
 * Parameters.cpp
 *
 *  Created on: 5Jul.,2017
 *      Author: dello
 */

#include <Parameters.h>

Parameters::Parameters() {
    //Scan parameters
        chunk_size = 1000;  //Number of pixels in a chunk
        chunk_meters = 1;    //Size of each pixel in meters for a chunk

        //Satellite scan parameters
        satellite_sensitivity_init = 10;   //Sensitivity of scan, initial value
        satellite_sensitivity;  //Sensitivity of scan

        //Map scan parameters
        map_sensitivity_init = 10;   //Sensitivity of scan, initial value
        map_sensitivity;  //Sensitivity of scan

        //Generic scan parameters
        resolution.x = 3000;
        resolution.y = 3000;


        //Land site parameters
        runway_length_init = 300;  //Length of runway in meters, initial value
        runway_width_init = 100;     //Width of runway in meters, initial value
        runway_length;   //Length of runway in meters
        runway_width;    //Length of runway in meters

        //Aircraft parameters
        glide_slope = 10;  //Meters forward for every meter of altitude lost

        //Aircraft landing approach parameters

        //EFLcheck parameters
        throttle_threshold = 30;
        current_min = 3.0;

        //Priority parameters
        priority_range = 0.0001;   //Converts meters of range into a priority level

    refresh();
}

//Resets parameters that may change during normal operation
void Parameters::refresh() {
    satellite_sensitivity = satellite_sensitivity_init;
    map_sensitivity = map_sensitivity_init;

    runway_length = runway_length_init;
    runway_width = runway_width_init;
}

//Getters
int Parameters::getChunk_size() {
    return chunk_size;
}

double Parameters::getChunk_meters() {
    return chunk_meters;
}

double Parameters::getRunway_length() {
    return runway_length;
}

double Parameters::getRunway_width() {
    return runway_width;
}

double Parameters::getGlide_slope() {
    return glide_slope;
}

int Parameters::getThrottle_threshold() {
    return throttle_threshold;
}

double Parameters::getCurrent_min() {
    return current_min;
}

double Parameters::getPriority_range() {
    return priority_range;
}

Matrix Parameters::getResolution() {
    return resolution;
}

//Setters
void Parameters::setRunway(double runway_length_new, double runway_width_new) {
    runway_length = runway_length_new;
    runway_width = runway_width_new;
}





