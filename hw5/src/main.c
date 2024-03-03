#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char empty_str[] = {0x0};

struct node {
  int64_t value;
  struct node *next;
};

struct node *add_element(int64_t value, struct node *next) {
  struct node *n = (struct node *)malloc(16);
  if (n == NULL) {
    return NULL;
  }

  n->value = value;
  n->next = next;

  return n;
}

void print_int(int64_t value) { printf("%lld ", value); }

void m(struct node *head, void (*fn)(int64_t)) {
  if (head == NULL) {
    return;
  }

  fn(head->value);
  m(head->next, fn);
}

bool p(int64_t value) { return value % 2 != 0; }

void f(struct node *head, struct node **ptr, bool(is_odd)(int64_t)) {
  if (head == NULL) {
    return;
  }

  if (is_odd(head->value)) {
    *ptr = add_element(head->value, *ptr);
  }

  f(head->next, ptr, is_odd);
}

int main() {
  int64_t arr[] = {4, 8, 15, 16, 23, 42};
  size_t data_lenght = sizeof(arr) / sizeof(arr[0]);

  struct node *head = NULL;

  for (int i = data_lenght - 1; i >= 0; i--) {
    head = add_element(arr[i], head);
    if (head == NULL) {
      return EXIT_FAILURE;
    }
  }

  struct node *tmp = head;
  m(tmp, print_int);

  puts(empty_str);

  struct node *new_ptr = NULL;
  f(head, &new_ptr, p);
  m(new_ptr, print_int);

  puts(empty_str);
}
