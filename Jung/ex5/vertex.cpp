#include "vertex.h"
#include<climits>


vertex::vertex()
{
	
	dist = 2000000000000;
	used = false;
	prev = -1;
	
}


vertex::~vertex()
{
}





void vertex::setDist(int dist){
	vertex::dist = dist;
}

void vertex::setPrev(int prev){
	vertex::prev = prev;
}

void vertex::setUsed(bool used){
	vertex::used = used;
}

void vertex::setEdge(int to, int weight){
	edges.push_back(std::pair<int, int>(to, weight));
}

long long vertex::getDist(){
	return vertex::dist;
}

int vertex::getPrev(){
	return vertex::prev;
}

bool vertex::getUsed(){
	return vertex::used;
}

std::vector<std::pair<int, int>> vertex::getEdges(){
	return edges;
}
