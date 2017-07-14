#ifndef HEAP_H
#define HEAP_H

/** @file heap.h
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Header file of heap.c
 */

#include "misc.h"

/** @brief Define type Heap
 *
 * A heap is supposed to hold elements that are associated with a value 
 * (or priority). 
 * We use this heap as a min heap, providing the user with the element that has smallest value.
 * The elements are supposed to be integer, distinct and between 0 and max.
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

/** @brief Construct the heap.
 *
 * Especially allocate memory for arrays.
 * @param h a heap
 * @param max the max size of the heap.
 * @return modifies the heap h 
 */
void construct_heap(
        Heap *h,
        unsigned int max);

/** @brief Deletes the heap. 
 *
 * Especially frees all the memory.
 * @param h a heap
 * @return modifies the heap h 
 */
void delete_heap(
        Heap *h);

/** @brief Adds an element to the heap.
 *
 * @param h a heap
 * @param vert an element that is to be added.
 * @param value the value (or priority) of the element
 * @return modifies the heap h 
 */
void push(
        Heap *h,
        unsigned int vert,
        unsigned long value);

/** @brief Deletes the element with minimal value and return it.
 *
 * @param h a heap
 * @return the element with smalles value, modifies the heap h 
 */
unsigned int pop(
        Heap *h);

/** @brief Decreases the value of an element.
 *
 * @param h a heap
 * @param vert the element whose value should be decreased
 * @param value the new value of element. should be smaller than the current one.
 * @return modifies the heap h 
 */
void decrease_value(
        Heap *h,
        unsigned int vert,
        unsigned long value);

/** @brief Bubble an element up.
 *
 * @param h a heap
 * @param index the index of the element that is to be moved upwards to its right position.
 * @return modifies the heap h 
 */
void bubble_up(
        Heap *h,
        unsigned int index);

/** @brief Bubble an element down.
 *
 * @param h a heap
 * @param index the index of the element that is to be moved downwards to a feasible position.
 * @return modifies the heap h 
 */
void bubble_down(
        Heap *h,
        unsigned int index);

/** @brief Balances out a pair consisting of child and parent.
 *
 * @param h a heap
 * @param pos_child the index of the child.
 * @param pos_parent the index of the parent.
 * @return if the elements have been swapped, modifies the heap h 
 */
bool balance_pair(
        Heap *h,
        unsigned int pos_child,
        unsigned int pos_parent);

/** @brief Print out information about the heap
 *
 * @param h a heap
 */
void print_heap(
        Heap *h);

#endif 
