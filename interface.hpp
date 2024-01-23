#ifndef INTERFACE_H
#define INTERFACE_H

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <ncurses.h>

using namespace std;

class Interface {
    public:
        Interface();

        WINDOW get_interface_window();

    private:
        WINDOW* interface_window;

};

#endif
