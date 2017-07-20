/** @file heap.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Short c file containing min heap functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

    bubble_up(h, h->n -1);
}

unsigned int pop(
        Heap *h) {

    assert(h->n > 0);
    unsigned int root = h->heap[0];
    if(h->n == 1) {
        h->n = 0;
        return root;
    }

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
	
    assert(h->pos[vert] < h->n);
    assert(h->val[vert] > value);
    h->val[vert] = value;
    bubble_up(h, h->pos[vert]);
}

void bubble_up(
        Heap *h, 
        unsigned int index) {

    assert(index < h->n);
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

    assert(index < h->n);
    unsigned int pos_parent = index;
    bool result = true;

    while (result && h->n > 2*pos_parent + 1) {
        unsigned int pos_child1 = 2*pos_parent + 1;
        unsigned int pos_child2 = 2*pos_parent + 2;
        unsigned long val_child1 = h->val[h->heap[pos_child1]];
        unsigned long val_child2 = h->val[h->heap[pos_child2]];
        unsigned long val_parent = h->val[h->heap[pos_parent]];

        bool gt_both = (val_parent > val_child1 && val_parent > val_child2);
        bool only_gt_child2 = (val_parent > val_child2 && val_parent <= val_child1);
        bool child1_gt_child2 = (val_child1 > val_child2);
        
        if ((gt_both && child1_gt_child2) || only_gt_child2) { 
            result = balance_pair(h, pos_child2, pos_parent);
            pos_parent = pos_child2;
        } else {
            result = balance_pair(h, pos_child1, pos_parent);
            pos_parent = pos_child1;
        }
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

void print_heap(
        Heap *h) {

    printf("heap has length %d\n", h->n);
    for (int i = 0; i < h->n; i++) { 
        printf("pos %d: elem %d, val %li\n", 
                i, 
                h->heap[i]+1, 
                h->val[h->heap[i]] ); 
    }

}
