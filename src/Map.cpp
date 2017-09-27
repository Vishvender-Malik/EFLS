/*
 * Map.cpp
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#include <Map.h>

Map::Map() {

}

Scan Map::getScan() {
    return scan;
}

void Map::update(Scan scan) {
    this->scan = scan;
    this->scan.scanType = map;
    MapBuilder::sharedMapBuilder().update(this->scan);
    process();
}

void Map::process() {
    std::cout << "Map: Started" << std::endl;

    // Use Libosmium to read map data
    FileWriter time1("mapRead");
    std::cout << "Map: Reading OSM data" << std::endl;
    osmium::io::Reader reader{"res/data/map/australia.pbf", osmium::osm_entity_bits::node | osmium::osm_entity_bits::way};
    index_type index;
    location_handler_type location_handler{index};
    RoadLengthHandler road_length_handler;
    osmium::apply(reader, location_handler, road_length_handler);
    time1.end();

    // Retrieve the processed map data
    FileWriter time2("mapProcess");
    std::cout << "Map: Processing OSM data" << std::endl;
    MapBuilder::sharedMapBuilder().process();
    scan = MapBuilder::sharedMapBuilder().getScan();
    scan.data = Convert::lineExpansion(scan.data);
    time2.end();

    cv::imwrite(FileWriter::cwdVariable("Map.bmp"), scan.data);
}

// Libosmium custom handler
void RoadLengthHandler::way(const osmium::Way& way) {
    std::vector<Location> wayLocation;
    bool stop = false;
    Location origin = MapBuilder::sharedMapBuilder().getOrigin();

    for (const osmium::NodeRef& nr : way.nodes()) {
        const osmium::geom::Coordinates nodeLocation = nr.location();
        Location locTemp;
        locTemp.lat = nodeLocation.y;
        locTemp.lon = nodeLocation.x;
        // Rejects nodes not near the origin location very quickly
        if (fabs(nodeLocation.y-origin.lat) > 0.5|| fabs(nodeLocation.x-origin.lon) > 0.5) {
            stop = true;
            break;
        }
        wayLocation.push_back(locTemp);
    }
    if (!stop) {
        MapBuilder::sharedMapBuilder().addWay(wayLocation);
    }
}






