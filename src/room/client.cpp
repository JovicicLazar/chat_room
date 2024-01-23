#include "../../headers/room/client.hpp"

Client::Client( int client_fd, string username ) {
    this->client_fd = client_fd;
    this->username  = username;
}

int Client::get_client_fd() {
    return client_fd;
}

string Client::get_username() {
    return username;
}