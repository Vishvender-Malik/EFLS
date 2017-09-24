/*
 * Selection.h
 *
 *  Created on: 1Aug.,2017
 *      Author: dello
 */

#ifndef SELECTION_H_
#define SELECTION_H_

#include <Aircraft.h>
#include <Parameters.h>
#include <Scan.h>
#include <Sat.h>
#include <Map.h>
#include <Ter.h>
#include <Site.h>
#include <Waypoint.h>
#include <Convert.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <exception>

class Selection {
    Scan scan;
    Site site;
    std::vector<Waypoint> waypoint;
    cv::Mat scanner(cv::Mat input);
    cv::Mat rotateImage(cv::Mat input, double angle);
    cv::Mat unRotateImage(cv::Mat original, cv::Mat input, double angle);
    Matrix calcLandSite(cv::Mat input);
    cv::Mat removeMarker(cv::Mat input);
    void setWaypoints(Site site);
    static const int marker = 42;   //ToDo replace after removing marker method

public:
    Selection();
    Site getLandingSite();
    std::vector<Waypoint> getWaypoints();
    Scan getScan();
    void update(Scan satData, Scan mapData, Scan terData, Scan generic);
    void process();
};

struct NoSite : public std::exception {
    const char * what () const throw() {
        return "No landing site found, Exception";
    }
};

#endif /* SELECTION_H_ */
