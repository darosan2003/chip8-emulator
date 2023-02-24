#ifndef __STACK_H
#define __STACK_H

#include <stdint.h>

#ifndef __INFO__T
#define __INFO__T
typedef uint16_t info_t;
#endif

#ifndef __NODE__T
#define __NODE__T
typedef struct node_t {
	info_t info;
	struct node_t *next;
} node_t;
#endif

#ifndef __STACK__T
#define __STACK__T
typedef node_t * stack_t;
#endif

int create_stack(stack_t *);
int empty_stack(stack_t *);
int insert_stack(stack_t *, info_t);
info_t delete_stack(stack_t *);

#endif
