/*
 * Selection.cpp
 *
 *  Processes the scan results to generate a landing site and waypoints for landing.
 *
 *  Created on: 1Aug.,2017
 *      Author: dello
 */

#include <Selection.h>

Selection::Selection() {
}

Site Selection::getLandingSite() {
    return site;
}

std::vector<Waypoint> Selection::getWaypoints() {
    return waypoint;
}

Scan Selection::getScan() {
    return scan;
}

// Fuses the binary arrays together
void Selection::update(Scan satData, Scan mapData, Scan terData, Scan generic) {
    scan = generic;
    scan.data = (cv::Scalar::all(255)-satData.data);
    scan.data += (cv::Scalar::all(255)-mapData.data);
    scan.data += (cv::Scalar::all(255)-terData.data);
    scan.data = cv::Scalar::all(255) - scan.data;
    //scan.data = -(-satData.data - mapData.data - terData.data);
    cv::imwrite(FileWriter::cwdVariable("selectionRaw.bmp"),scan.data);
    process();
}

// Finds a valid site for landing
cv::Mat Selection::scanner(cv::Mat input) {
    std::cout << "Selection: Scanning for a landing site" << std::endl;

    int dimension_length, dimension_width, dimension_skip;
    int dimension_length_previous, dimension_width_previous;
    bool break_search = false;
    bool break_loop = false;
    bool duplicates = false;
    bool first_run = true;
    cv::Size sz = input.size();
    cv::Mat valid;
    valid.create(sz, input.type());
    valid = cv::Scalar::all(0);         //Bug fixed -- When Mat is created it is not always initialised with all 0 elements, creating fake results. Setting Mat to all 0 fixes this issue.

    int position_length_save = 0;
    int position_width_save = 0;
    double multiplier = 1.5;
    int count = 0;
    int fullLoopCount = 0;
    int maxNumberLoop = 5;

    dimension_length = (int) round(scan.param.getRunway_length() * scan.pixelPerMeter);
    dimension_width = (int) round(scan.param.getRunway_width() * scan.pixelPerMeter);
    dimension_skip = (int) round(dimension_width/3);

    while (!break_search) {
        // Search algorithm
        fullLoopCount++;
        for (int i = dimension_length; i < sz.height - dimension_length; i += dimension_skip) {
            for (int j = dimension_width; j < sz.width - dimension_width; j += dimension_skip) {
                break_loop = false;
                for (int k = 0; k < dimension_length; k++) {
                    for (int l = 0; l < dimension_width; l++) {
                        if (input.at<uchar>(i+k, j+l) == 0) {
                            break_loop = true;
                            break;
                        }
                    }
                    if (break_loop) {
                        break;
                    }
                }
                if (!break_loop) {
                    count++;
                    position_length_save = i;
                    position_width_save = j;
                }
            }
        }
        if (count == 0 && first_run) {  //No location found on first scan -->> Stop search with no found sites
            break_search = true;
            throw NoSite();
        }
        else if (count == 0) {          //No location found on an increase of area scan -->> Reduce site area and force accept
            duplicates = true;
            dimension_length = dimension_length_previous;
            dimension_width = dimension_width_previous;
        }
        else if (count == 1 || duplicates || fullLoopCount > maxNumberLoop) {    //Found a suitable landing site -->> Write landing site
            for (int k = 0; k < dimension_length; k++) {
                for (int l = 0; l < dimension_width; l++) {
                    valid.at<uchar>(position_length_save+k, position_width_save+l) = 255;
                }
            }
            for (int k = 0; k < 3; k++) {       //Todo Replace marker method
                for (int l = 0; l < 3; l++) {
                    valid.at<uchar>((int) round(position_length_save+dimension_length/2-1+k), (int) round(position_width_save+dimension_width/2-1+l)) = marker;
                }
            }
            site.height = dimension_length / scan.pixelPerMeter;
            site.width = dimension_width / scan.pixelPerMeter;
            break_search = true;
        }
        else {                          //Multiple landing sites found -->> Increases landing site area to find a larger site
            dimension_length_previous = dimension_length;
            dimension_width_previous = dimension_width;
            dimension_length = (int) round(dimension_length * multiplier);
            dimension_width = (int) round(dimension_width * multiplier);
            count = 0;
        }
        first_run = false;
    }
    return valid;
}

cv::Mat Selection::rotateImage(cv::Mat input, double angle) {
    cv::Mat output;
    cv::Point2f center(input.cols/2.0, input.rows/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Rect bbox = cv::RotatedRect(center,input.size(), angle).boundingRect();
    rot.at<double>(0,2) += bbox.width/2.0 - center.x;
    rot.at<double>(1,2) += bbox.height/2.0 - center.y;
    cv::warpAffine(input, output, rot, bbox.size());
    return output;
}

// Note: The 'angle' input is the same as the original rotation made to the image
cv::Mat Selection::unRotateImage(cv::Mat original, cv::Mat input, double angle) {
    cv::Mat output;
    cv::Point2f center(input.cols / 2.0, input.rows / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle * -1, 1.0);
    cv::Rect bbox = cv::RotatedRect(center, original.size(), 0).boundingRect();
    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    cv::warpAffine(input, output, rot, original.size());
    return output;
}

//ToDo Replace altitude value with parameters or algorithm. Also needs to be based on terrain
void Selection::setWaypoints(Site site) {
    std::cout << "Selection: Calculating flight path to landing sites" << std::endl;
    Waypoint temp;
    waypoint.clear();
    waypoint.reserve(20);
    Location planeLocation = scan.aircraft.getLocation();

    // Land on runway - Notes: Lands in the middle of site
    temp.angle = site.angle;
    temp.location = site.location;
    temp.altitude = 0;
    waypoint.push_back(temp);

    // Final approach - Note: Parameter height, Parameter distance, fixed angle,
    temp.angle = site.angle;
    temp.location = Convert::coordinateProjection(site.location, scan.param.landFinalApproachDistance, temp.angle);
    temp.altitude = scan.param.landFinalApproachAltitude;
    waypoint.push_back(temp);

    // End of turn - Note: Variable height, Fixed angle, Fixed distance
    temp.angle = site.angle;
    temp.location = Convert::coordinateProjection(waypoint.at(1).location, scan.param.landEndTurnDistance, temp.angle);
    temp.altitude = scan.param.landEndTurnAltitude;
    waypoint.push_back(temp);


    // Final approach turn algorithm
    double landArcViewTurn = scan.param.landArcViewTurn;               // The total viewing angle that a waypoint can be accepted within. If a waypoint is not seen, 1/2 of this value is used for the angle of the next waypoint to be plotted at.
    double landArcViewFinal = scan.param.landArcViewFinal;               // The total viewing angle that a waypoint can be accepted within, if its on final approach.
    double landArcDistance = scan.param.landArcDistance;              // Distance between consecutive waypoint during the turn
    double landArcDistanceFinal = scan.param.landArcDistanceFinal;          // Distance between first and second turn waypoints
    double landArcAcceptDistance = scan.param.landArcAcceptDistance;          // ToDo Not required with landing algorithm
    double landArcPartialAcceptDistance = scan.param.landArcPartialAcceptDistance; // ToDo May not be required with landing algorithm
    double landArcPartialAcceptAngle = scan.param.landArcPartialAcceptAngle;      // ToDo May not be required with landing algorithm
    double landArcRateOfDescent = scan.param.landArcRateOfDescent;            // Rate of descent of the aircraft during approach
    int landArcMinWaypoint = scan.param.landArcMinWaypoint;                 // Minimum number of turn waypoints to plot (total number of waypoints = landArcMinWaypoint + 3)

    bool landArcAccepted = false;
    double landArcView = landArcViewFinal;

    //Curved turn algorithm - Limit of 20 for spiral case
    for (int i=0; i<20; i++) {
        if (landArcAccepted && i>landArcMinWaypoint) {  // Enforces minimum waypoints
            break;
        }
        double angle = Convert::wrapTo180(Convert::coordinate2Bearing(waypoint.at(i+2).location,planeLocation) - waypoint.at(i+2).angle);
        if (i > 2) {                        //Allows final turn to be at a reduced angle
            landArcView = landArcViewTurn;
        }
        if (angle > landArcView/2) {        //Allows the angle of the waypoint to be set to the view window
            angle = landArcView/2;
        }
        else if (angle < -landArcView/2) {  //Negative case of above condition
            angle = -landArcView/2;
        }
        else {                              //Plane is within the window
            landArcAccepted = true;
        }

        // Limits unnecessary waypoints
        if (Convert::haversine(temp.location, planeLocation) < landArcAcceptDistance) {
            landArcAccepted = true;
        }
        if (Convert::haversine(temp.location, planeLocation) < landArcPartialAcceptDistance && fabs(angle) > landArcPartialAcceptAngle) {
            landArcAccepted = true;
        }

        temp.altitude += 20;
        if (landArcAccepted && (i-1)>landArcMinWaypoint) {
        	temp.altitude += 60;
        }
        temp.angle = angle + waypoint.at(i+2).angle;
        temp.location = Convert::coordinateProjection(waypoint.at(i+2).location, landArcDistance, temp.angle);
        waypoint.push_back(temp);


    }

    temp.angle = scan.aircraft.getBearing();
    temp.location = scan.aircraft.getLocation();
    temp.altitude = scan.aircraft.getAltitude();
    waypoint.push_back(temp);

    //Flips vector around so waypoint 0, is the first waypoint.
    std::vector<Waypoint> waypointTemp = waypoint;
    waypoint.clear();
    for(int i=0; i < (int) waypointTemp.size(); i++) {
        waypoint.push_back(waypointTemp.at(waypointTemp.size()-i-1));
    }
}

//ToDo replace with better method using maths instead of scanning an image for a marker
// Note: Provides reference from image center
Matrix Selection::calcLandSite(cv::Mat input) {
    cv::Size sz = input.size();
    Matrix location;
    for (int i=0; i<sz.height; i++) {
        for (int j=0; j<sz.width; j++) {
            if (input.at<uchar>(i,j) == marker) {
                location.x = j-sz.width/2;
                location.y = -1*(i-sz.height/2);
                break;
            }
        }
    }
    return location;
}

//ToDo remove after replacing marker method
cv::Mat Selection::removeMarker(cv::Mat input) {
    cv::Size sz = input.size();
    for (int i = 0; i < sz.height; i++) {
        for (int j = 0; j < sz.width; j++) {
            if (input.at<uchar>(i,j) == marker) {
                input.at<uchar>(i,j) = 255;
            }
        }
    }
    return input;
}

// Site selection process method
void Selection::process() {
    FileWriter time("selectionProcess");
    std::cout << "Selection: Starting" << std::endl;

    site.angle = Convert::wrapTo180(180 + scan.aircraft.getWindDirection());
    scan.data = unRotateImage(scan.data, scanner(rotateImage(scan.data, site.angle)), site.angle);
    site.location = Convert::matrix2Coordinate(scan.origin, calcLandSite(scan.data), scan.pixelPerMeter);
    scan.data = removeMarker(scan.data);    //ToDo replace unrotate method with calculation based method
    time.end();

    FileWriter time1("selectionWaypoint");
    setWaypoints(site);
    scan.processed = true;
    time1.end();

    cv::imwrite(FileWriter::cwdVariable("selectionSite.bmp"), scan.data);


}

