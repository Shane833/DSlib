#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <stdlib.h>
#include <dbg.h>

typedef struct ForwardListNode ForwardListNode;

ForwardListNode{
    void *data;
    ForwardListNode *next;
};

typdef struct{
    size_t count;
    ForwardListNode *first;
}ForwardList;

#endif
