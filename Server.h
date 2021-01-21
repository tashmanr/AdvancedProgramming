/*
 * Server.h
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"

using namespace std;

class ClientHandler {
protected:
    DefaultIO *dio;
public:
    virtual void handle(int clientID) = 0;
};

class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        dio = new SocketIO(clientID);//create socketIO
        CLI cli(dio);//create cli
        cli.start();
    }
};

class Server {
    thread *t; // the thread to run the start() method in
    int fd; //file descriptor
    int clientLimit = 2;
    int clientSocket;
    bool stopFlag;
    sockaddr_in server;
    sockaddr_in client;
public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
