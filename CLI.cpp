/*
 * CLI.cpp
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */
#include "CLI.h"
//constructor for CLI
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->data = new cliData();
    //initializing the order of the options
    commands.push_back(new Upload(dio));
    commands.push_back(new Settings(dio));
    commands.push_back(new DetectAnomalies(dio));
    commands.push_back(new DisplayResults(dio));
    commands.push_back(new UploadAndAnalyze(dio));
    commands.push_back(new Exit(dio));
}

void CLI::start(){
    bool indicator = true;
    while (indicator) {//as long as exit has not been chosen, keep going
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        //show options
        for (Command *i : commands) {
            dio->write(i->description);
        }
        //receive option choice
        string input = dio->read();
        int index = (int) input[0] - 48; //adjusting the char number to its integer counterpart
        if ( index < 7 && index > 0) { //if index is in range
            commands[(index - 1)]->execute(data);
        }
        if (index == 6) { //exit the program
            indicator = !indicator;
        }
    }
}


CLI::~CLI() {
}

