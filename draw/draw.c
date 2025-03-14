#include "draw.h"
#include "../layers/leyers2.h"
#include "../list/list.h"
#include "../wprint/wprint.h"
#include <locale.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

#ifdef _WIN32
#include <windows.h>
void asleep(unsigned long long usec) {
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative
                              // value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
void $sleep(unsigned long long a) { asleep(a * 1000); }
#else
void $sleep(unsigned long long a) { usleep(a * 1000); }
#endif
volatile sig_atomic_t todraw = 0;
void cursorOff() { WFPRINT(L"\033[?25l"); }
void cursorOn() { WFPRINT(L"\033[?25h"); }

static pthread_mutex_t lock;
static List *screen = NULL;
void *worker(void *thing) {
  while (1) {
    cursorOff();
    pthread_mutex_lock(&lock);
    if (todraw && screen) {
      todraw = 0;
      box(screen);
      fflush(stdout);
    }
    pthread_mutex_unlock(&lock);
  }
}
void draw(List *l) {
  pthread_mutex_lock(&lock);
  todraw = 1;
  screen = l;
  pthread_mutex_unlock(&lock);
}
void begin(void) {

#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  setlocale(LC_ALL, ".UTF-8");
#else
  setlocale(LC_ALL, "");
#endif
  if (pthread_mutex_init(&lock, NULL)) {
    printf("\n mutex init has failed\n");
    return;
  }
  pthread_t thing1;
  pthread_create(&thing1, NULL, worker, 0);
  /* pthread_join(thing1, NULL); */
  /* pthread_mutex_destroy(&lock); */
}
