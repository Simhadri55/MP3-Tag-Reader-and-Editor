#ifndef TYPES_H
#define TYPES_H

/* Status will be used in fn. return type */
typedef enum
{
    success,
    failure
} Status;

typedef enum
{
    view,
    edit,
    unsupported
} OperationType;

#endif



#if 0
typedef enum {
    STATUS_SUCCESS,
    STATUS_FAILURE
} Status;

typedef enum {
    OP_VIEW,
    OP_EDIT,
    OP_UNSUPPORTED
} OperationType;
#endif