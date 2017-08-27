/*
 * Sat.cpp
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#include <Sat.h>
#include <DecisionEngine.h>


Sat::Sat() {
}

Scan Sat::getScan() {
    return scan;
}

void Sat::update(Scan scan) {
    this->scan = scan;
    this->scan.scanType = sat;
    process();  //Need this line of code for processing!!!
}

void Sat::save() {
    cv::Mat src, dst;
    src = rawImage;
    cv::multiply(src, cv::Scalar::all(1), dst, 0.5);
    src = cv::Scalar::all(255);
    src.copyTo(dst, scan.data);
    cv::imwrite("result.bmp", dst);
}

cv::Mat Sat::lineExpansion(cv::Mat input) {
    cv::Mat edges;
    cv::Size sz = input.size();
    edges.create(input.size(), input.type());
    edges = cv::Scalar::all(255);
    int boundary = 3;   //ToDo make parameter
    for (int i = boundary; i < sz.height - boundary; i++) {
        for (int j = boundary; j < sz.width - boundary; j++) {
            if (input.at<uchar>(i,j) == 0) {
                for (int k = 0; k < 5; k++) {
                    for (int l = 0; l < 5; l++) {
                        edges.at<uchar>(i+k-boundary, j+l-boundary) = 0;
                    }
                }
            }
        }
    }
    return edges;
}

// Note:: Black pixel is edges and white pixel is valid area
cv::Mat Sat::cannyDetection(cv::Mat src) {
    cv::Mat src_gray, edges, edges_invert;
    cv::cvtColor(src, src_gray, CV_BGR2GRAY );
    cv::blur(src_gray, edges, cv::Size(3, 3));
    cv::Canny(edges, edges, lowThreshold, lowThreshold * ratio, kernel_size);
    edges_invert.create(edges.size(), edges.type());
    cv::subtract(cv::Scalar::all(255),edges, edges_invert);
    cv::imwrite("test.bmp",edges_invert);
    return edges_invert;
}

size_t Sat::write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ostringstream *stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    return count;
}

cv::Mat Sat::fetchImage(Location originFetch) {
    //Define URL
    std::stringstream urlstm;
    urlstm.precision(10);
    urlstm << "http://maps.googleapis.com/maps/api/staticmap?maptype=satellite&center=" << originFetch.lat << "," << originFetch.lon << "&zoom=" << scan.zoom << "&size=640x640&key=AIzaSyC2C3Q5lTuaYkaw7m8A9XLDgq7avZuMxQ4";
    std::string url = urlstm.str();
    std::cout << url << "\n";
    //Fetch image
    CURL *curl;
    CURLcode res;
    std::ostringstream stream;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //the img url
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr when the writefunction is called
    res = curl_easy_perform(curl); // start curl
    std::string output = stream.str(); // convert the stream into a string
    curl_easy_cleanup(curl); // cleanup
    std::vector<char> data = std::vector<char>( output.begin(), output.end() ); //convert string into a vector
    cv::Mat data_mat = cv::Mat(data); // create the cv::Mat datatype from the vector
    cv::Mat image = cv::imdecode(data_mat,1); //read an image from memory buffer
    image = image(cv::Rect(20,20,600,600)); //Crop image to remove watermark. Center coordinate is kept the same.
    return image;
}

//Returns satellite image, that is made up of 25 smaller images (5x5)
cv::Mat Sat::getImage() {
    //Creates file name to check and use
    Location locMod = scan.aircraft.getLocation();
    std::stringstream tempLocMod;
    tempLocMod.precision(5);
    tempLocMod << locMod.lat;
    locMod.lat = atof(tempLocMod.str().c_str());
    tempLocMod.str("");
    tempLocMod << locMod.lon;
    locMod.lon = atof(tempLocMod.str().c_str());

    std::stringstream fileNameStm;
    fileNameStm.precision(5);
    fileNameStm << "SatImageLat" << locMod.lat << "Lon" << locMod.lon << ".bmp";
    std::string fileName = fileNameStm.str();
    scan.origin = locMod;
    //Checks if cache exists for the image
    cv::Mat images;
    struct stat buf;
    if (stat(fileName.c_str(), &buf) == 0) {
        std::cout << "Using cached satellite image.\n";
        images = cv::imread(fileName);
        return images;
    }
    double angle;
    double pixelDimension = 600;
    double distance;
    cv::Mat imagesHoz;
    std::vector<Location> originRows;
    originRows.reserve(6);
    for (int i=0; i<5; i++) {
        distance = sqrt(pow(2,2) + pow(2-i,2))*pixelDimension/scan.pixelPerMeter;
        angle = 270 + Convert::rad2Deg(atan((double) (2-i)/2));
        originRows.push_back(Convert::coordinateProjection(scan.origin, distance, angle));
    }
    Location originFetch;
    for (int i=0; i<5; i++) {
        originFetch = originRows.at(i);
        imagesHoz.release();
        imagesHoz = fetchImage(originFetch);
        for (int j=0; j<4; j++){
            originFetch = Convert::coordinateProjection(originFetch, pixelDimension/scan.pixelPerMeter, 90);
            cv::hconcat(imagesHoz, fetchImage(originFetch), imagesHoz);
        }
        if (i == 0) {
            images = imagesHoz;
        }
        else {
            cv::vconcat(images, imagesHoz, images);
        }
    }
    return images;
}

void Sat::process() {
    cv::Mat src, dst;
    src = getImage();
    rawImage = src;
    dst.create( src.size(), src.type() );
    cv::Mat detected_edges;
    detected_edges = cannyDetection(src);
    detected_edges = lineExpansion(detected_edges);

    scan.data = detected_edges;
    scan.processed = true;
}

cv::Mat Sat::getRawImage() {
    return rawImage;
}

void Sat::runBuddy() {
    cv::Mat image = getImage();
    cv::imwrite("large.bmp", image);
    cv::namedWindow("Image output", CV_WINDOW_AUTOSIZE);
    cv::imshow("Image output", image); //display image
    cvWaitKey(0); // press any key to exit
    cv::destroyWindow("Image output");
}

