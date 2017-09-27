/*
 * FileWriter.h
 *
 *  Created on: 26Sep.,2017
 *      Author: dello
 */

#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <sstream>
#include <ctime>
#include <chrono>




class FileWriter {
    const char* timeName;
    std::chrono::high_resolution_clock::time_point startTime;
    static std::string cwdVar;


public:
    FileWriter(const char* timeName);
    void start();
    void end();
    //static std::string DIR_File;
    static void write(const char* input);
    static void write(const char* input, double timeDifference);
    static std::string cwd(const char* input);
    static std::string cwdVariable(const char* input);
    static void setCwdVariable(const char* input);
};

#endif /* FILEWRITER_H_ */
