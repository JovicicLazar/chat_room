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

#include "headers/client/client.hpp"

#define PORT 8080

using namespace std;

int main(int argc, char const *argv[]) {
    try {
        Client *new_client = new Client();

        int client_fd = new_client->get_client_fd();
        const char *username = "hahahaha";

        uint32_t username_len = ntohl(strlen(username)) - 1;
        send(new_client->get_client_fd(), &username_len, sizeof(uint32_t), 0);
        send(new_client->get_client_fd(), username, strlen(username), 0);

        std::thread connectionThread([new_client]() {
                new_client->recieve_message();
        });
        connectionThread.detach();

        // std::thread sendingThread([new_client]() {
        //     new_client->send_message();
        // });
        // sendingThread.detach();

         while (1) {
          
        }

        close(client_fd);

        delete new_client;

        return 0;
    } catch (const exception &e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}
