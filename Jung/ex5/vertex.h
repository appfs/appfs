#include <vector>
#include <utility>

class vertex
{

	std::vector<std::pair<int,int>> edges;				//first int: edge destination, second int: edge weight
	int prev;
	long long  dist;
	bool used;
	

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

