/*
 * Ter.cpp
 *
 *  Created on: 13Jul.,2017
 *      Author: dello
 */

#include <Ter.h>

Ter::Ter() {

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
    const int GRAD_SIZE = SRTM_SIZE - 1;
    double gradient[GRAD_SIZE][GRAD_SIZE] = { { 0 } };
    double tempX, tempY;
    Location locEnd = locURL;
    locEnd.lon = locEnd.lon + 1;
    double pixelPerMeterX = Convert::haversine(locURL, locEnd);
    locEnd = locURL;
    locEnd.lat = locEnd.lat + 1;
    double pixelPerMeterY = Convert::haversine(locURL, locEnd);
    std::cout << "Lon:" << pixelPerMeterX << " Lat:" << pixelPerMeterY << std::endl;

    for (int i=0; i<GRAD_SIZE; i++) {
        for (int j=0; j<GRAD_SIZE; j++) {
            tempX = (height[i][j] + height[i][j+1]) / (1/pixelPerMeterX);
            tempY = (height[i][j] + height[i+1][j]) / (1/pixelPerMeterY);
            gradient[i][j] = sqrt(pow(tempX,2) + pow(tempY,2));
            std::cout << gradient[i][j] << " ";
        }
    }
}

void Ter::process() {
    locURL = scan.origin;
    locURL.lat = abs((int)locURL.lat);
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
    process();
}

Scan Ter::getScan() {
    return scan;
}
