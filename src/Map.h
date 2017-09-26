/*
 * Map.h
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#ifndef MAP_H_
#define MAP_H_

#include <Aircraft.h>
#include <Parameters.h>
#include <Scan.h>
#include <MapBuilder.h>
#include <FileWriter.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>

#include <math.h>
#include <cstdlib>
#include <osmium/io/any_input.hpp>
#include <osmium/geom/haversine.hpp>
#include <osmium/visitor.hpp>
#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
using index_type = osmium::index::map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;


class Map {
    Scan scan;
    void process();

public:
    Map();
    Scan getScan();
    void update(Scan scan);
};

// Custom handler for Libosmium
struct RoadLengthHandler : public osmium::handler::Handler {
    void way(const osmium::Way& way);
};


#endif /* MAP_H_ */
