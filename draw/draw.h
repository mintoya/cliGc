
#ifndef DRAW_H
#define DRAW_H
#include "../list/list.h"
int begin(void (*layerFactory)(void));
void draw(List *l);
#endif // DRAW_H
