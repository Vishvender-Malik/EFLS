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
        if (!estimator.sufficent() || true) {
            updateSubroutines();

            Scan scan;
            scan.param = param;
            scan.aircraft = aircraft;
            //scan.origin = aircraft.getLocation(); --This is set in Sat::getImage()
            scan.zoom = 16;
            scan.pixelPerMeter = Convert::pixelPerMeter(scan.zoom, scan.aircraft.getLocation().lat); //The latitude should be the center of the image, however the assumption that the aircraft is near the center of the map should be valid for all cases (I hope), otherwise call this method inside of the Sat::getImage() function.
            scan.sensitivity = 10;
            scan.processed = false;
            scan.scanType = generic;

            sat.update(scan);
            sat.save();
            Scan satData = sat.getScan();
            scan.origin = satData.origin; //Not great, but will work. Should create new method of managing cache.
            //sat.runBuddy();

            scan.data.create(satData.data.size(), satData.data.type());
            map.update(scan);
            Scan mapData = map.getScan();
            cv::imwrite("test10.bmp",mapData.data);

            cv::Mat rawCalFaded, rawCalImage;
            rawCalImage = cv::imread("satPic.bmp");
            cv::multiply(rawCalImage, cv::Scalar::all(1), rawCalFaded, 0.5);
            rawCalImage.copyTo(rawCalFaded, map.getScan().data);
            cv::imwrite("test13.bmp",rawCalFaded);

            Selection selection;
            selection.update(satData, mapData, satData, scan);

            //Send waypoints to AircraftLink
            aircraftLink.send(selection.getWaypoints());

            //Display results -- For testing only
            Scan temp = selection.getScan();
            cv::Mat rawFaded, rawImage;
            rawImage = sat.getRawImage();
            cv::multiply(sat.getRawImage(), cv::Scalar::all(1), rawFaded, 0.5);
            rawImage.copyTo(rawFaded, temp.data);
            cv::imwrite("test4.bmp",rawFaded);
            cv::imwrite("finalResults.bmp",selection.getScan().data);
            // Print waypoints
            std::vector<Waypoint> waypoint = selection.getWaypoints();
            for(int i=0; i < (int) waypoint.size(); i++) {
                printf("%d, %4.6f, %4.6f \n", i, waypoint.at(i).location.lat, waypoint.at(i).location.lon);
            }
            printf("plane, %4.6f, %4.6f \n", scan.aircraft.getLocation().lat, scan.aircraft.getLocation().lon);



            /*
            map.update(scan);
            Scan mapData = map.getScan();

            ter.update(scan);
            Scan terData = ter.getScan();

            Selection land;
            land.update(satData, mapData, terData, scan);
            Site landing_site = land.getLandingSite();
            Waypoint way = land.getWaypoints();


             */

        }
        run_state = false;
    }
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

