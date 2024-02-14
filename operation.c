#include "operation.h"

#include <stdio.h>
#include <stdlib.h>

Operation *init_op(enum AccessType access_type, unsigned int address) {
    Operation *op = malloc(sizeof(Operation));

    op->access_type = access_type;
    op->address = address;
    op->next = NULL;

    return op;
}

void enqueue(OperationQueue *self, Operation *op)
{
    if (self->size == 0) {
        self->head = op;
        self->tail = op;
    } else {
        self->tail->next = op;
        self->tail = op;
    }

    self->size++;
}

Operation *dequeue(OperationQueue *self)
{
    if (self->size == 0) { return NULL; }

    Operation *op = self->head;
    self->head = self->head->next;
    self->size--;

    return op;
}

OperationQueue *init_operation_queue()
{
    OperationQueue *q = (OperationQueue *)malloc(sizeof(OperationQueue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    
    q->enqueue = &enqueue;
    q->dequeue = &dequeue;
    
    return q;
}

void view_op(Operation *op) {
    printf("MEM_ACCESS: %d | ADDRESS: %X\n", op->access_type, op->address);
}