#include <vector>
#include <utility>

/**
* @class vertex
* @brief a vertex for the dijkstra algorithm
*/
class vertex
{

	std::vector<std::pair<int,int>> edges;				///< first int: edge destination, second int: edge weight
	int prev;							///< the previous vertex in the path			
	long long  dist;						///< the distance from the vertex 1
	bool used;							///< label if the vertex is already used in the dijkstra algorithm
	

public:
	vertex();
	~vertex();



	void setDist(int dist);
	void setPrev(int prev);
	void setUsed(bool used);
	void setEdge(int to, int weight);

	long long getDist();
	int getPrev();
	bool getUsed();
	std::vector<std::pair<int, int>> getEdges();
};

