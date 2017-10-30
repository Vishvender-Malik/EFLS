/*
 * MapBuilder.cpp
 *
 *  Created on: 11Sep.,2017
 *      Author: dello
 *
 *  Map builder, is used to work wiht the Libosmiumn custom handler to save the node locations for ease of plotting with the line algorithm
 */

#include <MapBuilder.h>

MapBuilder::MapBuilder() {

}

void MapBuilder::update(Scan input) {
    scan = input;
    //Warning >> The scan.data needs to be initlised to the same size and type as per the Sat output
    clear();
}

// Add a way to the location vector
void MapBuilder::addWay(std::vector <Location> location) {
    WayLocation temp;
    temp.wayLocation = location;
    mapLocation.push_back(temp);
}

// Clear the binary array
void MapBuilder::clear() {
    MapBuilder::scan.processed = false;
    scan.data = cv::Scalar::all(255);
    mapLocation = std::vector<WayLocation>();
}

Scan MapBuilder::getScan() {
    return scan;
}

// Bresenham's line algorithm to plot the ways
void MapBuilder::drawLine(float x1, float y1, float x2, float y2){
    bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep){
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    float dx = x2 - x1;
    float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++){
        if(steep){
            if (x<scan.param.getResolution().y && x>0 && y<scan.param.getResolution().x && y>0){
                scan.data.at<uchar>(scan.param.getResolution().y-x, scan.param.getResolution().x-y) = 0;
            }
        }
        else{
            if (x<scan.param.getResolution().x && x>0 && y<scan.param.getResolution().y && y>0){
                scan.data.at<uchar>(scan.param.getResolution().y-y, scan.param.getResolution().x-x) = 0;
            }
        }

        error -= dy;
        if(error < 0){
            y += ystep;
            error += dx;
        }
    }
}

// Process all of the ways stored in the location vector
void MapBuilder::process() {
    int preLoc_x, preLoc_y;
    Matrix current, pre;
    for (int i=0; i<mapLocation.size(); i++) {
        bool first_run = true;
        WayLocation currentMapLocation = mapLocation.at(i);
        for (int j=0; j<currentMapLocation.wayLocation.size(); j++) {
            current = Convert::coordinate2Matrix(scan.origin, currentMapLocation.wayLocation.at(j), scan.param.getResolution(), scan.pixelPerMeter);
            if (!first_run) {
                if (current.x != -1) {
                    drawLine(current.x, current.y, pre.x, pre.y);
                    pre = current;
                }
            }
            else if (current.x != -1){
                pre = current;
                first_run = false;
            }
        }
    }
    scan.processed = true;
}

Location MapBuilder::getOrigin() {
    return scan.origin;
}

