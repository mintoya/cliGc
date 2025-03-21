#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#ifdef _WIN32
#include <windows.h>
void WFPRINT(const wchar_t *message) {
  BOOL success = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message,
                               wcslen(message), NULL, NULL);
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
void WFPRINT(const wchar_t *stringLit) { wprintf(stringLit); }
#endif
static FILE *debugfile = NULL;
void DEBUG(const wchar_t *format, ...) {
  if (!debugfile) {
    debugfile = fopen("debug.txt", "w"); // Open in text mode
    if (!debugfile)
      return;
    // Set stream to wide-oriented (first wide operation does this implicitly)
    fwide(debugfile, 1); // 1 = wide-oriented stream
  }

  va_list args;
  va_start(args, format);
  vfwprintf(debugfile, format, args); // Use vfwprintf for va_list
  va_end(args);
}

void closeDebug() { fclose(debugfile); }
