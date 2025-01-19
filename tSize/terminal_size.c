#include "terminal_size.h"

#ifdef _WIN32
#include <windows.h>

TerminalSize get_terminal_size() {
  TerminalSize size = {0, 0};

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  }

  return size;
}

#else
#include <sys/ioctl.h>
#include <unistd.h>

TerminalSize get_terminal_size() {
  TerminalSize size = {0, 0};

  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
    size.width = ws.ws_col;
    size.height = ws.ws_row;
  }

  return size;
}

#endif
