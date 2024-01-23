#include "../../headers/room/room.hpp"

//TODO add error log
//TODO no same username
Room::Room() {
    try {
        ifstream file("./settings.json");
        Reader reader;
        Value setup_data;
        
        reader.parse(file, setup_data);

        this->port                = setup_data["port"].asInt();
        this->timeout             = setup_data["timeout"].asInt();
        this->max_connections     = setup_data["max_connections"].asInt();
        this->max_clients         = setup_data["max_clients"].asInt();
        room_addr.sin_family      = AF_INET;
	    room_addr.sin_addr.s_addr = INADDR_ANY;
	    room_addr.sin_port        = htons(this->port);

        if ( (room_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
            throw runtime_error(string("socket error: \n") + strerror(errno));
        }

        if ( setsockopt(room_fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) ) {
            throw runtime_error(string("setsockopt error: \n") + strerror(errno));
	    }

        if ( bind(room_fd, (struct sockaddr*)&room_addr,sizeof(room_addr)) < 0 ) {
            throw runtime_error(string("binding error: \n") + strerror(errno));
        }

        if ( listen(room_fd, 256) < 0 ) {
            throw runtime_error(string("listen error: \n") + strerror(errno));
        }
    } catch ( const exception& e ) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void Room::handleClientMessages(Client client) {

    char buffer[4096];
    timeval timeout;
    timeout.tv_sec         = this->timeout; 
    timeout.tv_usec        = 0;
    int client_fd          = client.get_client_fd();
    string client_username = client.get_username();

    try {
        if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw std::runtime_error("setsockopt error: " + std::string(strerror(errno)));
        }

        while( true ) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(client_fd, &read_fds);

            int ready = select(client_fd + 1, &read_fds, nullptr, nullptr, &timeout);

            if (ready == -1) {
                throw std::runtime_error("select error: " + std::string(strerror(errno)));
            } else if (ready == 0) {
                const char* timeoutMessage = "Timeout reached. Connection closing.";
                send(client_fd, timeoutMessage, strlen(timeoutMessage), 0);
                throw std::runtime_error("Timeout reached, closing thread" + std::string(strerror(errno)));
            } else {
                uint32_t message_size;
                recv(client_fd, &message_size, sizeof(uint32_t), 0);
                message_size = ntohl(message_size);
                cout <<"Message size: "<< message_size << endl;
                

                char* message = new char[message_size];

                memset(message, 0, sizeof(message));
                int bytesReceived = recv(client_fd, message, message_size, 0);

                if (bytesReceived == -1) {
                    throw std::runtime_error("recv error: " + std::string(strerror(errno)));
                } else if (bytesReceived == 0) {
                    throw std::runtime_error("client gracefully disconnected"  + std::string(strerror(errno)));
                }
                cout << message << endl;
                uint32_t k = htonl(message_size);

                for (auto& pair : connectedClients) {
                    if (pair.second.get_client_fd() != client_fd) {
                        send(pair.second.get_client_fd(), &k, sizeof(uint32_t), 0);
                        send(pair.second.get_client_fd(), message, message_size, 0);
                    }
                }
                delete[] message;
                timeout.tv_sec  = this->timeout; 
                timeout.tv_usec = 0;
            }
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        std::lock_guard<std::mutex> lock(clients_mutex);
        close(client_fd);
        this->connectedClients.erase(client_username);
        this->connected_clients--;
        return;
    }
}

void Room::handleConnections() {
    try {
        while( true ) {
            struct sockaddr_in client_address;
            socklen_t addrlen = sizeof(client_address);

            std::string username;
            char buffer[512];

            int client_fd = accept(room_fd, (struct sockaddr*)&client_address, &addrlen);
            uint32_t username_size;
            recv(client_fd, &username_size, sizeof(uint32_t), 0);
            username_size = ntohl(username_size);

            int bytesReceived = recv(client_fd, buffer, username_size, 0);

            if( connected_clients >= this->max_clients ) {
                const char* timeoutMessage = "Clients limit reached. Connection closing.";
                send(client_fd, timeoutMessage, strlen(timeoutMessage), 0);
                close(client_fd);
                continue;
            }

            username.append(buffer, bytesReceived);
   
            Client new_client( client_fd, username );
            std::lock_guard<std::mutex> lock(clients_mutex);

            this->connectedClients.insert(std::make_pair(new_client.get_username(), new_client));
            this->connected_clients++;

            int client_port = ntohs(client_address.sin_port);

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
            std::cout << "Connection accepted from: "<< new_client.get_username().c_str()<<" : " << client_ip << " port: " << client_port <<std::endl;

            std::thread clientThread(&Room::handleClientMessages, this, new_client);
            clientThread.detach();
        }
    } catch ( const exception& e ) {
        cerr << e.what() << endl;
        return;
    }
}

int Room::get_fd() {
    return this->room_fd;
}

int Room::get_socket() {
    return this->room_socket;
}

int Room::get_port() {
    return this->port;
}

int Room::get_timeout() {
    return this->timeout;
}

int Room::get_max_connections() {
    return this->max_connections;
}

int Room::get_max_clients() {
    return this->max_clients;
}

int Room::get_connected_clients() {
    return this->connected_clients;
}

sockaddr_in Room::get_addr() {
    return this->room_addr;
}

void Room::set_timeout(int new_timeout) {
    this->timeout = new_timeout;
}

void Room::set_max_connections(int new_max_connections) {
    this->max_connections = new_max_connections;
}

void Room::set_max_clients(int new_max_clients) {
    this->max_clients = new_max_clients;
}