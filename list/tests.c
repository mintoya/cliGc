#include "list.h" // Assuming the header is in the same directory
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef struct {
  char name[50];
  int age;
} Person;

// Helper function to print and check the person
int print_person(void *p) {
  Person *person = (Person *)p;
  printf("Name: %s, Age: %d\n", person->name, person->age);
  return 1;
}

// Helper function to compare people
int compare_person(void *p1, void *p2) {
  Person *person1 = (Person *)p1;
  Person *person2 = (Person *)p2;
  /* printf("comparing...\n"); */
  /* print_person(person1); */
  /* print_person(person2); */
  int res =
      !strcmp(person1->name, person2->name) && person1->age == person2->age;
  /* printf((res) ? ("they are the same\n") : ("they are different\n")); */
  return res;
}

void test_List_new_and_append() {
  List *list = List_new(sizeof(Person));
  Person person1 = {"John Doe", 30};
  Person person2 = {"Jane Smith", 25};

  // Append persons to the list
  List_append(list, &person1);
  List_append(list, &person2);

  // Check if the list has the correct length and data
  Person *retrieved_person1 = (Person *)List_gst(list, 0);
  Person *retrieved_person2 = (Person *)List_gst(list, 1);

  // Assert the data is correct
  assert(retrieved_person1 != NULL);
  assert(compare_person(retrieved_person1, &person1));

  assert(retrieved_person2 != NULL);
  assert(compare_person(retrieved_person2, &person2));

  printf("Test List_new_and_append passed!\n");

  // Clean up
  List_delete(list);
}

void test_List_set_and_get() {
  List *list = List_new(sizeof(Person));
  Person person1 = {"John Doe", 30};
  Person person2 = {"Jane Smith", 25};

  List_append(list, &person1);
  List_append(list, &person2);

  Person newPerson = {"Alice Brown", 28};
  // Set new person in position 1
  List_set(list, 1, &newPerson);

  Person *retrieved_person = (Person *)List_gst(list, 1);
  assert(retrieved_person != NULL);
  assert(!compare_person(retrieved_person, &newPerson));

  printf("Test List_set_and_get passed!\n");

  List_delete(list);
}

void test_List_remove() {
  List *list = List_new(sizeof(Person));
  Person person1 = {"John Doe", 30};
  Person person2 = {"Jane Smith", 25};
  Person person3 = {"Alice Brown", 28};

  List_append(list, &person1);
  List_append(list, &person2);
  List_append(list, &person3);

  // Remove person at index 1
  List_remove(list, 1);

  // Check if the list is correct after removal
  Person *retrieved_person1 = (Person *)List_gst(list, 0);
  Person *retrieved_person2 = (Person *)List_gst(list, 1);

  assert(retrieved_person1 != NULL);
  assert(compare_person(retrieved_person1, &person1));

  assert(retrieved_person2 != NULL);
  assert(compare_person(retrieved_person2, &person3));

  printf("Test List_remove passed!\n");

  List_delete(list);
}

void test_List_resize() {
  List *list = List_new(sizeof(Person));
  Person person1 = {"John Doe", 30};
  Person person2 = {"Jane Smith", 25};

  List_append(list, &person1);
  List_append(list, &person2);

  // Resize list to a larger size
  List_resize(list, 5);

  // Append more items to the list
  Person person3 = {"Alice Brown", 28};
  Person person4 = {"Bob White", 35};

  List_append(list, &person3);
  List_append(list, &person4);

  // Check the size of the list
  assert(list->size == 5);

  // Check the values in the list
  Person *retrieved_person3 = (Person *)List_gst(list, 2);
  Person *retrieved_person4 = (Person *)List_gst(list, 3);

  assert(retrieved_person3 != NULL);
  assert(compare_person(retrieved_person3, &person3));

  assert(retrieved_person4 != NULL);
  assert(compare_person(retrieved_person4, &person4));

  printf("Test List_resize passed!\n");

  List_delete(list);
}

void test_List_forEach() {
  List *list = List_new(sizeof(Person));
  Person person1 = {"John Doe", 30};
  Person person2 = {"Jane Smith", 25};

  List_append(list, &person1);
  List_append(list, &person2);

  // Print each person in the list
  List_forEach(list, print_person);

  printf("Test List_forEach passed!\n");

  List_delete(list);
}
int filterHelper(void *elem) {
  Person person1 = {"John Doe", 30};
  return (!compare_person(elem, &person1));
}
void test_List_filter() {
  List *list = List_new(sizeof(Person));
  Person person1 = {"John Doe", 30};
  Person person2 = {"Jane Smith", 25};
  List_append(list, &person1);
  List_append(list, &person1);
  List_append(list, &person1);
  List_append(list, &person1);
  List_append(list, &person1);
  List_append(list, &person2);

  // Print each person in the list
  printf("before filter\n");
  List_forEach(list, print_person);
  List_filter(list, filterHelper);
  printf("after filter\n");
  List_forEach(list, print_person);
  assert(list->length == 1);
  printf("Test List_filter passed!\n");

  List_delete(list);
}

int main() {
  test_List_new_and_append();
  test_List_set_and_get();
  test_List_remove();
  test_List_resize();
  test_List_forEach();
  test_List_filter();

  printf("All tests passed!\n");
  return 0;
}
