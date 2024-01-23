#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h> 

#include <stdexcept> 
#include <cstring>   
#include <cstdlib>   
#include <cerrno> 

#include <jsoncpp/json/json.h>

#include "client.hpp"

using namespace std;
using namespace Json;

class Room {
    public:
        Room();

        void handleConnections();
        void handleClientMessages(Client client);

        int get_fd();
        int get_socket();
        int get_port();
        int get_timeout();
        int get_max_connections();
        int get_max_clients();
        int get_connected_clients();

        sockaddr_in get_addr();

        void set_timeout(int new_timeout);
        void set_max_connections(int new_max_connections);
        void set_max_clients(int new_max_clients);

    private:
        int room_fd;
        int room_socket;
        int opt = 1;
        int port;
        int timeout;
        int max_connections;
        int max_clients;
        int connected_clients;

        sockaddr_in room_addr; 
        mutex clients_mutex; 
        unordered_map< string, Client > connectedClients;
};

#endif
