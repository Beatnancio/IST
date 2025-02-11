#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <queue>

#define IN_OUT 2
#define IN 0
#define OUT 1
#define FOUNTAIN -1
#define SINK -2
#define NIL -3

using namespace std;

typedef struct{
    int next;
    int cap;
} Edge;


class RoadNetwork{
    int numVertex, numAvenues, numStreets;
    int fountain, sink;
    list<Edge> *graph;

public:
    RoadNetwork(int val1, int val2){
        int vertex1;
        int vertex2;
        numAvenues = val1;
        numStreets = val2;                                  //All intersections have an entry point and an exit point, so that only
        numVertex = numAvenues * numStreets * IN_OUT + 2;   //one person can go through that intersection at a time
        fountain = numVertex - 1;
        sink = numVertex - 2;

        graph = new list<Edge>[numVertex + 2];

        for (int i = 0; i < numAvenues; i++){
            for (int j = 0; j < numStreets; j++){           //double for loop that connects all intersections together,                                               //by adding the corresponding edges and the residual edges as well
                vertex1 = getIndex(i, j, IN);               //normal edges start with a capacity of 1, residual edges start with a capacity of 0
                vertex2 = getIndex(i, j, OUT);
                addEdge(vertex1, vertex2, 1);
                addEdge(vertex2, vertex1, 0);
                if (i > 0){
                    vertex1 = getIndex(i - 1, j , IN);
                    addEdge(vertex2, vertex1, 1);
                    addEdge(vertex1, vertex2, 0);
                }

                if (i < numAvenues - 1){
                    vertex1 = getIndex(i + 1, j, IN);
                    addEdge(vertex2, vertex1, 1);
                    addEdge(vertex1, vertex2, 0);
                }

                if (j > 0){
                    vertex1 = getIndex(i, j - 1 , IN);
                    addEdge(vertex2, vertex1, 1);
                    addEdge(vertex1, vertex2, 0);
                }
                
                if (j < numStreets - 1){
                    vertex1 = getIndex(i, j + 1, IN);
                    addEdge(vertex2, vertex1, 1);
                    addEdge(vertex1, vertex2, 0);
                }
            }
        }
    }

    void addEdge(int src, int dest, int fill){
        Edge newEdge;
        newEdge.next = dest;
        newEdge.cap = fill;
        graph[src].push_back(newEdge);
    }

    int getIndex(int avenue, int street, int IO){
        return numStreets* IN_OUT * avenue +
               IN_OUT * street + IO;
    }

    void addSupermarket(int avenue, int street){
        int v = getIndex(avenue-1, street-1, OUT);
        addEdge(v, sink, 1);
        addEdge(sink, v, 0);
    }

    void addHouse(int avenue, int street){
        int v = getIndex(avenue-1, street-1, IN);
        addEdge(fountain, v, 1);
        addEdge(v, fountain, 0);
    }

    void augmentEdges(int v1, int v2){
        list<Edge> :: iterator edge;
        for (edge = graph[v1].begin(); edge != graph[v1].end(); ++edge)
            if (edge->next == v2)
                edge->cap--;
        
        for (edge = graph[v2].begin(); edge != graph[v2].end(); ++edge)
            if (edge->next == v1)
                edge->cap++;
    }

    bool bfs(){
        bool *visited = new bool[numVertex];
        int *prev = new int[numVertex];
        for (int i = 0; i < numVertex; i++){
            prev[i] = NIL;
            visited[i] = false;
        }

        queue<int> q = queue<int>();
        list<Edge> :: iterator adjEdge; 
        int vertex = NIL;
        
        q.push(fountain);
        visited[fountain] = true;

        while (!q.empty()){
            if (vertex == sink)
                break;

            vertex = q.front();
            q.pop();

            
            for (adjEdge = graph[vertex].begin(); adjEdge != graph[vertex].end(); ++adjEdge)
                if (!visited[adjEdge->next] && adjEdge->cap > 0){
                    
                    visited[adjEdge->next] = true;
                    prev[adjEdge->next] = vertex;
                    if (adjEdge->next == sink){
                        vertex = sink;
                        break;
                    }
                    q.push(adjEdge->next);
                }
        }
        
        if (vertex != sink)
            return false;
        else{
            while (vertex != fountain){
                augmentEdges(prev[vertex], vertex);
                vertex = prev[vertex];
            }
            return true;
        }
        free(prev);
        free(visited);
    }

    int edmondsKarp(){
        int maxFlow = 0;
        list<int> :: iterator it;
        while(bfs())
            maxFlow++;
        return maxFlow;
    }
};

int main(){
    int totalAvenues, totalStreets;
    int numHouses, numSupermarkets;
    int avenue, street;
 
    //gather graph info
    cin >> totalAvenues;
    cin.ignore();
    cin >> totalStreets;
    RoadNetwork network = RoadNetwork(totalAvenues, totalStreets);

    //gather house and supermarket locations
    cin >> numSupermarkets;
    cin.ignore();
    cin >> numHouses;
    for (int i = 0; i < numSupermarkets; i++){   
        cin >> avenue;
        cin.ignore();
        cin >> street;
        network.addSupermarket(avenue, street);
    }

    for (int i = 0; i < numHouses; i++){
        cin >> avenue;
        cin.ignore();
        cin >> street;
        network.addHouse(avenue, street);
    }
    // out = max number of citizens that can go to a supermarket without encountering another citizen
    int out = network.edmondsKarp();
    cout << out << endl;
    return 0;
}
