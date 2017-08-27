/*
 * Parameters.cpp
 *
 *  Created on: 5Jul.,2017
 *      Author: dello
 */

#include <Parameters.h>

Parameters::Parameters() {
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

//Setters
void Parameters::setRunway(double runway_length_new, double runway_width_new) {
    runway_length = runway_length_new;
    runway_width = runway_width_new;
}





