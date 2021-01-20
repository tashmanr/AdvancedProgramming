/*
 * CLI.h
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    vector<Command *> commands;
    // you can add data members
public:
    cliData* data;
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
