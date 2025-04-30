#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    void DFS(int src, vector<bool>& visited) {
        visited[src] = true;
        cout << src << " ";
        for (int neighbor : adj[src]) {
            if (!visited[neighbor]) {
                DFS(neighbor, visited);
            }
        }
    }

    void parallelDFS(int src, vector<bool>& visited) {
        stack<int> s;
        s.push(src);

        #pragma omp parallel
        {
            while (true) {
                int node;

                #pragma omp critical
                {
                    if (!s.empty()) {
                        node = s.top();
                        s.pop();
                    } else {
                        node = -1;
                    }
                }

                if (node == -1) break;

                if (!visited[node]) {
                    #pragma omp critical
                    {
                        visited[node] = true;
                        cout << node << " ";
                    }

                    #pragma omp parallel for
                    for (int i = 0; i < adj[node].size(); i++) {
                        int neighbor = adj[node][i];
                        if (!visited[neighbor]) {
                            #pragma omp critical
                            s.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    void BFS(int src) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(src);
        visited[src] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";
            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    void parallelBFS(int src) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(src);
        visited[src] = true;

        while (!q.empty()) {
            int size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int node;

                #pragma omp critical
                {
                    if (!q.empty()) {
                        node = q.front();
                        q.pop();
                    } else {
                        node = -1;
                    }
                }

                if (node != -1) {
                    cout << node << " ";
                    for (int neighbor : adj[node]) {
                        if (!visited[neighbor]) {
                            #pragma omp critical
                            {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
    }
};

/* ------------WITH USER INPUT ---------------

int main() {
    int V = 6;
    Graph g(V);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "Sequential DFS: ";
    vector<bool> visited(V, false);
    g.DFS(0, visited);
    cout << "\n";

    cout << "Parallel DFS: ";
    vector<bool> visited_parallel(V, false);
    g.parallelDFS(0, visited_parallel);
    cout << "\n";

    cout << "Sequential BFS: ";
    g.BFS(0);
    cout << "\n";

    cout << "Parallel BFS: ";
    g.parallelBFS(0);
    cout << "\n";

    return 0;
} 

COMMAND TO RUN : g++ -fopenmp main.cpp -o output
        EXECUTE: .\output

*/

int main() {
    int V = 6;
    Graph g(V);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "Sequential DFS: ";
    vector<bool> visited(V, false);
    g.DFS(0, visited);
    cout << "\n";

    cout << "Parallel DFS: ";
    vector<bool> visited_parallel(V, false);
    g.parallelDFS(0, visited_parallel);
    cout << "\n";

    cout << "Sequential BFS: ";
    g.BFS(0);
    cout << "\n";

    cout << "Parallel BFS: ";
    g.parallelBFS(0);
    cout << "\n";

    return 0;
}
