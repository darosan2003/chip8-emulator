#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "stack.h"

int create_stack(stack_t *s) {
    
  if(s == NULL) return -1;
  *s = NULL;
  return 0;

}

int empty_stack(stack_t *s) {
   
  if(s == NULL) return -1;
  return (*s == NULL);
  
}

int insert_stack(stack_t *s, info_t info) {

  node_t *temp;

  if(s == NULL)
    return -1;
  
  if((temp = malloc(sizeof(node_t))) == NULL)
    return -2;

  temp->info = info;
  temp->next = NULL;

  if(empty_stack(s))
    *s = temp;
  else {
    temp->next = *s;
    *s = temp;
  }

  return 0;

}

info_t delete_stack(stack_t *s) {
   
  if(s == NULL || *s == NULL)
    return -1;

  node_t *del = *s;
  info_t info = del->info;

  *s = (*s)->next;
  free(del);

  return info;

}
