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
        int status, valread;
        struct sockaddr_in serv_addr;
        char message[1024];
        char buffer[1024] = {0};

        Client *new_client = new Client();

        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
        refresh();

        new_client->msg_window = newwin(LINES - 3, COLS, 0, 0);
        box(new_client->msg_window, 0, 0);
        wrefresh(new_client->msg_window);

        int client_fd = new_client->get_client_fd();
        const char *username = "hahahaha";

        uint32_t username_len = ntohl(strlen(username)) - 1;
        send(new_client->get_client_fd(), &username_len, sizeof(uint32_t), 0);
        send(new_client->get_client_fd(), username, strlen(username), 0);

        std::thread connectionThread([new_client]() {
            while (1) {
                new_client->recieve_message();
            }
        });
        connectionThread.detach();

        std::thread sendingThread([new_client]() {
            new_client->send_message();
        });
        sendingThread.detach();

        int scroll_pos = 0;

        while (1) {
            int ch = getch(); // Get user input

            if (ch == 27) { // 27 is the ASCII code for the Esc key
                break;       // Exit the loop when Esc is pressed
            } else if (ch == KEY_UP && scroll_pos > 0) {
                scroll_pos--;
                werase(new_client->msg_window);
                box(new_client->msg_window, 0, 0);
                wrefresh(new_client->msg_window);
            } else if (ch == KEY_DOWN) {
                // Simulate scrolling for demonstration purposes
                scroll_pos++;
                werase(new_client->msg_window);
                box(new_client->msg_window, 0, 0);
                wrefresh(new_client->msg_window);
            }
        }

        close(client_fd);

        delete new_client;

        endwin(); // End ncurses

        return 0;
    } catch (const exception &e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}
