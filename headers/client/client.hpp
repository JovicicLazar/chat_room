#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <limits>
#include <thread>
#include <stdio.h>
#include <ncurses.h>

#include "../../headers/room/client.hpp"

using namespace std;

class Client {
    public:
        Client();

        WINDOW *msg_window;

        void send_message();
        void recieve_message();

        int get_client_fd();
        string get_username();

    private:
        int client_fd;
        int opt = 1;
        int port;

        string username;
        sockaddr_in client_addr;
};

#endif
