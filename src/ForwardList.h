#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <stdlib.h>
#include <dbg.h>


struct ForwardListNode{
    void *data;
    struct ForwardListNode *next;
}ForwardListNode;

typdef struct{
    size_t count;
    ForwardListNode *first;
}ForwardList;

#endif
