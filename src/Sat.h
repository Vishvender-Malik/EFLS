/*
 * Sat.h
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#ifndef SAT_H_
#define SAT_H_

#include <Aircraft.h>
#include <Parameters.h>
#include <Scan.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "curl.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>

class Sat {
    cv::Mat cannyDetection(cv::Mat);
    cv::Mat lineExpansion(cv::Mat);
    cv::Mat getImage();
    cv::Mat fetchImage(Location originFetch);
    static size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata);
    cv::Mat curlImg();
    cv::Mat rawImage;

    Scan scan;

    static int const edgeThresh = 1;
    static int const lowThreshold = 70;
    static int const max_lowThreshold = 100;
    static int const ratio = 3;
    static int const kernel_size = 3;

public:
    Sat();
    Scan getScan();
    void update(Scan scan);
    void process();
    void save();
    void runBuddy();
    cv::Mat getRawImage();
};

#endif /* SAT_H_ */
