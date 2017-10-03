/*
 * Ter.h
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#ifndef TER_H_
#define TER_H_

#include <Aircraft.h>
#include <Parameters.h>
#include <Scan.h>
#include <Convert.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "curl/curl.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>

class Ter {
    const std::string DIR_TER = "res/data/terrain/";
    const static int SRTM_SIZE = 1201;
    const static int GRAD_SIZE = SRTM_SIZE - 1;
    std::vector<std::vector<short> > height;
    Location locURL;
    Scan scan;

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    void downloadSRTM(const char* fileName);
    void processSRTM(const char* fileNameDir);
    void processGradient();

public:
    Ter();
    Scan getScan();
    void update(Scan scan);
    void process();
};

#endif /* TER_H_ */
