/*
 * Scan.h
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#ifndef SCAN_H_
#define SCAN_H_

#include <Parameters.h>
#include <Aircraft.h>
#include <Location.h>
#include <Matrix.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

enum method {sat, map, ter, selection, generic};

struct Scan {
    method scanType;
    bool processed;

    Aircraft aircraft;
    Parameters param;

    Location origin;        //Center point
    int zoom;
    double pixelPerMeter;

    int level;

    cv::Mat data;
};



#endif /* SCAN_H_ */
