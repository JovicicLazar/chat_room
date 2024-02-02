#ifndef INTERFACE_H
#define INTERFACE_H

#include <string.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <ncurses.h>

using namespace std;

class Interface {
    public:
        Interface();
        ~Interface();

        vector<string> get_display_messages();
        void display_message(); 
        void handle_input_mwin();
        void handle_input_iwin();
        void print_to_window(const string message);

    private:

        struct MESSAGE_WINDOW {
            WINDOW* messages_window;
            WINDOW* messages_wrapper;
            int mwin_height;
            int mwin_width;
        };

        MESSAGE_WINDOW mwin;

        WINDOW* messages_wrapper;
        WINDOW* messages_window;
        WINDOW* input_wrapper;
        WINDOW* input_window;

        vector<string> display_messages;

        int maxX;
        int maxY;
        int row;
        int messages_end;
        int padding = 2;

        double ratio = 0.8;

};

#endif
