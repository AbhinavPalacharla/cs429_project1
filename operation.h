#ifndef OPERATION_H_
#define OPERATION_H_

enum AccessType
{
    DATA_READ,
    DATA_WRITE,
    INSTRUCTION_READ
};

typedef struct _Operation
{
    enum AccessType access_type;
    unsigned int address;
    struct _Operation *next;
} Operation;

typedef struct _OperationQueue
{
    Operation *head;
    Operation *tail;
    int size;

    void (*enqueue)(struct _OperationQueue *self, Operation *op);
    Operation *(*dequeue)(struct _OperationQueue *self);
} OperationQueue;

Operation *init_op(enum AccessType access_type, unsigned int address);

void view_op(Operation *op);

OperationQueue *init_operation_queue();

#endif