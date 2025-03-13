#include "leyers2.h"
#include "../list/list.h"
#include "../tSize/terminal_size.h"
#include "../wprint/wprint.h"
#include <assert.h>
#include <locale.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#define BENCHMARK 0 // turns off the diff functino

#define $eq(a, b) !memcmp(&a, &b, sizeof(a))

void setCursorPosition(int row, int col) {
  static wchar_t buffer[20] = {0};
  swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"\033[%d;%dH", row, col);
  WFPRINT(buffer);
}
wchar_t *setAlloc(const wchar_t *input) {
  // stack-allocated strign copy
  wchar_t *new_string = (wchar_t *)calloc(wcslen(input) + 1, sizeof(wchar_t));
  wcsncpy(new_string, input, wcslen(input));
  return new_string;
}
uint8_t nibbleConvert(char nibble[2]) {
  uint8_t result;
  if (nibble[0] >= 'A' && nibble[0] <= 'F') {
    result = nibble[0] - 'A' + 10;
  } else {
    result = nibble[0] - '0';
  }
  result <<= 4;
  if (nibble[1] >= 'A' && nibble[1] <= 'F') {
    result += nibble[1] - 'A' + 10;
  } else {
    result += nibble[1] - '0';
  }
  return result;
}
wchar_t *colorAscii(rgbColor g) {
  // dont use twice in one line
  static wchar_t finalcolor[50] = {0};
  swprintf(finalcolor, 20, L"\x1b[38;2;%d;%d;%dm", g.fg[0], g.fg[1], g.fg[2]);
  // #000000 is trasparent
  if (g.bg[0] == 0 && g.bg[1] == 0 && g.bg[2] == 0) {
    swprintf(finalcolor + wcslen(finalcolor), 20, L"\033[0m");
  } else {
    swprintf(finalcolor + wcslen(finalcolor), 20, L"\x1b[48;2;%d;%d;%dm",
             g.bg[0], g.bg[1], g.bg[2]);
  }
  return finalcolor;
}
rgbColor hexC(char fgColor[7], char bgColor[7]) {
  rgbColor resutl;
  assert(*fgColor == '#');
  fgColor += 1;
  resutl.fg[0] = nibbleConvert(fgColor);
  fgColor += 2;
  resutl.fg[1] = nibbleConvert(fgColor);
  fgColor += 2;
  resutl.fg[2] = nibbleConvert(fgColor);

  assert(*bgColor == '#');
  bgColor += 1;
  resutl.bg[0] = nibbleConvert(bgColor);
  bgColor += 2;
  resutl.bg[1] = nibbleConvert(bgColor);
  bgColor += 2;
  resutl.bg[2] = nibbleConvert(bgColor);
  return resutl;
}

typedef struct {
  wchar_t g;
  rgbColor color;
} Cell;
// clang-format off
static const Cell noCell  = {0,     {{0, 0, 0},       {0, 0, 0}}};
static const Cell newLine = {L'\n', {{0, 0, 0},       {0, 0, 0}}};
static const Cell space   = {L' ',  {{255, 255, 255}, {0, 0, 0}}};
static const Cell hor     = {L'─',  {{255, 255, 255}, {0, 0, 0}}};
static const Cell ver     = {L'│',  {{255, 255, 255}, {0, 0, 0}}};
static const Cell tl      = {L'┌',  {{255, 255, 255}, {0, 0, 0}}};
static const Cell bl      = {L'└',  {{255, 255, 255}, {0, 0, 0}}};
static const Cell tr      = {L'┐',  {{255, 255, 255}, {0, 0, 0}}};
static const Cell br      = {L'┘',  {{255, 255, 255}, {0, 0, 0}}};
// clang-format om
Line Line_new(int row, int col, rgbColor color, Direction orientation,
              wchar_t *content) {
  Line a;
  a.row = row;
  a.ori = orientation;
  a.col = col;
  a.color = color;
  a.contents = setAlloc(content);
  return a;
}

void pretty_print_box(const Box box) {
  wprintf(colorAscii(box.defcolor));
  wprintf(L" ## \n");
  wprintf(L"\033[0m");
  wprintf(L"box\n");
  wprintf(L" lines: %p\n"
          L" (%d,%d)\n"
          L" (%d,%d)\n",
          box.lines, box.row, box.col, box.brow, box.bcol);
  fflush(stdout);
}

Box Box_new(int row, int col, int brow, int bcol, rgbColor color,
            wchar_t fill) {
  Box a;
  a.lines = List_new(sizeof(Line));
  for (int i = row; i < brow; i++) {
    for (int o = col; o < bcol; o++) {
      wchar_t stat[2] = {fill, 0};
      Line temp = Line_new(i, o, color, Horizontal, stat);
      List_append(a.lines, &temp);
    }
  }
  a.defcolor = color;
  a.defchar = fill;
  a.row = row;
  a.col = col;
  a.bcol = bcol;
  a.brow = brow;
  return a;
}
void Box_set(Box box, int row, int col, rgbColor color, wchar_t val) {
  if (row >= (box.brow - box.row) || row < 0 || col >= (box.bcol - box.col) ||
      col < 0) {
    return;
  }
  int toedit = row * (box.bcol - box.col) + col;
  ((Line *)List_gst(box.lines, toedit))->color = color;
  ((Line *)List_gst(box.lines, toedit))->contents[0] = val;
}
void Box_reset(Box box, int row, int col) {
  if (row >= (box.brow - box.row) || row < 0 || col >= (box.bcol - box.col) ||
      col < 0) {
    return;
  }
  ((Line *)List_gst(box.lines, row * (box.bcol - box.col) + col))->color =
      box.defcolor;
  ((Line *)List_gst(box.lines, row * (box.bcol - box.col) + col))->contents[0] =
      box.defchar;
}
void Layer_delete(List *l) {
  for (int i = 0; i < l->length; i++) {
    Line *loc = List_gst(l, i);
    free(loc->contents);
  }
  List_delete(l);
}
void stringAppend(List *l, const wchar_t *nTString) {
  int i = wcslen(nTString);
  wchar_t* place =l->head + l->length*sizeof(wchar_t);
  List_resize(l,l->length+i);
  l->length+=i;
  memcpy(place,nTString,i*sizeof(wchar_t));
}
void rasterset(Cell *c, TerminalSize ts, int row, int col, Cell element) {
  if (row >= ts.height || col >= ts.width)
    return;
  if (row < 0 || col < 0)
    return;
  *(c + row * (ts.width + 1) + col) = element;
  return;
}
Cell *Leyer_rasterize(List *l, TerminalSize ts) {
  static List *gridHolder = NULL;
  if (!gridHolder) {
    gridHolder = List_new(sizeof(Cell));
  }
  List_resize(gridHolder, (ts.height + 1) * (ts.width + 1));
  List_zeroOut(gridHolder);
  Cell *grid = gridHolder->head;
  for (int i = 0; i < l->length; i++) {
    Line *line = List_gst(l, i);
    for (int j = 0; line->contents[j]; j++) {
      Cell thisCell;
      thisCell.color = line->color;
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

void printCells(Cell *cellLayer) {
  static List *printCellsBuffer = NULL;
  setCursorPosition(0, 0);
  if (printCellsBuffer == NULL) {
    printCellsBuffer = List_new(sizeof(wchar_t));
  } else {
    printCellsBuffer->length = 0;
  }

  wchar_t prevColor[41] = L"";

  for (int i = 0; cellLayer[i].g; i++) {
    Cell lcell = cellLayer[i];
    wchar_t *colorStr = colorAscii(lcell.color);
    if (wcscmp(colorStr, prevColor) != 0) {
      stringAppend(printCellsBuffer, colorStr);
      wcsncpy(prevColor, colorStr, wcslen(prevColor));
      prevColor[40] = 0;
    }

    List_append(printCellsBuffer, &(lcell.g));
  }

  wchar_t zero = 0;
  List_append(printCellsBuffer, &zero);
  WFPRINT(printCellsBuffer->head);
}
List *LastLRenderScreen = NULL;
Cell *LastLRender = NULL;
void printCellDiff(Cell *cellLayer) {
  int i = 0;
  int row = 1;
  int col = 1;
  while (cellLayer[i].g) {
    Cell current = cellLayer[i];

    if (!$eq(LastLRender[i], current)) {
      setCursorPosition(row, col);
      WFPRINT(colorAscii(current.color));
      // for debugging
      /* rgbColor test = current.color; */
      /* test.bg[2] = rand() % 254; */
      /* WFPRINT(colorAscii(test)); */

      wchar_t literal[2] = {current.g, 0};
      WFPRINT(literal);
    }

    if (current.g == L'\n') {
      row++;
      col = 1;
    } else {
      col++;
    }
    LastLRender[i] = current;
    i++;
  }
  WFPRINT(L"\033[0m");
}

TerminalSize LastTerminalSize = {0, 0};
Cell *bottomLayer(TerminalSize ts) {
  static List *baseLayer = NULL;
  if (!baseLayer) {
    baseLayer = List_new(sizeof(Cell));
  }else if($eq(ts,LastTerminalSize)){
    return baseLayer->head;
  }
  List_resize(baseLayer, (ts.height + 1) * (ts.width + 1));
  Cell *grid = baseLayer->head;
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
void box(List *content) {
  // content is a list of lists who contain layers
  TerminalSize ts = get_terminal_size();
  
  setCursorPosition(0, 0);
  
  Cell *screen = bottomLayer(ts);

  $List_forEach(content,List*,thisContent )
  {
    Cell *lr = Leyer_rasterize(thisContent, ts);
    cellArrMerge(screen, lr);
  }

  if (!BENCHMARK && $eq(LastTerminalSize, ts)) {
    printCellDiff(screen);
  } else {
    printCells(screen);
  }
  fflush(stdout);

  if (!LastLRender) {

    LastLRenderScreen = List_new(sizeof(Cell));
    int i = 0;
    while (screen[i].g) {
      i++;
    }
    List_copyInto(LastLRenderScreen,screen,i+1);
    /* for (int i = 0; screen[i].g; i++) { */
    /*   List_append(LastLRenderScreen, screen + i); */
    /* } */

    LastLRender = LastLRenderScreen->head;
  }else{

    int i = 0;
    while (screen[i].g) {
      i++;
    }
    List_copyInto(LastLRenderScreen,screen,i+1);

    LastLRender = LastLRenderScreen->head;
  }

  LastTerminalSize = ts;
}
