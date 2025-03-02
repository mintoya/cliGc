#include "../layers/leyers2.h"
#include "../list/list.h"
#include "../wprint/wprint.h"
#include <assert.h>
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
void wsleep(useconds_t usec) {
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative
                              // value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#define $sleep(a) wsleep(a * 1000)
#else
#define $sleep(a) usleep(a * 1000)
#endif

void cursorOff() { WFPRINT(L"\033[?25l"); }
void cursorOn() { WFPRINT(L"\033[?25h"); }

static pthread_t threadIds;
static pthread_mutex_t lock;
volatile sig_atomic_t terminate = 0;
void handle_sigint(int sig) { terminate = 1; }

void draw(List *l) {
  // list of "Line"
  assert(l->width == sizeof(Line));
  box(l);
}

static void (*exp)(void);
void *frame(void *vargp) {
  int myid = getpid();
  while (!terminate) {
    pthread_mutex_lock(&lock);
    exp();
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
int begin(void (*layerFactory)(void)) {
  setlocale(LC_ALL, "");
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  setlocale(LC_ALL, ".UTF-8");
#endif
  if (pthread_mutex_init(&lock, NULL)) {
    printf("\n mutex init has failed\n");
    return 1;
  }
  signal(SIGINT, handle_sigint);
  setvbuf(stdout, NULL, _IOFBF, 16384);
  cursorOff();
  exp = layerFactory;
  pthread_create(&threadIds, NULL, frame, NULL);

  while (!terminate) {
    $sleep(1000);
  }

  pthread_join(threadIds, NULL);

  pthread_mutex_destroy(&lock);
  WFPRINT(L"\nThreads joined\n");
  pthread_exit(NULL);
}
