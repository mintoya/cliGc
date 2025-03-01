#include "../list/list.h"
#include "../tSize/terminal_size.h"
#include "../wprint/wprint.h"
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define BENCHMARK 0 // turns off the diff functino

#define $eq(a, b) !memcmp(&a, &b, sizeof(a))

void setCursorPosition(int row, int col) {
  static wchar_t buffer[20] = {0};
  swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"\033[%d;%dH", row, col);
  WFPRINT(buffer);
}
wchar_t *setAlloc(const wchar_t *input) {
  wchar_t *new_string = (wchar_t *)calloc(wcslen(input) + 1, sizeof(wchar_t));
  wcsncpy(new_string, input, wcslen(input));
  return new_string;
}

typedef enum {
  RESET = 0,
  RED = 31,
  GREEN = 32,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
  WHITE = 37,
  BRIGHT_RED = 91,
  BRIGHT_GREEN = 92,
  BRIGHT_YELLOW = 93,
  BRIGHT_BLUE = 94,
  BRIGHT_MAGENTA = 95,
  BRIGHT_CYAN = 96,
  BRIGHT_WHITE = 97,
  BG_BLACK = 40,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_YELLOW = 43,
  BG_BLUE = 44,
  BG_MAGENTA = 45,
  BG_CYAN = 46,
  BG_WHITE = 47,
  BG_BRIGHT_BLACK = 100,
  BG_BRIGHT_RED = 101,
  BG_BRIGHT_GREEN = 102,
  BG_BRIGHT_YELLOW = 103,
  BG_BRIGHT_BLUE = 104,
  BG_BRIGHT_MAGENTA = 105,
  BG_BRIGHT_CYAN = 106,
  BG_BRIGHT_WHITE = 107
} Color;
typedef enum {
  Horizontal = 1,
  Vertical = 0,
} Direction;

typedef struct {
  uint8_t row;
  uint8_t col;
  Color fgColor;
  Color bgColor;
  Direction ori;
  wchar_t *contents; // cant contain escape sequences
} Line;
typedef struct {
  wchar_t g;
  Color fg;
  Color bg;
} Cell;

static const Cell noCell = {0, 0, 0};
static const Cell newLine = {L'\n', 0, 0};
static const Cell space = {L' ', 0, 0};
static const Cell hor = {L'─', 0, 0};
static const Cell ver = {L'│', 0, 0};
static const Cell tl = {L'┌', 0, 0};
static const Cell bl = {L'└', 0, 0};
static const Cell tr = {L'┐', 0, 0};
static const Cell br = {L'┘', 0, 0};

const wchar_t *getColors(Color foreground, Color background) {
  static wchar_t colorCode[20] = {0};
  swprintf(colorCode, 20, L"\033[%d;%dm", foreground, background);
  return colorCode;
}
Line Line_new(int row, int col, Color fg, Color bg, Direction orientation,
              wchar_t *content) {
  Line a;
  a.row = row;
  a.ori = orientation;
  a.col = col;
  a.bgColor = bg;
  a.fgColor = fg;
  a.contents = setAlloc(content);
  return a;
}
void Layer_delete(List *l) {
  for (int i = 0; i < l->length; i++) {
    Line *loc = List_gst(l, i);
    free(loc->contents);
  }
  List_delete(l);
}
void stringAppend(List *l, const wchar_t *nTString) {
  for (int i = 0; nTString[i]; i++) {
    List_append(l, (void *)(nTString + i));
  }
}
void rasterset(Cell *c, TerminalSize ts, int row, int col, Cell element) {
  if (row >= ts.height || col >= ts.width)
    return;
  *(c + row * (ts.width + 1) + col) = element;
  return;
}
Cell *Leyer_rasterize(List *l, TerminalSize ts) {
  Cell *grid = (Cell *)calloc((ts.height + 1) * (ts.width + 1), sizeof(Cell));
  for (int i = 0; i < l->length; i++) {
    Line *line = List_gst(l, i);
    for (int j = 0; line->contents[j]; j++) {
      Cell thisCell;
      thisCell.bg = line->bgColor;
      thisCell.fg = line->fgColor;
      thisCell.g = line->contents[j];
      rasterset(grid, ts, line->row + (!line->ori) * j,
                line->col + line->ori * j, thisCell);
    }
  }
  return grid;
}
void cellArrMerge(Cell *completeLeyer, Cell *incompleteLeyer) {
  // mutates the completeLeyer
  for (int i = 0; completeLeyer[i].g; i++) {
    if (incompleteLeyer[i].g)
      completeLeyer[i] = incompleteLeyer[i];
  }
}

Cell *bottomLayer(TerminalSize ts) {
  Cell *grid = (Cell *)calloc((ts.height + 1) * (ts.width + 1), sizeof(Cell));
  for (int i = 0; i < (ts.height) * (ts.width + 1); i++) {
    grid[i] = space;
  }
  for (int i = 0; i < ts.height; i++) {
    for (int j = 0; j < ts.width; j++) {
      if (i == 0 || i == ts.height - 1) {
        *(grid + i * (ts.width + 1) + j) = hor;
      }
      if (j == 0 || j == ts.width - 1) {

        *(grid + i * (ts.width + 1) + j) = ver;
      }
      if (i == ts.height - 1) {
        grid[(i * (ts.width + 1)) + ts.width] = noCell;
      } else {
        grid[(i * (ts.width + 1)) + ts.width] = newLine;
      }
    }
  }

  /* *(grid + row * (ts.width + 1) + col) = element; */
  *(grid + 0 * (ts.width + 1) + 0) = tl;
  *(grid + 0 * (ts.width + 1) + (ts.width - 1)) = tr;
  *(grid + (ts.height - 1) * (ts.width + 1) + (0)) = bl;
  *(grid + (ts.height - 1) * (ts.width + 1) + (ts.width - 1)) = br;

  return grid;
}
static List *printCellsBuffer = NULL;
void printCells(Cell *cellLayer) {
  setCursorPosition(0, 0);
  if (printCellsBuffer == NULL) {
    printCellsBuffer = List_new(sizeof(wchar_t));
  } else {
    printCellsBuffer->length = 0;
  }

  wchar_t prevColor[20] = L"";

  for (int i = 0; cellLayer[i].g; i++) {
    Cell lcell = cellLayer[i];
    wchar_t *colorStr = getColors(lcell.fg, lcell.bg);
    if (wcscmp(colorStr, prevColor) != 0) {
      stringAppend(printCellsBuffer, colorStr);
      wcsncpy(prevColor, colorStr, 19);
      prevColor[19] = 0;
    }

    List_append(printCellsBuffer, &(lcell.g));
  }

  wchar_t zero = 0;
  List_append(printCellsBuffer, &zero);
  WFPRINT(printCellsBuffer->head);
}

Cell *LastLRender = NULL;
void printCellDiff(Cell *cellLayer) {
  int i = 0;
  int row = 1;
  int col = 1;
  while (cellLayer[i].g) {
    Cell current = cellLayer[i];

    if (!$eq(LastLRender[i], current)) {
      setCursorPosition(row, col);
      WFPRINT(getColors(current.fg, current.bg));
      wchar_t literal[2] = {current.g, 0};
      WFPRINT(literal);
    }

    if (current.g == L'\n') {
      row++;
      col = 1;
    } else {
      col++;
    }
    i++;
  }
  WFPRINT(getColors(RESET, RESET));
}
TerminalSize LastTerminalSize = {0, 0};

void box(List *content) {
  TerminalSize ts = get_terminal_size();
  setCursorPosition(0, 0);

  Cell *screen = bottomLayer(ts);
  Cell *lr = Leyer_rasterize(content, ts);
  cellArrMerge(screen, lr);
  free(lr);

  if (!BENCHMARK && $eq(LastTerminalSize, ts)) {
    printCellDiff(screen);
    /* idk windows or linux is lieing about the duration of the sleep functinos
     */
  } else {
    printCells(screen);
  }
  fflush(stdout);

  if (LastLRender != NULL) {
    free(LastLRender);
  }
  LastTerminalSize = ts;
  LastLRender = screen;
}
