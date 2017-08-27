/*
 * Priority.cpp
 *
 *  Created on: 12Jul.,2017
 *      Author: dello
 */

#include <Priority.h>

Priority::Priority() {
}

void Priority::updateLevel(int range) {
    if (range >= 0) {
        level += range * param.getPriority_range();
    }
    level += iteration;
    iteration++;
    if (level > 10) {
        level = 10;
    }
    if (level < 0) {
        level = 0;
    }
}

void Priority::reset() {
    level = 0;
    iteration = 0;
}

int Priority::getLevel() {
    return level;
}

