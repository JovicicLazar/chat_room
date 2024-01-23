#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <malloc.h>

#include "headers/room/room.hpp"

#define PORT 8080

int main(int argc, char const* argv[])
{
    try {
        Room* r = new Room();
        int server_fd, new_socket;
        server_fd = r->get_fd();
        sockaddr_in address = r->get_addr();
        int opt = 1;
        socklen_t addrlen = sizeof(address);
        char buffer[1024] = { 0 };
        char* hello = "Hello from server";

        std::thread connectionThread(&Room::handleConnections, r);
        connectionThread.detach();
        cout << "server up!!" <<endl;
        while(1){

        }

        // Close the listening socket (after breaking out of the loop)
        close(server_fd);
        delete r;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }
    
	return 0;
}
