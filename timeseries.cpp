/*
 * timeseries.cpp
 * Author: Rebecca Tashman 336423124
 * Created on 12/7/2020.
 */

#include "timeseries.h"

/*Constructor*/
TimeSeries::TimeSeries(const char *CSVfileName) {
    timeSeriesMap = map<string, vector<float>>();
    categories = vector<string>();
    ifstream file(CSVfileName);
    string line, word;
    if (file) {
        getline(file, line);//reading the first line
        stringstream str(line);
        while (getline(str, word, ',')) {//creating the keys for the map
            timeSeriesMap.insert(std::pair<string, vector<float>>(word, vector<float>()));
            categories.push_back(word);
        }
    }
    if (file) {
        while (getline(file, line)) {//now need to fill the map pairs with data for each line in the csv
            int i = 0;
            stringstream str(line);
            while (getline(str, word, ',')) {//creating the keys for the map
                timeSeriesMap[categories[i]].push_back(stof(word));
                i++;
            }
        }
    }
    file.close();
}

/*This function returns a copy of the object's map*/
map<string, vector<float>> TimeSeries::getMap() const {
    return map<string, vector<float>>(timeSeriesMap);
}

vector<string> TimeSeries::getCategories() const {
    return vector<string>(this->categories);
}