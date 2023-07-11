#include<stack>
#include<queue>
#include<vector>
#include<climits>
#include<iostream>
using namespace std;

struct Road {
  std::pair<int, int> endpoints;
  int time;
  Road(std::pair<int, int> eps, int t) : endpoints(eps), time(t) {}
};


template <class T> struct greater {
  bool operator() (const T& x, const T& y) const {return x>y;}
  typedef T first_argument_type;
  typedef T second_argument_type;
  typedef bool result_type;
};
class Graph
{
    int S; // the # of vertices of the graph

    int** adj;  // adjacency matrix
    //std::vector<int>*  adj;
    public:
    Graph(int S);
	~Graph();
	std::stack<int> path;
	int pathCost = 0;
    void addEdge(int key1, int key2, int weight);
	void Djikstra(int s, int e);

};

Graph::Graph(int S)
{
    this->S = S;
    adj = new int*[S];
    for(int i=0; i<S; i++)
    {
      adj[i] = new int[S];
	  for(int j=0; j<S; j++) adj[i][j] = 0;  // initialize to zero
    }
}

Graph::~Graph()
{
	for(int i=0; i<S; i++) delete adj[i];
	delete[] adj;
}

void Graph::addEdge(int key1, int key2, int weight)
{
	adj[key1][key2] = weight;  // initialize adjacency matrix
	adj[key2][key1] = weight;  // 
}

void Graph::Djikstra(int s, int e)
{
	//while(!path.empty()) path.pop();  //clear the path
	std::stack<int> ipath;
    std::priority_queue< std::pair<int,int>, std::vector<std::pair<int,int>> , greater<std::pair<int,int>> > pq;
    std::vector<int> distance(S, INT_MAX);
	std::vector<int> previous(S, -1);
	std::vector<bool> visited(S, false);

	//struct Pair p = Pair(0, s);  // first one is weight, second one is source vertice
	std::pair<int,int> p(0, s);
    pq.push(p);
    distance[s] = 0;
	previous[s] = -1;
    while (!pq.empty())
    {
        int v = pq.top().second;
        pq.pop();
  
		visited[v] = true; // current node is visited
		for(int j = 0; j<S; j++)
		{
			if(adj[v][j] != 0 && !visited[j])
			{
				int w = j;  // adj to v
				int weight = adj[v][j];  // weight of (v,w)
				if(distance[w] > distance[v] + weight)
				{
					distance[w] = distance[v] + weight;
					std::pair<int,int> p(distance[w], w);
					pq.push(p);
					previous[w] = v;
				}
			}
		}
    }

	// Now distances[e] is the distance between start and end
	// To find the path, traverse back through previous.
	int i = e;
	while(previous[i] != -1)
	{
		ipath.push(i);
		i = previous[i];
	}
	ipath.push(i);
	//path.push(s);  // add s to stack
	pathCost = distance[e];  // the total cost
	this->path = ipath;
}

template <class ForwardIt>
void PrintRange(ForwardIt first, ForwardIt last, char const *delim = "->") {
  --last;
  for (; first != last; ++first) {
    std::cout << *first << delim;
  }
  std::cout << *first;
}

void FindRoute(int n, std::vector<Road> roads, int s, int d, int x, int y) {
	// You can change these variables. These are here for demo only.
	Graph G(n);
	for(auto el: roads)
	{
		G.addEdge(el.endpoints.first, el.endpoints.second, el.time);
	}
	int cost1 = 0;
	/// First state:
	G.Djikstra(s,x);  // Go to X
	cost1 += G.pathCost;
	std::vector<int> path1;
	while(!G.path.empty())
	{
		path1.push_back(G.path.top());
		G.path.pop();
	}
	G.Djikstra(x,y);  // Then Y
	cost1 += G.pathCost;
	G.path.pop(); // delete X because it is already in the path after first Djikstra.
	while(!G.path.empty())
	{
		path1.push_back(G.path.top());
		G.path.pop();
	}
	G.Djikstra(y,d);  // Lastly D
	cost1 += G.pathCost;
	G.path.pop(); // delete Y because it is already in the path after first Djikstra.
	while(!G.path.empty())
	{
		path1.push_back(G.path.top());
		G.path.pop();
	}
	//std::stack<int> firstStack (G.path);
//////////////////////////////////////////////////////////////////////////////////////////
	int cost2 = 0;
	// Second state:
	G.Djikstra(s,y);  // Go to Y
	cost2 += G.pathCost;
	std::vector<int> path2;
	while(!G.path.empty())
	{
		path2.push_back(G.path.top());
		G.path.pop();
	}
	G.Djikstra(y,x);  // Then X
	cost2 += G.pathCost;
	G.path.pop(); // delete Y because it is already in the path after first Djikstra.
	while(!G.path.empty())
	{
		path2.push_back(G.path.top());
		G.path.pop();
	}
	G.Djikstra(x,d);  // Lastly D
	cost2 += G.pathCost;
	G.path.pop(); // delete Y because it is already in the path after first Djikstra.
	while(!G.path.empty())
	{
		path2.push_back(G.path.top());
		G.path.pop();
	}
//////////////////////////////////////////////////////////////////////////////////////////////////
	int cost = cost1<cost2 ? cost1: cost2;
	std::vector<int> path = cost1 < cost2 ? path1 : path2;

	// Write your code here...

	// Your output should be like this. You can change this as long as you keep
	// the output format. PrintRange function helps you print elements of
	// containers with iteratos (e.g., std::vector).
	std::cout << cost << " ";
	PrintRange(path.begin(), path.end());
	std::cout << std::endl;
}

int main()
{
	Graph G(9);
    //vector< std::pair<int,int> > adj[V];
  
    // making above shown graph
	std::vector<Road> roads = {{{0, 1}, 13}, {{0, 2}, 13}, {{0, 3}, 18}, {{1, 2}, 19}, {{1, 3}, 10}, {{1, 4}, 18}, {{2, 3}, 13}, {{2, 4}, 20}, {{3, 4}, 18}};
	FindRoute(5,roads,3,2,1,4);
/*    G.addEdge(0, 1, 13);
    G.addEdge(0, 7, 8);
    G.addEdge(1, 2, 8);
    G.addEdge(1, 7, 11);
	//G.addEdge(1, 6, 1);
    G.addEdge(2, 3, 7);
    G.addEdge(2, 8, 2);
    G.addEdge(2, 5, 4);
    G.addEdge(3, 4, 9);
    G.addEdge(3, 5, 14);
    G.addEdge(4, 5, 10);
    G.addEdge(5, 6, 2);
    G.addEdge(6, 7, 1);
    G.addEdge(6, 8, 6);
    G.addEdge(7, 8, 7);
  
	G.Djikstra(1, 8);
	cout << G.pathCost << endl;
	while(!G.path.empty())
	{
		cout << G.path.top() << " ";
		G.path.pop();
	}*/
    return 0;
}