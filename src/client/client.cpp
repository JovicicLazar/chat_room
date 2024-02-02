#include "../../headers/client/client.hpp"

using namespace std;

Client::Client() {
    try {        

        if ( (client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
            throw runtime_error(string("socket error: \n") + strerror(errno));
        }

        if ( setsockopt(client_fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) ) {
            throw runtime_error(string("setsockopt error: \n") + strerror(errno));
	    }

		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(8080);

		if (inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr) <= 0) {
			throw runtime_error(string("invalid address: \n") + strerror(errno));
		}

		if ((connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr))) < 0) {
			throw runtime_error(string("connection error: \n") + strerror(errno));
		}
    } catch ( const exception& e ) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void Client::recieve_message() {
	try {
		std::thread inputThread(&Interface::handle_input_mwin, &client_interface); // Create a new thread
        inputThread.detach(); 
		while( true ) {
			uint32_t message_bytes_size;
			recv(client_fd, &message_bytes_size, sizeof(uint32_t), 0);
			uint32_t message_length = ntohl(message_bytes_size);
			//cout << "message length: " << message_length << endl;

			char* message = new char[message_length];
			memset(message, 0, sizeof(message));
			//message[message_length] = '\0';
			uint64_t bytes_received = recv(client_fd, message, message_length, 0);
			
			if (bytes_received == 0) {
				delete[] message;
				throw std::runtime_error("disconnected from the server: "  + std::string(strerror(errno)));
			}
			if (bytes_received == -1) {
				delete[] message;
				throw std::runtime_error("recieve message error: "  + std::string(strerror(errno)));
			}
			client_interface.print_to_window(std::string(message));
			delete[] message;
		}
	} catch(const exception& e) {
		std::cout << e.what() << std::endl;
		return;
	}
}

void Client::send_message() {
	try {
		char message[256];
		while( 1 ) {
			cin.getline(message, 256);
			if(strlen(message) == 0) continue;
			uint32_t message_length = strlen(message);
			cout << message_length << endl;
			uint32_t message_size = ntohl(message_length);
			send(client_fd, &message_size, sizeof(uint32_t), 0);
			send(client_fd, message, message_length, 0);
		}
	} catch( const exception& e ) {
		cout << e.what() << endl;
	}
	
}

int Client::get_client_fd() {
    return client_fd;
}

string Client::get_username() {
    return username;
}
