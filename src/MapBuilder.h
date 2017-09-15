/*
 * MapBuilder.h
 *
 *  Created on: 11Sep.,2017
 *      Author: dello
 */

#ifndef MAPBUILDER_H_
#define MAPBUILDER_H_

#include <Location.h>
#include <Scan.h>
#include <Convert.h>
#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

class MapBuilder {
    MapBuilder();
    struct WayLocation {
        std::vector <Location> wayLocation;
    };
    std::vector <WayLocation> mapLocation;
    Scan scan;
    void drawLine(float x1, float y1, float x2, float y2);

public:
    static MapBuilder &sharedMapBuilder() {static MapBuilder mapBuilder; return mapBuilder;}
    void update(Scan scan);
    void addWay(std::vector <Location> wayLocation);
    void clear();
    void process();
    Scan getScan();
    Location getOrigin();
};



#endif /* MAPBUILDER_H_ */
