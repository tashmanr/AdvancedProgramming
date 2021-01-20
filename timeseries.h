/*
 * timeseries.h
 * Author: Rebecca Tashman 336423124
 * Created on 12/7/2020.
 */
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class TimeSeries {
private:
    map<string, vector<float>> timeSeriesMap;
    vector<string> categories;

public:
    TimeSeries(const char *CSVfileName);

    TimeSeries(){};

    map<string, vector<float>> getMap() const;

    vector<string> getCategories() const;
};


#endif /* TIMESERIES_H_ */
