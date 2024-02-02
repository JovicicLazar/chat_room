#include "../../headers/interface/interface.hpp"

Interface::Interface() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    getmaxyx(stdscr, this->maxY, this->maxX);

    int messages_wh       = (int)(this->maxY * this->ratio);
    int input_wh          = this->maxY - messages_wh;

    mwin.mwin_height      = messages_wh;
    mwin.mwin_width       = maxX;
    mwin.messages_wrapper = newwin(mwin.mwin_height, mwin.mwin_width, 0,0);
    mwin.messages_window  = newwin(mwin.mwin_height - this->padding, mwin.mwin_width - this->padding, 1,1);

    box(mwin.messages_wrapper, 0, 0);
    refresh();
    wrefresh(mwin.messages_wrapper);
    wrefresh(mwin.messages_window);
    
}

Interface::~Interface() {
    delwin(mwin.messages_window);
    delwin(mwin.messages_wrapper);

    delwin(input_wrapper);
    delwin(input_window);

    endwin();
}

void Interface::handle_input_mwin() {
    try {
        int character = 0;
        
        while ( true ) {
            character = getch();
            
            switch (character) {
                case KEY_UP:
                    if(messages_end <= mwin.mwin_height - this->padding) break;
                    if(messages_end <= 0) break;
                    messages_end --;
                    display_message();
                    break;
                
                case KEY_DOWN:
                    if (messages_end >= row) break;
                    messages_end++;
                    display_message();
                    break;
                
                case 10:
                    messages_end = row;
                    display_message();
                    break;

                case 27:
                    throw std::runtime_error("escape pressed: "  + std::string(strerror(errno)));
                    break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        delwin(messages_window);
        endwin();
        exit(EXIT_FAILURE);
    }
}

void Interface::print_to_window( const string message ) {
    if(messages_end < row ) {
        row++;
        display_messages.push_back( message );
    } else {
        row++;
        messages_end = row;
        display_messages.push_back( message );
        display_message();
    }
}

void Interface::display_message() {
    wclear(mwin.messages_window);
    int display_start = std::max(0, messages_end - mwin.mwin_height + this->padding);
    for (int i = display_start; i < min(row, display_start + mwin.mwin_height - this->padding); i++) {
        mvwprintw(mwin.messages_window, i - display_start, 0, "%s", (display_messages[i] + " end" + std::to_string(messages_end)
        + " row" + std::to_string(display_start)).c_str());
    }
    wrefresh(mwin.messages_window);
}
