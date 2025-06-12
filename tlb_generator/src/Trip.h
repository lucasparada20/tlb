#ifndef TRIP_H
#define TRIP_H

#include <stdio.h>

class Trip {
public:
    int origin;
    int destination;
    int start_time;
    int end_time;
    int scenario;

    // Constructor to initialize Trip object
    Trip(int _origin = -1, int _destination = -1, int _start_time = -1, int _end_time = -1, int _scenario = -1)
        : origin(_origin), destination(_destination), start_time(_start_time), end_time(_end_time), scenario(_scenario) {}

    void Show() const {
        printf("o:%d d:%d start_time:%d end_time:%d scenario:%d\n", origin, destination, start_time, end_time, scenario);
    }
};

#endif