#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

#include <stdexcept> 
#include <cstring>   
#include <cstdlib>   
#include <cerrno> 

using namespace std;

class Client {
    public:
        Client(int client_fd, string username);

        int get_client_fd();
        string get_username();

    private:
        int client_fd;
        string username;
};

#endif
