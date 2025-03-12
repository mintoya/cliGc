#ifndef LIST_H
#define LIST_H

typedef struct {
  unsigned long width;
  unsigned int length;
  unsigned int size;
  void* head;
} List;

List* List_new(unsigned long bytes);
void List_prettyPrint(const List *list);
void List_set(List* l, unsigned int i,const void* element);
void* List_gst(List* l, unsigned int i);
void List_resize(List *l, unsigned int newSize);
void List_append(List*l,void* element);
void List_insert(List*l,unsigned int i,void*element);
void List_remove(List*l,unsigned int i);
void List_zeroOut(List *l);
void List_delete(List*l);
void List_print_as_string(List* l);
int List_forEach(List *l, int (*function)(void *));
int List_filter(List *l, int (*function)(void *));
#endif // LIST_H
