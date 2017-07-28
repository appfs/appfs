#ifndef HEAP_H
#define HEAP_H


/**
* The heap structure, with the current distances, the values needed in the heap, the previous smallest node, the index, called positionInHeap, since gcc didn't allowed me to use index, and the reversed index.
*/
struct BinaryHeap{
	double* heapVal;//values from which the heap is build
	size_t lastFirst;//last heap position accessed
	unsigned int* positionInHeap;//the heap order in the canonical order
	unsigned int* reversedIndex;//get the position in the heap
};

/**
*A classical stack, with the data, the topElement and the maxSize. It's in arrayform. It saved the parent, too. It is build for graphs.
*/
struct Stack{
    unsigned int* data;
    unsigned int* parents;
    size_t topElement;
    size_t maxSize;
};


/**
* Adds an element to the stack
*@param The new data, the parent and the stack
*@return The stack
*
*/
struct Stack push(unsigned int newData, unsigned int parent,struct Stack stk){
    	assert(stk.topElement <= (stk.maxSize-1));
        ++stk.topElement;
        stk.parents[stk.topElement] = parent;
        stk.data[stk.topElement] = newData;
    	
    	return stk;
}

/**
* Deletes the uppest element
*@param The stack
*@return The stack
*
*/
struct Stack pop(struct Stack stk){
    	assert(stk.topElement > 0);
	--stk.topElement;
    	return stk;
}


/**
* Makes shure everything is still a heap, and in case sorting the elements down
*@param The point where to start, the heap-array and the size of the heap
*@return The heap-array
*
*/
struct BinaryHeap minHeapify(size_t i, struct BinaryHeap heap, size_t size){
    size_t left;
    size_t right;
    size_t smallest;
    //since we start at 0 we need special rules
    if(i != 0)
    {
    	left = 2*i;
    	right = 2*i + 1;
    	smallest = i;
    }
    else
    {
    	left = 1;
    	right = 2;
    	smallest = 0;    	
    }
    //check if left child is smaller
    if (left < size && heap.heapVal[heap.positionInHeap[left]] < heap.heapVal[heap.positionInHeap[smallest]])
    {
        smallest = left;
    }  
    //if right child is smaller
    if (right < size && heap.heapVal[heap.positionInHeap[right]] < heap.heapVal[heap.positionInHeap[smallest]])
    {
        smallest = right;
    }    
    //if one was smaller, update and call it recursive
    if (smallest != i)
    {
    	size_t temp6 = heap.positionInHeap[i];
    	heap.positionInHeap[i] = heap.positionInHeap[smallest];
    	heap.positionInHeap[smallest] = temp6;
    	heap.reversedIndex[heap.positionInHeap[i]] = i;
    	heap.reversedIndex[heap.positionInHeap[smallest]] = smallest;
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
struct BinaryHeap update(size_t startPoint, struct BinaryHeap heap){
	size_t child;
    	size_t father;
    	//need special rules for 0
    	if(startPoint ==0)
    	{
    		return heap;
    	}
    	else
    	{
    		child = startPoint;
    		father = startPoint/2;
    	}
    	//if the partent element is larger push it upwards, reverse call
    	if(heap.heapVal[heap.positionInHeap[child]] < heap.heapVal[heap.positionInHeap[father]])
    	{
    		size_t temp6 = heap.positionInHeap[father];
    		if (father == heap.lastFirst)
    		{
    			heap.lastFirst = child;
    		}
    		heap.positionInHeap[father] = heap.positionInHeap[child];
    		heap.positionInHeap[child] = temp6;
    		heap.reversedIndex[heap.positionInHeap[child]] = child;
    		heap.reversedIndex[heap.positionInHeap[father]] = father;
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
    //bring everything from the buttom to the top into order
    for(size_t i = size/2; i>0; --i){
        heap = minHeapify(i, heap, size);
    } 
    heap = minHeapify(0, heap, size);
    return heap;	        
}        

/**
* Deletes the first element, by making it irrelevant in boulding the heap.
*@param The heap and the size of the heap
*@return The heap array
*
*/
struct BinaryHeap makeFirstIrrelevant(struct BinaryHeap heap, size_t size){
	heap.heapVal[heap.positionInHeap[heap.lastFirst]] = INFINITY;
    	heap = minHeapify(heap.lastFirst, heap, size);
	return heap;
}
#endif
