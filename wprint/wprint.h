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
  /* BOOL success = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message,wcslen(message), NULL, NULL); */


      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return 1; // returns error code if unable to get handle
    }

    DWORD charsWritten;
    HANDLE hFile = CreateFileA(L"\0", GENERIC_WRITE, FILE_SHARE_READ |
FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return 1; // returns error code if unable to create file
    }

    if (!WriteFile(hFile, message, wcslen(message), &charsWritten, NULL)) {
        return 1; // returns error code if unable to write to file
    }

    CloseHandle(hFile);
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
