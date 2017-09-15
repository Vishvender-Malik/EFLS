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
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>


#include <cstdlib>  // for std::exit
#include <iostream> // for std::cout, std::cerr



// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>

// For the osmium::geom::haversine::distance() function
#include <osmium/geom/haversine.hpp>

// For osmium::apply()
#include <osmium/visitor.hpp>

// For the location index. There are different types of indexes available.
// This will work for small and medium sized input files.
#include <osmium/index/map/sparse_mem_array.hpp>

// For the NodeLocationForWays handler
#include <osmium/handler/node_locations_for_ways.hpp>

//Specific header for mapper
#include <stdio.h>      /* printf */
#include <math.h>       /* round, floor, ceil, trunc */
#include <stdlib.h>     /* abs */
//#include <test.hpp>

//#include <osmium/osm/node_ref.hpp>

// The type of index used. This must match the include file above
using index_type = osmium::index::map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;

// The location handler always depends on the index type
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

// This handler only implements the way() function, we are not interested in
// any other objects.

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
