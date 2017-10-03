/*
 * FileWriter.cpp
 *
 *  Created on: 26Sep.,2017
 *      Author: dello
 */

#include <FileWriter.h>

using namespace std;

string FileWriter::cwdVar = "/home/dello/EFLS/build/results/temp/";

FileWriter::FileWriter(const char* timeName) {
    this->timeName = timeName;
    start();
}

void FileWriter::start() {
    startTime = chrono::high_resolution_clock::now();
}

void FileWriter::end() {
    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - startTime);
    write(timeName, time_span.count());
}

void FileWriter::write(const char* input, double timeDifference){
    ofstream file;
    file.open(cwd("data.txt").c_str(), ios::app);
    file.precision(6);
    file << input << " " << timeDifference << endl;
    file.flush();
    file.close();
}

void FileWriter::write(const char* input){
    ofstream file;
    file.open(cwd("data.txt").c_str(), ios::app);
    file << input << endl;
    file.flush();
    file.close();
}

string FileWriter::cwd(const char* input) {
    stringstream message;
    message << "/home/dello/EFLS/build/results/temp/" << input;
    string messageStr = message.str();
    return messageStr;
}

string FileWriter::cwdVariable(const char* input) {
    stringstream message;
    message << cwdVar << input;
    string messageStr = message.str();
    return messageStr;
}

void FileWriter::setCwdVariable(const char* input) {
    cwdVar = input;
    mkdir(cwdVar.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}


