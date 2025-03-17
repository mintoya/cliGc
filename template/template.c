#include "template.h"
#include "../tSize/terminal_size.h"
#include <unistd.h>
#include <wchar.h>
Rational Rational_add(Rational a, Rational b) {
  Rational result;
  if (a.denominator == b.denominator) {
    result.denominator = a.denominator;
    result.numerator = a.numerator + b.numerator;
  } else {
    result.denominator = a.denominator * b.denominator;
    result.numerator =
        a.numerator * b.denominator + b.numerator * a.denominator;
  }
  return result;
}
Rational Rational_subtract(Rational a, Rational b) {
  b.numerator *= -1;
  return Rational_add(a, b);
}
Rational Rational_multiply(Rational a, Rational b) {
  Rational result;
  result.numerator = a.numerator * b.numerator;
  result.denominator = a.denominator * b.denominator;
  return result;
}
Rational Rational_divide(Rational a, Rational b) {
  int temp = b.numerator;
  b.numerator = b.denominator;
  b.denominator = temp;
  return Rational_multiply(a, b);
}
int Rational_toInt(Rational a) { return a.numerator / a.denominator; }
Node solidify(Node parent, Node n) {
  Node result = n;

  if (!result.size.dcol.isDefined)
    result.size.dcol.n = Rational_multiply(n.size.dcol.n, parent.size.dcol.n);

  if (!result.size.drow.isDefined)
    result.size.drow.n = Rational_multiply(n.size.drow.n, parent.size.drow.n);

  if (!result.position.dcol.isDefined)
    result.position.dcol.n =
        Rational_add(Rational_multiply(n.position.dcol.n, parent.size.dcol.n),
                     parent.position.dcol.n); // Corrected here

  if (!result.position.drow.isDefined)
    result.position.drow.n =
        Rational_add(Rational_multiply(n.position.drow.n, parent.size.drow.n),
                     parent.position.drow.n); // Corrected here

  return result;
}
Box fromNode(Node *n) {
  int start_row = Rational_toInt(n->position.drow.n);
  int start_col = Rational_toInt(n->position.dcol.n);
  int end_row = start_row + Rational_toInt(n->size.drow.n);
  int end_col = start_col + Rational_toInt(n->size.dcol.n);
  Box result = Box_new(start_row, start_col, end_row, end_col, n->color, L' ');
  return result;
}
List *renderNodes(List *result, Node *start) {
  if (!result)
    result = List_new(sizeof(List));
  List_append(result, fromNode(start).lines);
  if (start->children != NULL) {
    for (int i = 0; i < start->children->length; i++) {
      Node element = *((Node *)List_gst(start->children, i));
      Node solid = solidify(*start, element);
      renderNodes(result, &solid);
    }
  }
  return result;
}
