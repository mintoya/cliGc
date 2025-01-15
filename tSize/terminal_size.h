#ifndef TERMINAL_SIZE_H
#define TERMINAL_SIZE_H

// Define a struct to store the terminal size (width and height)
typedef struct {
    int width;
    int height;
} TerminalSize;

// Function prototype to get the size of the terminal
TerminalSize get_terminal_size();

#endif // TERMINAL_SIZE_H
