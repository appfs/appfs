


#include "dijkstra.h"


//typedegs for edges
typedef std::pair<int, int> Edge;


void bubbleDown(int* heap, int* whereInHeap, long long* dist,int nodePos, int &heapLength){


	int node= heap[nodePos];
	int value= dist[node];
	int smallChildPos;	

	bool down=true;

	while(down){

			down=false;

			if(dist[heap[nodePos*2]]<dist[heap[nodePos*2+1]]){
				smallChildPos=nodePos*2;
			}
			else{
				smallChildPos=nodePos*2+1;
			}

			if(dist[heap[smallChildPos]]<value){

				heap[nodePos]=heap[smallChildPos];
				whereInHeap[heap[smallChildPos]]=nodePos;
				nodePos=smallChildPos;

					down=true;
			}
	}

	heap[nodePos]=node;
	whereInHeap[node]=nodePos;

}


// Bubble up one entry in the heap until no smaller parent is there
void bubbleUp(int* heap, int* whereInHeap, long long* dist,int &nodePos){

	int node= heap[nodePos];
	int value= dist[node];
	int bubblePos=nodePos;

	bool up=true;

	while(up){

		int parentPos=bubblePos/2;
		if(dist[heap[parentPos]]>value){

			heap[bubblePos]=heap[parentPos];
			whereInHeap[heap[parentPos]]=bubblePos;
			bubblePos=parentPos;

		}
		else{

			heap[bubblePos]=node;
			whereInHeap[node]=bubblePos;
			up=false;
		}
	}

}


// Bubble down one entry in the heap until no smaller child is there
void update(int* heap,int* whereInHeap,long long* dist, int* pre, std::vector<Edge> &steinEdges, bool* steinTree,int node,int &count){

	int parent=pre[node];


	while(!steinTree[parent]){

		// save Edge
		steinEdges.push_back(Edge(node,parent));
		steinTree[parent]=true;

		// put node in heap with distance 0
		dist[parent]=0;
		heap[count]=parent;
		whereInHeap[parent]=count;
		count++;
		bubbleUp(heap,whereInHeap, dist, whereInHeap[parent]);

		node=parent;
		parent=pre[node];

	}

		steinEdges.push_back(Edge(node,parent));


}

/**
* \brief algorithm,to find the steiner Tree which is mostly the dijkstra algorithm
*
*
*
* @param edges: The Edges of the given graph
* @param steinTree: the nodes of the graph
* @param pre: The predecessor map
* @param vertNumb: Number of vertexes
* @param start: The start node
* @param steinEdges: vector to write the edges of the steiner Tree
* @isPrime: the Terminals
* @primeNumb: number of terminals
* @weightST: weight of the steiner Tree
**/

void dijkstra(Edg &edges, bool* steinTree, int* pre, int &vertNumb,
			int start, std::vector<Edge> &steinEdges, int &heapLength, bool* isPrime,int primeNumb, long long &weightST){


	int* heap= new int[heapLength]();
	int* whereInHeap=new int[vertNumb]();		//determine from the node the position in the heap
	long long *dist = new long long[vertNumb]();


	for(int i=0;i<vertNumb;i++){

		dist[i]=INT_MAX;
		heap[i]=vertNumb-1;
		whereInHeap[i]=0;
	}

	for(int i=vertNumb;i<heapLength;i++){
		heap[i]=vertNumb-1;
		//heap[i]=INT_MAX;
	}

	////////// The start node
	heap[0]=0;
	heap[1]=start;
	whereInHeap[start]=1;
	dist[0]=-1;
	dist[start]=0;

	int count=2;				// points to the next free place in the heap

	int node;
	int toNode;				// node has edge to toNode
	int toWeight;				// the weight of the edge from node to toNode	
	int pos;				// position in the edges array
	int value;				// The distance of the first node in the Heap
	int help;

	//////////////////// Dijkstra


	for(int k=0;k<primeNumb;k++){

		bool searchNext=true;

		while(searchNext){

			node=heap[1];

			// Found a Terminal
			if(isPrime[node]){

				weightST=weightST+dist[heap[1]];

				//set distance 0
				dist[node]=0;

				update(heap, whereInHeap, dist, pre, steinEdges, steinTree, node, count);
				searchNext=false;
				steinTree[node]=true;
				isPrime[node]=false;
			}
			else{

				value=dist[heap[1]];
				count--;

				//remove old node from the Heap
				whereInHeap[node]=0;

				// We need this if count==1
				help=heap[count];
				whereInHeap[heap[count]]=1;
				heap[count]=vertNumb-1;
				heap[1]=help;

				bubbleDown(heap,whereInHeap,dist,1,heapLength);

				int end=edges.size(node);
				pos=edges.nodes[node];

				// update the Neighbors
				for(int i=0; i<end; i++){

					toNode=edges.edges[pos+i];
					toWeight=edges.weights[pos+i];;

					if(toWeight+value<dist[toNode]){

						dist[toNode]=toWeight+value;

						if(whereInHeap[toNode]==0){

							heap[count]=toNode;
							whereInHeap[toNode]=count;
							count++;
						}

						bubbleUp(heap,whereInHeap,dist,whereInHeap[toNode]);
						pre[toNode]=node;

					}
				}
			}
		}
	}


	delete[] heap;
	delete[] dist;
	delete[] whereInHeap;


}



