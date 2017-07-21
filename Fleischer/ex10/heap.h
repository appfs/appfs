#ifndef HEAP_H
#define HEAP_H

#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h> 
#include<assert.h>
#include<ctype.h>
#include <fenv.h>
#include <time.h>
#include <sys/time.h>
#include<omp.h>

/**
* The heap structure, with the current distances, the values needed in the heap, the previous smallest node, the index, called index66, since gcc didn't allowed me to use index, and the reversed index.
*/
struct BinaryHeap{
	double* dist;
	double* heapVal;
	size_t lastFirst;
	size_t* index66;
	size_t* reversedIndex;
};


/**
* Makes shure everything is still a heap.
*@param The point where to start, the heap-array and the size of the heap
*@return The heap-array
*
*/

struct BinaryHeap minHeapify(size_t i, struct BinaryHeap heap, size_t size){
    size_t left;
    size_t right;
    size_t smallest;
    
    if(i != 0){
    	left = 2*i;
    	right = 2*i + 1;
    	smallest = i;
    }
    else{
    	left = 1;
    	right = 2;
    	smallest = 0;    	
    }
    
    if (left < size && heap.heapVal[heap.index66[left]] < heap.heapVal[heap.index66[smallest]]){
        smallest = left;
    }  
    
    if (right < size && heap.heapVal[heap.index66[right]] < heap.heapVal[heap.index66[smallest]]){
        smallest = right;
    }    
    
    if (smallest != i){
    	size_t temp6 = heap.index66[i];
    	heap.index66[i] = heap.index66[smallest];
    	heap.index66[smallest] = temp6;
    	heap.reversedIndex[heap.index66[i]] = i;
    	heap.reversedIndex[heap.index66[smallest]] = smallest;
        heap = minHeapify(smallest, heap, size);
    }  

    return heap;   
}        

/**
* Check if still heap for updated element.
*@param The start index of the point where to update and the heap-array 
*@return The heap array
*
*/
struct BinaryHeap update(size_t i, struct BinaryHeap heap){
	size_t child;
    	size_t father;
    	if(i ==0){
    		return heap;
    	}
    	else{
    		child = i;
    		father = i/2;
    	}
    	
    	if(heap.heapVal[heap.index66[child]] < heap.heapVal[heap.index66[father]]){
    		size_t temp6 = heap.index66[father];
    		if (father == heap.lastFirst){
    			heap.lastFirst = child;
    		}
    		heap.index66[father] = heap.index66[child];
    		heap.index66[child] = temp6;
    		heap.reversedIndex[heap.index66[child]] = child;
    		heap.reversedIndex[heap.index66[father]] = father;
        	heap = update(father, heap);
    	}
    	return heap;
}

/**
* Builds the heap.
*@param The heap and the size of the heap
*@return The heap array
*
*/

struct BinaryHeap buildMinHeap(struct BinaryHeap heap, size_t size){
    
    for(size_t i = size/2; i>0; --i){
        heap = minHeapify(i, heap, size);
    } 
    heap = minHeapify(0, heap, size);
    return heap;	        
}        

/**
* Deletes the first element.
*@param The heap and the size of the heap
*@return The heap array
*
*/

struct BinaryHeap popHeap(struct BinaryHeap heap, size_t size){
	heap.heapVal[heap.index66[heap.lastFirst]] = INFINITY;
    	heap = minHeapify(heap.lastFirst, heap, size);
	return heap;
}
#endif
