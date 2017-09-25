/*
 * Ter.cpp
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#include <Ter.h>

Ter::Ter() {
    height.resize(SRTM_SIZE);
    for (int i=0 ; i<SRTM_SIZE; i++)
        height[i].resize(SRTM_SIZE);
}

size_t Ter::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void Ter::downloadSRTM(const char* fileName) {
    std::cout << "Terrain: Downloading terrain data" << std::endl;

    //Define URL
    std::stringstream urlstm;
    urlstm << "http://firmware.ardupilot.org/SRTM/Australia/" << fileName << ".zip";
    std::string url = urlstm.str();
    std::cout << url << std::endl;
    //Fetch image
    CURL *curl;
    CURLcode res;
    FILE *fp;
    curl = curl_easy_init();
    std::stringstream openStm;
    openStm << DIR_TER << fileName;
    fp = fopen(openStm.str().c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //the img url
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); // pass the stream ptr when the writefunction is called
    res = curl_easy_perform(curl); // start curl
    curl_easy_cleanup(curl); // cleanup
    fflush(fp);
    fclose(fp);
    std::stringstream systemStm;
    systemStm << "unzip -j -q -o " << openStm.str().c_str() << " -d " << DIR_TER;
    std::cout << system(systemStm.str().c_str());

}

void Ter::processSRTM(const char* fileNameDir) {
    std::cout << "Terrain: Reading terrain data" << std::endl;

    std::ifstream file(fileNameDir, std::ios::in|std::ios::binary);
    if (!file) {
        std::cout << "Error opening SRTM file" << std::endl;
        return;
    }
    unsigned char buffer[2];
    for (int i=0; i<SRTM_SIZE; ++i) {
        for (int j=0; j<SRTM_SIZE; ++j) {
            if (!file.read(reinterpret_cast<char*>(buffer), sizeof(buffer))) {
                std::cout << "Error reading file" << std::endl;
                return;
            }
            height[i][j] = (buffer[0] << 8) | buffer[1];
        }
    }
}

void Ter::processGradient() {
    double tempX, tempY;
    cv::Mat tempGrad;
    cv::Size sz;
    sz.height = GRAD_SIZE;
    sz.width = GRAD_SIZE;
    tempGrad.create(sz, scan.data.type());

    Location locEnd = locURL;
    locEnd.lon = locEnd.lon + 1;
    double meterPerPixelX = Convert::haversine(locURL, locEnd)/SRTM_SIZE;
    locEnd = locURL;
    locEnd.lat = locEnd.lat + 1;
    double meterPerPixelY = Convert::haversine(locURL, locEnd)/SRTM_SIZE;
    std::cout << "Lon:" << meterPerPixelX << " Lat:" << meterPerPixelY << std::endl;

    for (int i=0; i<GRAD_SIZE; i++) {
        for (int j=0; j<GRAD_SIZE; j++) {
            tempX = (height[i][j] - height[i][j+1]) / meterPerPixelX;
            tempY = (height[i][j] - height[i+1][j]) / meterPerPixelY;
            if (sqrt(pow(tempX,2) + pow(tempY,2)) > scan.param.terrain_gradient) {
                tempGrad.at<uchar>(i,j) = 0;
            }
            else {
                tempGrad.at<uchar>(i,j) = 255;
            }
            //tempGrad.at<uchar>(j,i) = (int) round(sqrt(pow(tempX,2) + pow(tempY,2)) *255 / scan.param.terrain_gradient);
            //tempGrad.at<uchar>(i,j) = (int) round((height[i][j]-400)*255/1000);
        }
    }
    double dimension = sqrt(pow(scan.param.resolution.x/scan.pixelPerMeter/2,2) + pow(scan.param.resolution.y/scan.pixelPerMeter/2,2));
    std::cout << dimension << std::endl;
    Matrix start, end;
    start = Convert::srtmPixel(scan.origin, Convert::coordinateProjection(scan.origin, dimension, -45));
    end = Convert::srtmPixel(scan.origin, Convert::coordinateProjection(scan.origin, dimension, 135));
    if (!(start.x < 0 || start.y < 0 || end.x > GRAD_SIZE || end.y > GRAD_SIZE)) {
        tempGrad = tempGrad(cv::Rect(start.x, start.y, end.x-start.x, end.y-start.y));
        sz.width = scan.param.resolution.x;
        sz.height = scan.param.resolution.y;
        cv::resize(tempGrad, tempGrad, sz, 0, 0, cv::INTER_NEAREST);
        cv::imwrite("test32.bmp", tempGrad);
        scan.data = tempGrad;
    }
    else {
        std::cout << "Terrain: Error, outside of limits. Terrain not used." << std::endl;
        tempGrad = cv::Scalar::all(255);
        sz.width = scan.param.resolution.x;
        sz.height = scan.param.resolution.y;
        cv::resize(tempGrad, tempGrad, sz, 0, 0, cv::INTER_NEAREST);
        cv::imwrite("test51.bmp", tempGrad);
        scan.data = tempGrad;
    }
}

void Ter::process() {
    locURL = scan.origin;
    locURL.lat = abs((int)locURL.lat-1);
    locURL.lon = abs((int)locURL.lon);
    std::stringstream fileNameStm;
    fileNameStm << "S" << locURL.lat << "E" << locURL.lon << ".hgt";
    std::string fileName = fileNameStm.str();
    std::stringstream fileNameDirStm;
    fileNameDirStm << DIR_TER << fileName;
    std::string fileNameDir = fileNameDirStm.str();

    struct stat buf;
    if (stat(fileNameDir.c_str(), &buf) != 0) {
        downloadSRTM(fileName.c_str());
    }
    else {
        std::cout << "Terrain: Using cache terrain data" << std::endl;
    }
    processSRTM(fileNameDir.c_str());



    processGradient();

}

void Ter::update(Scan scan) {
    this->scan = scan;
    this->scan.scanType = ter;
    this->scan.data = cv::Scalar::all(255);
    process();
}

Scan Ter::getScan() {
    return scan;
}
