/** @file heap.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Short c file containing min heap functionality
 */

#include <stdlib.h>
#include <assert.h>
#include "misc.h"
#include "heap.h"

void construct_heap(
        Heap *h,
        unsigned int max) {
    
    h->n = 0;
    h->max = max;
    h->heap = malloc(sizeof(h->heap) * max);
    h->pos = malloc(sizeof(h->pos) * max);
    h->val = malloc(sizeof(h->val) * max);
}

void delete_heap(
        Heap *h) {

    free(h->heap);
    free(h->pos);
    free(h->val);
}

void push(
        Heap *h,
        unsigned int vert,
        unsigned long value) {

    assert(h->n < h->max);

    h->heap[h->n] = vert;
    h->pos[vert] = h->n;
    h->val[vert] = value;

    h->n = h->n + 1;

    bubble_up(h, vert);
}

unsigned int pop(
        Heap *h) {

    unsigned int root = h->heap[0];
    unsigned int last_elem = h->heap[h->n - 1];
    h->heap[0] = last_elem;
    h->pos[last_elem] = 0;

    h->n = h->n - 1;
    
    bubble_down(h, 0);
    return root;
}

void decrease_value(
        Heap *h,
        unsigned int vert,
        unsigned long value) {

    assert(vert < h->n);
    h->val[vert] = value;
    bubble_up(h, vert);
}

void bubble_up(
        Heap *h, 
        unsigned int index) {

    assert(index<h->n);
    unsigned int pos_child = index;
    bool result = true;

    while (result && pos_child != 0) {
        unsigned int pos_parent = ((pos_child+1)/2)-1;

        result = balance_pair(h, pos_child, pos_parent);

        pos_child = pos_parent;
    }
}

void bubble_down(
        Heap *h, 
        unsigned int index) {

    assert(index<h->n);
    unsigned int pos_parent = index;
    bool result = true;

    while (result && h->n > 2*pos_parent + 1) {
        unsigned int pos_child1 = 2*pos_parent + 1;
        unsigned int pos_child2 = 2*pos_parent + 2;
        
        unsigned int pos_child;
        if (h->n > pos_child2 && h->val[h->heap[pos_child1]] > h->val[h->heap[pos_child2]]) {
            pos_child = pos_child1;
        } else {
            pos_child = pos_child2;
        }

        balance_pair(h, pos_child, pos_parent);

        pos_parent = pos_child;
    }
}

bool balance_pair(
        Heap *h,
        unsigned int pos_child,
        unsigned int pos_parent) {

    unsigned int child = h->heap[pos_child];
    unsigned int parent = h->heap[pos_parent];
    if (h->val[parent] > h->val[child]) {
        h->heap[pos_parent] = child;
        h->heap[pos_child] = parent;
        h->pos[child] = pos_parent;
        h->pos[parent] = pos_child; 
        return true;
    } else {
        return false;
    }
}
