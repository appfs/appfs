#ifndef HEAP_H
#define HEAP_H

/** @file heap.h
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Header file of heap.c
 */

#include "misc.h"

/** @brief Define type Graph
 */
typedef struct Heap {
    /** number of elements */
    unsigned int n; 
    /** max number of elements */
    unsigned int max;
    /** the heap itself */
    unsigned int *heap;
    /** positions of elems in heap */
    unsigned int *pos;
    /** the values of the elems */
    unsigned long *val;
} Heap;

/** @brief 
 *
 * @param 
 * @return 
 */
void construct_heap(
        Heap *h,
        unsigned int max);

/** @brief 
 *
 * @param 
 * @return 
 */
void delete_heap(
        Heap *h);

/** @brief 
 *
 * @param 
 * @return 
 */
void push(
        Heap *h,
        unsigned int index,
        unsigned long value);

/** @brief 
 *
 * @param 
 * @return 
 */
unsigned int pop(
        Heap *h);

/** @brief 
 *
 * @param 
 * @return 
 */
void decrease_value(
        Heap *h,
        unsigned int index,
        unsigned long value);

/** @brief 
 *
 * @param 
 * @return 
 */
void bubble_up(
        Heap *h,
        unsigned int index);

/** @brief 
 *
 * @param 
 * @return 
 */
void bubble_down(
        Heap *h,
        unsigned int index);

/** @brief 
 *
 * @param 
 * @return 
 */
bool balance_pair(
        Heap *h,
        unsigned int pos_child,
        unsigned int pos_parent);

#endif 
