#include <stdlib.h>
#include <stdio.h>
struct node;

typedef struct pair {
  char c;
  struct node *next;
} pair;

typedef struct node {
  int data;
  int is_end;
  size_t capacity;
  size_t size;
  pair next[];
} node;

void *xcalloc(size_t size, size_t nmemb) {
  void *tmp = calloc(size, nmemb);
  if (!tmp) {
    perror("Cannot alloc memory");
    exit(1);
  }
  return tmp;
}

node *push_back_next(node *root, pair val) {
  if (root->size + 1 > root->capacity) {
    size_t new_capacity = 2 * (root->size + 1);
    node *tmp = realloc(root, sizeof(node) + new_capacity * sizeof(pair));
    if (!tmp) {
      perror("Cannot alloc memory");                                                                          exit(1); 
    }
    root = tmp;
    root->capacity = new_capacity;
  }
  root->next[root->size] = val;
  root->size++;
  return root;
}

//  root -> [ ] -> struct node {   }
//          main.root
void build(node **root, char *s) {
  if (*s == '\0') return;
  if (*root == NULL) {
    *root = xcalloc(1, sizeof(node));
  }
  for (size_t i = 0; i < (*root)->size; i++) {
    if ((*root)->next[i].c == *s) {
      build(&(*root)->next[i].next, s + 1);
      return;
    }
  }
  pair new_pair = {*s, NULL};
  *root = push_back_next(*root, new_pair);
  build(&(*root)->next[(*root)->size - 1].next, s + 1);
}

void free_tree(node *root) {
  if (!root) return;
  for (size_t i = 0; i < root->size; i++) {
    free_tree(root->next[i].next);
  }
  free(root);
}

int main() {
  node *root = NULL;
  char s[] = "hello";

  build(&root, s);
  free_tree(root);
}
