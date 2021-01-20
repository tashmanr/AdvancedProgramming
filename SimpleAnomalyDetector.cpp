/*
 * SimpleAnomalyDetector.cpp
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    setThreshold(.9); //default value for threshold
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

//This function receives a TimeSeries and learns the expected correlations
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    TimeSeries timeSeries = ts;
    map<string, vector<float>> tmpMap = timeSeries.getMap();
    float tmp, tmpMax;
    int size = tmpMap.begin()->second.size();
    /* We run a loop on the categories, and then another loop
     * on the categories to the right of the first one.
     */
    for (auto itr = tmpMap.begin(); itr != tmpMap.end(); itr++) {//outer loop
        tmpMax = 0;
        string f2;
        for (auto itr2 = itr; itr2 != tmpMap.end(); itr2++) {//inner loop
            if (itr2 == itr) {
                continue;
            }
            tmp = abs(pearson(itr->second.data(), itr2->second.data(), size));
            if (tmp > tmpMax) {//to get maximum correlation
                tmpMax = tmp;
                f2 = itr2->first;
            }
        }
        if (tmpMax >= 0.5) {//check if max correlation > threshold (0.5) for circle
            correlatedFeatures tmpCf;
            tmpCf.x = 0;
            tmpCf.y = 0;
            tmpCf.r = 0;
            tmpCf.feature1 = itr->first;
            tmpCf.feature2 = f2;
            tmpCf.corrlation = tmpMax;
            vector<Point *> points;
            for (int i = 0; i < size; i++) {
                points.push_back(new Point(tmpMap[tmpCf.feature1][i], tmpMap[tmpCf.feature2][i]));
            }
            Point **pointsArr = points.data();
            if (tmpMax >= threshold) {
                tmpCf.lin_reg = linear_reg(pointsArr, size);
                tmpMax = 0;
                for (int i = 0; i < size; i++) {
                    float tmpDev = dev(pointsArr[i][0], tmpCf.lin_reg);
                    if (tmpDev > tmpMax) {//getting max threshold for dev of individual points in the vector
                        tmpMax = tmpDev;
                    }
                }
            } else {//if didn't pass threshold defined, but over 0.5, use minCircle
                Circle circle = findMinCircle(pointsArr, size);
                tmpCf.x = circle.center.x;
                tmpCf.y = circle.center.y;
                tmpCf.r = circle.radius;
                tmpMax = 0;
                for (int i = 0; i < size; i++) {
                    float tmpDev = distance(pointsArr[i][0], circle.center);
                    if (tmpDev > tmpMax) {//getting max threshold for dev of individual points in the vector
                        tmpMax = tmpDev;
                    }
                }
            }
            tmpCf.threshold = tmpMax;
            cf.push_back(tmpCf);
            for (int i = 0; i < size; i++) {
                points.push_back(new Point(tmpMap[tmpCf.feature1][i], tmpMap[tmpCf.feature2][i]));
            }
        }
    }
}

//This function receives a TimeSeries (live - line by line) and reports anomalies
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    map<string, vector<float>> tmpMap = ts.getMap();
    int vectorSize = tmpMap.begin()->second.size();
    vector<AnomalyReport> report;
    for (auto itr = cf.begin(); itr != cf.end(); itr++) {//loop to go through the correlated features
        for (int i = 0; i < vectorSize; i++) {//loop to go through the rows in the time series
            float tmp;
            if (itr->r == 0) {
                tmp = dev(Point(tmpMap[itr->feature1][i], tmpMap[itr->feature2][i]), itr->lin_reg);
            } else {
                tmp = distance(Point(tmpMap[itr->feature1][i], tmpMap[itr->feature2][i]), Point(itr->x, itr->y));
            }
            if (tmp > (itr->threshold * 1.1)) {
                report.emplace_back(string(itr->feature1 + "-" + itr->feature2), i + 1);
            }

        }
    }
    return report;
}

//This function receives a vector of anomaly reports and consolidates them into time frames per description
vector<ConsolidatedAnomalyReport> SimpleAnomalyDetector::ConsolidateReport(const vector<AnomalyReport> report) {
    vector<ConsolidatedAnomalyReport> car = vector<ConsolidatedAnomalyReport>();
    vector<string> descriptions = vector<string>();
    vector<pair<float,float>> times = vector<pair<float,float>>();//pairing up start and end times
    for (const auto & i : report) {
        //if description already exists, see if the event is at the same time
        auto it = find(descriptions.begin(), descriptions.end(), i.description);
        if (it != descriptions.end()) {
            int index = distance(descriptions.begin(), it);
            // if our second is right after the ending time, we'll add the ending time by one
            if (times[index].second == (i.timeStep-1)) {
                times[index].second++;
            }
            else {//else we'll add it to the consolidated report as a new event (maximum we'll add it back later)
                descriptions.push_back(i.description);
                times.emplace_back(i.timeStep,i.timeStep);
            }
        } else {//else we'll add it to the consolidated report as a new event
            descriptions.push_back(i.description);
            times.emplace_back(i.timeStep,i.timeStep);
        }
    }
    int limit = descriptions.size();
    for (int j = 0; j < limit; j++){
        car.emplace_back(descriptions[j], times[j].first, times[j].second);
    }
    return car;
}