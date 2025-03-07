#include <assert.h>
#include <locale.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

typedef struct rgbColor {
  uint8_t fg[3];
  uint8_t bg[3];
} rgbColor;

uint8_t nibbleConvert(const wchar_t *nibble) {
  uint8_t result;
  if (nibble[0] >= L'A' && nibble[0] <= L'F') {
    result = (uint8_t)(nibble[0] - L'A' + 10);
  } else {
    result = (uint8_t)(nibble[0] - L'0');
  }
  result <<= 4;
  if (nibble[1] >= L'A' && nibble[1] <= L'F') {
    result += (uint8_t)(nibble[1] - L'A' + 10);
  } else {
    result += (uint8_t)(nibble[1] - L'0');
  }
  return result;
}

wchar_t *colorize(rgbColor g) {
  static wchar_t finalcolor[50] = {0};
  swprintf(finalcolor, 50, L"\x1b[38;2;%d;%d;%dm", g.fg[0], g.fg[1], g.fg[2]);
  swprintf(finalcolor + wcslen(finalcolor), 50 - wcslen(finalcolor),
           L"\x1b[48;2;%d;%d;%dm", g.bg[0], g.bg[1], g.bg[2]);
  return finalcolor;
}

rgbColor hexC(const wchar_t *fgColor, const wchar_t *bgColor) {
  rgbColor result;
  assert(*fgColor == L'#');
  fgColor += 1;
  result.fg[0] = nibbleConvert(fgColor);
  fgColor += 2;
  result.fg[1] = nibbleConvert(fgColor);
  fgColor += 2;
  result.fg[2] = nibbleConvert(fgColor);

  assert(*bgColor == L'#');
  bgColor += 1;
  result.bg[0] = nibbleConvert(bgColor);
  bgColor += 2;
  result.bg[1] = nibbleConvert(bgColor);
  bgColor += 2;
  result.bg[2] = nibbleConvert(bgColor);
  return result;
}

void prettyPrintRgb(rgbColor a) {
  wprintf(L"%s\n", colorize(a));
  wprintf(L"#####\x1b[0m \n");
}

int main(void) {
  setlocale(LC_ALL, "");
  prettyPrintRgb(hexC(L"#000000", L"#00DDFF"));
  prettyPrintRgb(hexC(L"#FFDDFF", L"#0000FF"));
  return 0;
}
