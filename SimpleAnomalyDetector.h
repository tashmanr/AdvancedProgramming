/*
 * SimpleAnomalyDetector.h
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

class ConsolidatedAnomalyReport{
public:
    const string description;
    const long start;
    const long end;
    ConsolidatedAnomalyReport(string description, long start, long end):description(description),start(start),end(end){}
};

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    int r,x,y; //details for circle
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
    float threshold;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<ConsolidatedAnomalyReport> ConsolidateReport(const vector<AnomalyReport> report);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

    void setThreshold(float t) {
        this->threshold = t;
    }

    float getThreshold() {
        return this->threshold;
    }
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */