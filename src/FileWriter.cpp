/*
 * FileWriter.cpp
 *
 *  Created on: 26Sep.,2017
 *      Author: dello
 */

#include <FileWriter.h>

using namespace std;

string FileWriter::cwdVar = "/home/dello/EFLS/build/results/temp/";

// Initilise timeing profiler
FileWriter::FileWriter(const char* timeName) {
    this->timeName = timeName;
    start();
}

// Start time of timing profiler
void FileWriter::start() {
    startTime = chrono::high_resolution_clock::now();
}

// Calculates the time span of the requested timing profiler
void FileWriter::end() {
    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - startTime);
    write(timeName, time_span.count());
}

// The write method of the timing profiler
void FileWriter::write(const char* input, double timeDifference){
    ofstream file;
    file.open(cwd("data.txt").c_str(), ios::app);
    file.precision(6);
    file << input << " " << timeDifference << endl;
    file.flush();
    file.close();
}

// Writes a log message to file
void FileWriter::write(const char* input){
    ofstream file;
    file.open(cwd("data.txt").c_str(), ios::app);
    file << input << endl;
    file.flush();
    file.close();
}

// Returns the current working dictionary to save results to
string FileWriter::cwd(const char* input) {
    stringstream message;
    message << "/home/dello/EFLS/build/results/" << input;
    string messageStr = message.str();
    return messageStr;
}

// Returns the current working dictionary of the results from a specific interation of the Decision Engine
string FileWriter::cwdVariable(const char* input) {
    stringstream message;
    message << cwdVar << input;
    string messageStr = message.str();
    return messageStr;
}

// Changes the variable working dictionary
void FileWriter::setCwdVariable(const char* input) {
    cwdVar = input;
    mkdir(cwdVar.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}


