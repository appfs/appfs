


#include "dijkstra.h"



void bubbleDown(int* heap, int* whereInHeap, int* whichNode,int nodePos, int heapLength){


	int value= heap[nodePos];
	int node= whichNode[nodePos];

	int smallChildPos;	

	bool down=true;

	while(down){



			down=false;


			if(heap[nodePos*2]<heap[nodePos*2+1]){
				smallChildPos=nodePos*2;
			}
			else{
				smallChildPos=nodePos*2+1;
			}

			if(heap[smallChildPos]<value){

				heap[nodePos]=heap[smallChildPos];
				whichNode[nodePos]=whichNode[smallChildPos];
				whereInHeap[whichNode[nodePos]]=nodePos;
				nodePos=smallChildPos;	


				if(smallChildPos*2<=heapLength){	//TODO to avoid that comparison (~O(nlogn) per dijkstra) implement heap with heaplength 2^i with 2^(i-1)<=numbNodes<=2^i
					down=true;
				}
			}
		
	}

	heap[nodePos]=value;
	whichNode[nodePos]=node;
	whereInHeap[node]=nodePos;

}



void bubbleUp(int* heap, int* whereInHeap, int* whichNode,int nodePos){

	int value= heap[nodePos];
	int node= whichNode[nodePos];
	int bubblePos=nodePos;

	bool up=true;

	while(up){

		int parentPos=bubblePos/2;
		if(heap[parentPos]>value){

			heap[bubblePos]=heap[parentPos];
			whichNode[bubblePos]=whichNode[parentPos];
			whereInHeap[whichNode[parentPos]]=bubblePos;
			bubblePos=parentPos;

		}
		else{

			heap[bubblePos]=value;
			whichNode[bubblePos]=node;
			whereInHeap[node]=bubblePos;
			up=false;
		}
	}

}



int dijkstra(std::vector<Edge_wW>* edges, bool* steinTree, bool* used, int* pre, int vertNumb, 
			int restNodes, std::vector<int> &steinerNodes, int heapLength){



	#if BIG
		int* heap=new int[heapLength]();
		int* whereInHeap = new int[vertNumb]();		//determine from the node the position in the heap
		int* whichNode = new int[heapLength]();		//determine from the position of the heap which node
	#else

		int heap[heapLength];
		int whereInHeap[vertNumb];		//determine from the node the position in the heap
		int whichNode[heapLength];		//determine from the position of the heap which node
	#endif


	for(int i=0;i<vertNumb;i++){

		heap[i]=INT_MAX;
		whereInHeap[i]=-1;
	}

	for(int i=restNodes+1;i<heapLength;i++){
		heap[i]=INT_MAX;
		//heap[i]=INT_MAX;
	}



	///////////////////// Fill in the heap

	
	int count=0;
	whichNode[0]=-1;
	heap[0]=-1;
	count++;				// actual nodes in the heap

	int node;
	int end=steinerNodes.size();
	int toNode;				// node has edge to toNode
	int toWeight;			// the weight of the edge from node to toNode	
	for(int i=1;i<end;i++){
		
		node=steinerNodes[i];
		int size=edges[node].size();
		for(int j=0;j<size;j++){


			if(!steinTree[(edges[node])[j].first]){

				toNode=edges[node][j].first;
				toWeight= edges[node][j].second;


				if(whereInHeap[toNode]==-1){

					whereInHeap[toNode]=count;
					heap[whereInHeap[toNode]]=toWeight;
					whichNode[count]=toNode;
					pre[toNode]=node;
					count++;
				}
				else if( toWeight<heap[whereInHeap[toNode]]){

					heap[whereInHeap[toNode]]=toWeight;
					pre[toNode]=node;								//TODO omit?

				}
			}
		}
	}


	///////////////////// Sort the heap

	int where=count/2;
	int value;



	for(;where>0;where--){


		bubbleDown(heap,whereInHeap,whichNode,where,restNodes);	//TODO vertNumb


	}


	//////////////////// Dijkstra

	where=restNodes;

	while(true){

		if(whichNode[1]==vertNumb-1){			

			
			#if BIG
				int sol=heap[1];
				delete[] heap;
				delete[] whichNode;
				delete[] whereInHeap;
				return sol;
			#endif
				return heap[1];

			
		}
		else{

			node=whichNode[1];
			value=heap[1];
			used[node]=true;
			count--;

			heap[1]=heap[count];
			whichNode[1]=whichNode[count];
			whereInHeap[whichNode[count]]=1;
			whichNode[count]=-1;
			heap[count]=INT_MAX;


			if( count>1){		//TODO improve
				
				bubbleDown(heap,whereInHeap,whichNode,1,restNodes);			//TODO why its not working for last argument=restNodes
			}		
			end=edges[node].size();



			for(int i=0; i<end; i++){





				toNode=edges[node][i].first;			//TODO node is sometime -1
				toWeight=edges[node][i].second;


				if(!used[toNode]){

					if(whereInHeap[toNode]==-1){

						pre[toNode]=node;
						whereInHeap[toNode]=count;
						whichNode[count]=toNode;
						heap[count]=toWeight+value;

						bubbleUp(heap, whereInHeap, whichNode,count);
						count++;

					}
					else if(toWeight+value<heap[whereInHeap[toNode]]){

						pre[toNode]=node;
						heap[whereInHeap[toNode]]=toWeight+value;
						bubbleUp(heap, whereInHeap, whichNode, whereInHeap[toNode]);
					}
				}
			}
		}
	}
}



