#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdarg.h>
#define MAXLEN 256

static wchar_t buffer[MAXLEN] = {0};

#ifdef _WIN32
#include <windows.h>
void WFPRINT(const wchar_t* message){
  BOOL success = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message,wcslen(message), NULL, NULL);
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
void WFPRINT(const wchar_t* stringLit){
/* wprintf(L"%ls",stringLit); */
wprintf(stringLit);
/* fwrite((wchar_t *)(l->head), sizeof(wchar_t), l->length, stdout); */
/* fputws(stringLit, stdout); */

}
#endif
