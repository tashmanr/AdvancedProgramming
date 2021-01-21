/*
 * commands.h
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iomanip>

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}
};

class StandardIO : public DefaultIO {
    ifstream in;
    ofstream out;
public:
    StandardIO(string inputFile, string outputFile) {
        in.open(inputFile);
        out.open(outputFile);
    }

    virtual string read() {
        string s;
        in >> s;
        return s;
    }

    virtual void write(string text) {
        out << text;
    }

    virtual void write(float f) {
        out << f;
    }

    virtual void read(float *f) {
        in >> *f;
    }

    void close() {
        if (in.is_open())
            in.close();
        if (out.is_open())
            out.close();
    }

    ~StandardIO() {
        close();
    }
};

class SocketIO : public DefaultIO {
    int clientID;
    int bufferSize = 1024;
public:
    SocketIO(int clientID) : clientID(clientID) {}

    virtual string read() {
        string input;
        char c = 0;
        recv(clientID, &c, sizeof(char), 0);
        while (c != ' ' && c != '\n') {
            input += c;
            recv(clientID, &c, sizeof(char), 0);
        }
        return input;
    }

    virtual void write(string t) {
        const char *buffer = t.c_str();
        send(clientID, buffer, strlen(buffer), 0);
    }

    virtual void write(float f) {
        string s = to_string(f);
        write(s);
    }

    virtual void read(float *f) {
        char buffer[bufferSize];
        bzero(buffer, bufferSize);
        recv(clientID, buffer, bufferSize, 0);
        *f = stof(buffer);
    }

    ~SocketIO() {}
};

//This class is the data that will be passed through the execute parameter
class Info {
public:
    HybridAnomalyDetector *detector;
    vector<AnomalyReport> *ar;
    TimeSeries *train;
    TimeSeries *test;
    vector<vector<float>> anomalyWindows;

    Info() {
        detector = new HybridAnomalyDetector();
        ar = new vector<AnomalyReport>();
        train = new TimeSeries();
        test = new TimeSeries();
        anomalyWindows = vector<vector<float>>();
    }

    virtual ~Info() {};
};

class Command {
public:
    DefaultIO *dio;
    string description;

    Command(DefaultIO *dio, string string1) : dio(dio), description(string1) {}

    virtual void execute(Info *data) = 0;

    virtual ~Command() {}
};

//number 1 in options list
class Upload : public Command {
public:
    Upload(DefaultIO *dio) : Command(dio, "1.upload a time series csv file\n") {}

    void execute(Info *data) override {
        dio->write("Please upload your local train CSV file.\n");
        const char *trainFile = "train.csv";
        const char *testFile = "test.csv";
        ofstream train(trainFile);
        string s = dio->read();
        //keep reading stream as long as we haven't hit done
        while (s.find("done") == std::string::npos) {
            train << s + "\n";
            s = dio->read();
        }
        train.close();
        data->train = new TimeSeries(trainFile);
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        ofstream test(testFile);
        s = dio->read();
        //keep reading stream as long as we haven't hit done
        while (s.find("done") == std::string::npos) {
            test << s + "\n";
            s = dio->read();
        }
        test.close();
        data->test = new TimeSeries(testFile);
        dio->write("Upload complete.\n");
    }
};

//number 2 in options list
class Settings : public Command {
public:
    Settings(DefaultIO *dio) : Command(dio, "2.algorithm settings\n") {}

    void execute(Info *data) override {
        dio->write("The current correlation threshold is ");
        cout << "Good job! Made it to command 2!" << endl;
        dio->write(data->detector->getThreshold());
        dio->write("\n");
        dio->write("Type a new threshold\n");
        float value = stof(dio->read());
        //if value is not valid, request again
        while (value < 0 || value > 1) {
            dio->write("please choose a value between 0 and 1.\n");
            value = stof(dio->read());
        }
        data->detector->setThreshold(value);
    }
};

//number 3 in options list
class DetectAnomalies : public Command {
public:
    DetectAnomalies(DefaultIO *dio) : Command(dio, "3.detect anomalies\n") {}

    void execute(Info *data) override {
        data->detector->learnNormal(*(data->train));
        *data->ar = (data->detector->detect(*data->test));
        dio->write("anomaly detection complete.\n");
    }
};

//number 4 in options list
class DisplayResults : public Command {
public:
    DisplayResults(DefaultIO *dio) : Command(dio, "4.display results\n") {}

    void execute(Info *data) override {
        for (const AnomalyReport &a  : *data->ar) {
            dio->write(to_string(a.timeStep) + "\t" + a.description + "\n");
        }
        dio->write("Done.\n");
    }
};

//number 5 in options list
class UploadAndAnalyze : public Command {
public:
    UploadAndAnalyze(DefaultIO *dio) : Command(dio, "5.upload anomalies and analyze results\n") {}

    //function to read the file
    void readFile(Info *data) {
        vector<float> start;
        vector<float> end;
        vector<float> reports;
        dio->write("Please upload your local anomalies file.\n");
        const char *AnomaliesFile = "anomaliesFile.csv";
        ofstream file(AnomaliesFile);
        string s = dio->read();
        //keep reading until we find "done"
        while (s.find("done") == std::string::npos) {
            file << s + "\n";
            s = dio->read();
        }
        file.close();
        ifstream input(AnomaliesFile);
        string line, word;
        if (input) {
            while (getline(input, line)) {//fill the vectors with the data
                stringstream str(line);
                getline(str, word, ',');
                start.push_back(stof(word));
                getline(str, word, ',');
                end.push_back(stof(word));
                reports.push_back(0);
            }
        }
        input.close();
        dio->write("Upload complete.\n");
        //combine the vectors into anomalyWindows vector
        data->anomalyWindows = vector<vector<float>>();
        data->anomalyWindows.push_back(start);
        data->anomalyWindows.push_back(end);
        data->anomalyWindows.push_back(reports);
    }

    // Helper function to make the float a string, trimmed to the right precision
    string TrimFloatToString(float f, int n) {
        if (f == 0) {
            return "0";
        } else {
            stringstream ss;
            ss << std::fixed << setprecision(n) << f;
            string s = ss.str();
            //if the last number is not 0, return the string
            while (s[s.length() - 1] == '0'){
                n--;
                stringstream ss2;
                ss2 << std::fixed << setprecision(n) << f;
                s = ss2.str();
            }
            return s;


            if (s[s.length() - 1] != '0') {
                return s;
            } else { //else trim the last 0
                return TrimFloatToString(f, n-1);
            }
        }
    }

    //function to analyze the anomaly data
    void analyze(Info *data) {
        float FP = 0;
        float TP = 0;
        float numWindows = data->anomalyWindows.begin()->size();
        vector<ConsolidatedAnomalyReport> car = data->detector->ConsolidateReport(*(data->ar));
        for (ConsolidatedAnomalyReport a : car) {//loop to run on all consolidated anomaly reports
            bool flag = false;//flag for if each report matches the event
            for (int i = 0; i < numWindows; i++) {//loop to run on all events
                //checking if anomaly coincides with the event
                if ((a.start <= data->anomalyWindows[0][i] && a.end >= data->anomalyWindows[0][i])
                    || (a.start <= data->anomalyWindows[1][i] && a.end >= data->anomalyWindows[1][i])
                    || (a.start >= data->anomalyWindows[0][i] && a.end <= data->anomalyWindows[1][i])) {
                    if (data->anomalyWindows[2][i] == 0) {
                        TP++;
                    }
                    data->anomalyWindows[2][i]++;
                    //TP++;
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                FP++;
            }

        }
        //initialize N at total number of lines in test timeseries
        float N = data->test->getMap().begin()->second.size();
        for (int i = 0; i < numWindows; i++) {
            if (data->anomalyWindows[2][i] > 0) {
                N -= data->anomalyWindows[1][i] - data->anomalyWindows[0][i] + 1;
            }
        }
        float positiveT, positiveF;
        positiveT = TP / numWindows;
        positiveF = FP / N;
        dio->write("True Positive Rate: " + TrimFloatToString(positiveT, 3) + "\n"); //Todo: Add back in!
        //dio->write(floorf(positiveT * 1000.0) / 1000.0);//round to 3 points after decimal
        dio->write("False Positive Rate: " + TrimFloatToString(positiveF,3) + "\n");
        //dio->write(floorf(positiveF * 1000.0) / 1000.0);//round to 3 points after decimal
        //dio->write("\n");
    }

    void execute(Info *data) override {
        readFile(data);
        analyze(data);
    }
};

//number 6 in options list
class Exit : public Command {
public:
    Exit(DefaultIO *dio) : Command(dio, "6.exit\n") {}

    void execute(Info *data) override {
        return;
    }
};

#endif /* COMMANDS_H_ */
