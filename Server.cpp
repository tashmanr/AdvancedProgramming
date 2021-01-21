/*
 * Server.cpp
 * Author: Rebecca Tashman 336423124
 * Updated on 20/1/2021.
 */
#include "Server.h"

Server::Server(int port) throw(const char *) {
    stopFlag = false; //initialize stopFlag
    //making a new TCP socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw "Socket failed";
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; //local address
    server.sin_port = htons(port);//function to switch host to network
    if (bind(fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        throw "Bind failed";
    }
    if (listen(fd, clientLimit) < 0) {
        throw "Listen failed";
    }
}

//function that lets clients connect to the server
void Server::start(ClientHandler &ch) throw(const char *) {
    alarm(1);
    t = new thread([&ch, this]() {
        while (!stopFlag) {//loops until stop function is called
            socklen_t clientSize = sizeof(client);
            alarm(1);
            clientSocket = accept(fd, (struct sockaddr *) &client, &clientSize);
            alarm(1);
            if (clientSocket < 0) {
                throw "Accept failed";
            }
            ch.handle(clientSocket);
            close(clientSocket);
        }
        close(fd);
    });
}

//function to stop the run
void Server::stop() {
    stopFlag = true;
    t->join(); // do not delete this!
    delete t; //freeing the memory
}

Server::~Server() {
}



