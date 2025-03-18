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
    debugfile = fopen("debug.txt", "w");
  }
  va_list args;
  va_start(args, format);
  fwprintf(debugfile, format, args);
  va_end(args);
}
void closeDebug() { fclose(debugfile); }
