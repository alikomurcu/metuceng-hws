#include "the6.h"

class Graph
{
    int S; // the # of vertices of the graph

    std::vector<int>* adj;
    
    bool flag = true;

    public:
    Graph(int S);

    void addEdge(int key1, int key2);

    bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()

    std::pair<bool, std::vector<int>> topo_sort();


    bool DFS(int v);

    std::pair<bool, std::vector<int>> returnCycle();

    int *mama, *contvec;
    int theN, cycle_end;
};

Graph::Graph(int S)
{
    this->S = S;

    adj = new std::vector<int>[S]; // this is adjaceny list for each vertice. i.e., array of vectors which adj[i] -> implies an adjacency list of the vertice i.
    mama = new int[S];
    contvec = new int[S];
    for(int i=0; i<S; i++) 
    {
        mama[i] = -1;
        contvec[i] = -1;
    }
    
}

void Graph::addEdge(int key1, int key2)
{
    adj[key1].push_back(key2);
}

std::pair<bool, std::vector<int>> Graph::topo_sort()
{
    int indegree[S] = {0};
    for(int i=0; i<S; i++)
        for(auto j: adj[i])
            indegree[j]++;

    std::queue<int> Q;
    for (int i = 0; i < S; i++)
        if (indegree[i] == 0)
            Q.push(i);
 
    int c = 0;
 
    std::vector<int> result;
    
    while (Q.size()>0) {
        int u = Q.front();
        Q.pop();
        result.push_back(u);

        for(auto j: adj[u])
            if (--indegree[j] == 0)
                Q.push(j);

        c++;
    }
 
    if (c != S) // Check if cycle exists
    {
        flag = false;
        return {flag, {}};
    }

    std::pair <bool, std::vector<int>> res (flag, result);
    return res;
}

bool Graph::DFS(int v) {
    contvec[v] = 1;
    for (int u : adj[v]) {
        if (contvec[u] == 0) {
            mama[u] = v;
            if (DFS(u))
                return true;
        } else if (contvec[u] == 1) {
            cycle_end = v;
            theN = u;
            return true;
        }
    }
    contvec[v] = 2;
    return false;
}

std::pair<bool, std::vector<int>> Graph::returnCycle() {
    theN = -1;

    for (int v = 0; v < S; v++) {
        if (contvec[v] == 0 && DFS(v))
            break;
    }

    std::vector<int> cycle;
    cycle.push_back(theN);
    for (int v = cycle_end; v != theN; v = mama[v])
        cycle.push_back(v);
    cycle.push_back(theN);
    std::stack<int> S;
    std::vector<int> V;
    
    for(auto i:cycle) S.push(i);
    while(!S.empty())
    {
        V.push_back(S.top());
        S.pop();
    }

        return {false, V};
}


std::pair<bool, std::vector<int>>
RaceResult(int N, std::vector<std::pair<int, int>> updates)
{
    Graph G(N);
    for(auto i:updates)
    {
        G.addEdge(i.first, i.second);
    }

    std::pair <bool, std::vector<int>> res = G.topo_sort();
    std::vector<int> myvec = res.second, resvec;
    if(!res.first)
        return G.returnCycle();

    return res; // This is a dummy return value. YOU SHOULD CHANGE THIS!

}
/*
int main()
{
    int N = 10;
    updates=[{0, 5}, {1, 0}, {1, 7}, {2, 4}, {2, 0}, {3, 4}, {3, 5}, {3, 7}, {3, 6}, {4, 7}, {4, 1}, {6, 2}, {6, 5}, {6, 9}, {7, 9}, {7, 5}, {7, 3}, {8, 3}];
    std::vector<int> out = RaceResult(N, updates).second;

    for(i:out) cout << i << ", ";
}*/