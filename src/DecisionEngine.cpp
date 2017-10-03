/*
 * DecisionEngine.cpp
 *
 *  Created on: 12Jul.,2017
 *      Author: dello
 */

#include <DecisionEngine.h>

DecisionEngine::DecisionEngine() {
}

// Main loop
void DecisionEngine::run() {
    bool run_state = true;      // Runs decision engine continuously
    while (run_state) {
        updateLite();
        int counter = 0;
        if (!estimator.sufficentThrust() || true) {
            std::cout << "DecisionEngine: Emergency Forced Landing is required" << std::endl;

            counter++;
            std::stringstream variablePath;
            variablePath << "/home/dello/EFLS/build/results/temp/test" << counter << "/";
            FileWriter::setCwdVariable(variablePath.str().c_str());

            Location origin;
            bool foundSite = false;
            level = 11;
            while (!foundSite) {
                FileWriter time1("Search");
                origin = aircraft.getLocation();
                std::cout << std::endl;

                // Last resort landing --> No safe landing sites found, so just land
                if (!estimator.sufficentRange(param.levelState_rangeImmediate) || level == 1) {
                    std::cout << "DecisionEngine: Last resort landing engaged" << std::endl;
                    level = 1;
                    // ToDo Set waypoint for land infront of uav
                    foundSite = true;
                }

                // Last search for a landing site
                else if (level == 2) {
                    std::cout << "DecisionEngine: Last search for a safe landing site" << std::endl;
                    level = 3;
                    try {
                        FileWriter::write("\nLast Search");
                        process(origin, level);
                        foundSite = true;
                    } catch (NoSite& e) {
                        std::cout << "DecisionEngine: Failed to find site" << std::endl;
                        level = 1;
                    }
                }

                // Second last search for a landing site
                else if (!estimator.sufficentRange(param.levelState_rangeHighPriority) || level == 3) {
                    std::cout << "DecisionEngine: Second last search for a safe landing site" << std::endl;
                    level = 3;
                    try {
                        FileWriter::write("\nSecond Last Search");
                        process(origin, level);
                        foundSite = true;
                    } catch (NoSite& e) {
                        std::cout << "DecisionEngine: Failed to find site" << std::endl;
                        level = 2;
                    }
                }

                //Large search area --> finds a landing site a far distance away
                else if (estimator.sufficentRange(param.largeSearch_minRange) && level == 11) {
                    try {
                        FileWriter::write("\nLarge Area Search");
                        std::cout << "DecisionEngine: Landing location search from far zoom level" << std::endl;

                        Parameters paramTemp = param;
                        paramTemp.runway_length = param.largeSearch_runwayLength;
                        paramTemp.runway_width = param.largeSearch_runwayWidth;
                        Scan scan;
                        scan.param = paramTemp;
                        scan.aircraft = aircraft;
                        scan.origin = origin;
                        scan.zoom = param.largeSearch_zoom;
                        scan.pixelPerMeter = Convert::pixelPerMeter(scan.zoom, scan.aircraft.getLocation().lat);
                        scan.level = level;
                        scan.processed = false;
                        scan.scanType = generic;
                        Location locSite = processLargeSearch(scan);

                        std::cout << "DecisionEngine: Found a valid landing location, now finding a specific site" << std::endl;

                        counter++;
                        std::stringstream variablePath;
                        variablePath << "/home/dello/EFLS/build/results/temp/test" << counter << "/";
                        FileWriter::setCwdVariable(variablePath.str().c_str());

                        FileWriter::write("\nSpecific Search");
                        process(locSite, level);
                        foundSite = true;
                    } catch (NoSite& e) {
                        std::cout << "DecisionEngine: Failed to find landing location or site" << std::endl;
                        level = 10;
                    }
                }

                else {
                    // Search at the origin for a landing site
                    try {
                        FileWriter::write("\nOrigin Search");
                        std::cout << "DecisionEngine: Landing site search started, around the aircraft" << std::endl;
                        process(origin, level);
                        foundSite = true;
                    } catch (NoSite& e) {
                        std::cout << "DecisionEngine: Failed to find site" << std::endl;

                        // Select random location within region
                        double maxDistance, randomDistance, randomAngle;
                        srand(time(NULL));
                        for (int i=0; i<param.randomLoc_attempts; i++) {
                            if (estimator.sufficentRange(param.randomLoc_minRange)) {
                                std::cout << "DecisionEngine: Landing site search started, Lat: " << origin.lat << "Lon: " << origin.lon << std::endl;

                                counter++;
                                std::stringstream variablePath;
                                variablePath << "/home/dello/EFLS/build/results/temp/test" << counter << "/";
                                FileWriter::setCwdVariable(variablePath.str().c_str());

                                try {
                                    FileWriter::write("\nRandom Search");
                                    maxDistance = param.randomLoc_maxDistance;
                                    if (estimator.estimateRange() < param.randomLoc_maxDistance) {
                                        maxDistance = estimator.estimateRange();
                                    }
                                    randomDistance = rand() % (int) round((maxDistance - param.randomLoc_minDistance) + param.randomLoc_minDistance);
                                    randomAngle = rand() % 360;
                                    origin = Convert::coordinateProjection(aircraft.getLocation(), randomDistance, randomAngle);

                                    process(origin, level);
                                    foundSite = true;
                                    break;
                                } catch (NoSite& e) {
                                    std::cout << "DecisionEngine: Failed to find site" << std::endl;
                                }
                            }
                        }
                    }
                    if (!foundSite) {
                        level -= 1;
                        std::cout << "DecisionEngine: Changed land threshold to: " << level << std::endl;
                    }
                }
                time1.end();
            }
        }
        run_state = false;
    }
    std::cout << "DecisionEngine: Closing" << std::endl;
}

//Obtains new values and updates estimator
void DecisionEngine::updateLite() {
    aircraftLink.read(&aircraft);
    param.refresh();
    estimator.update(aircraft, param);
}

//Sets parameters and aircraft values for subroutines
//ToDo consider to replace this method with pointers
void DecisionEngine::updateSubroutines() {
    //
}

Location DecisionEngine::processLargeSearch(Scan scan) {
    updateSubroutines();

    sat.update(scan);
    sat.save();
    Scan satData = sat.getScan();

    scan.data.create(satData.data.size(), satData.data.type());
    map.update(scan);
    Scan mapData = map.getScan();

    scan.data.create(satData.data.size(), satData.data.type());
    ter.update(scan);
    Scan terData = ter.getScan();

    Selection selection;
    selection.update(satData, mapData, terData, scan);

    return selection.getLandingSite().location;
}

void DecisionEngine::process(Location origin, int level) {
    updateSubroutines();

    Scan scan;
    scan.param = param;
    scan.aircraft = aircraft;
    scan.origin = origin;
    scan.zoom = 16;
    scan.pixelPerMeter = Convert::pixelPerMeter(scan.zoom, scan.aircraft.getLocation().lat); //The latitude should be the center of the image, however the assumption that the aircraft is near the center of the map should be valid for all cases (I hope), otherwise call this method inside of the Sat::getImage() function.
    scan.level = level;
    scan.processed = false;
    scan.scanType = generic;

    sat.update(scan);
    sat.save();
    Scan satData = sat.getScan();

    Scan mapData;
    if (scan.level >= 3) {
        scan.data.create(satData.data.size(), satData.data.type());
        map.update(scan);
        mapData = map.getScan();
    }
    else {
        mapData = satData;
    }


    /*
    cv::Mat rawCalFaded, rawCalImage;
    rawCalImage = cv::imread("satPic.bmp");
    cv::multiply(rawCalImage, cv::Scalar::all(1), rawCalFaded, 0.5);
    rawCalImage.copyTo(rawCalFaded, map.getScan().data);
    cv::imwrite("test13.bmp",rawCalFaded);

    Scan temp = selection.getScan();
    cv::Mat rawFaded, rawImage;
    rawImage = sat.getRawImage();
    cv::multiply(sat.getRawImage(), cv::Scalar::all(1), rawFaded, 0.5);
    rawImage.copyTo(rawFaded, temp.data);
    cv::imwrite("test4.bmp",rawFaded);
    cv::imwrite("finalResults.bmp",selection.getScan().data);
    */

    Scan terData;
    if (scan.level > 7) {
        ter.update(scan);
        terData = ter.getScan();
    }
    else {
        terData = satData;
    }

    Selection selection;
    selection.update(satData, mapData, terData, scan);

    //Send waypoints to AircraftLink
    aircraftLink.send(selection.getWaypoints());

    //Display results
        cv::Mat rawCalFaded, rawCalImage;
    rawCalImage = cv::imread(FileWriter::cwdVariable("satelliteImage.bmp"));
    cv::multiply(rawCalImage, cv::Scalar::all(1), rawCalFaded, 0.5);
    rawCalImage.copyTo(rawCalFaded, selection.getScan().data);
    cv::imwrite(FileWriter::cwdVariable("landingSiteDisplayed.bmp"),rawCalFaded);

    // Print waypoints
    std::cout << "DecisionEngine: Displaying waypoints to safe landing site" << std::endl;
    std::vector<Waypoint> waypoint = selection.getWaypoints();
    for(int i=0; i < (int) waypoint.size(); i++) {
        printf("%d, %4.6f, %4.6f \n", i, waypoint.at(i).location.lat, waypoint.at(i).location.lon);
    }
    printf("plane, %4.6f, %4.6f \n", scan.aircraft.getLocation().lat, scan.aircraft.getLocation().lon);
}

