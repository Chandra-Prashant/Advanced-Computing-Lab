#include <iostream>
#include <limits.h>
#include <iomanip>
using namespace std;
#define MAX 20
// ANSI color codes
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
int n;
int graph[MAX][MAX], rGraph[MAX][MAX];
int parent[MAX], visited[MAX];
int source = -1, sink = -1;
bool graphEntered = false;

void line() {
    cout << CYAN << "--------------------------------------------------\n" << RESET;
}

void convertToFlowNetwork() {
    if (source == -1 || sink == -1) return;
    for (int i = 0; i < n; i++) {
        graph[i][source] = 0; //rm incoming to src
        graph[sink][i] = 0; //rm outgoing from snk
    }
    cout << YELLOW << "System: Undirected graph converted to (s,t) Flow Network.\n" << RESET;
}

void printResidual() {
    cout << YELLOW << "\n Residual Matrix (Intermediate State):\n" << RESET;
    cout << "    ";
    for (int i = 0; i < n; i++) cout << setw(4) << i; // clm indices
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(4) << i; //rw indices
        for (int j = 0; j < n; j++) {
            if (rGraph[i][j] > 0) cout << GREEN << setw(4) << rGraph[i][j] << RESET;
            else cout << RED << setw(4) << rGraph[i][j] << RESET;
        }
        cout << endl;
    }
}

int bfs(int s, int t) {
    int q[MAX], front = 0, rear = 0;
    for (int i = 0; i < n; i++) visited[i] = 0;    
    q[rear++] = s;
    visited[s] = 1;
    parent[s] = -1;
    while (front < rear) {
        int u = q[front++];
        for (int v = 0; v < n; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                q[rear++] = v;
                parent[v] = u;
                visited[v] = 1;
                if (v == t) return 1;
            }
        }
    }
    return 0;
}

void fordFulkerson(int s, int t) {
    int maxFlow = 0, step = 1;    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            rGraph[i][j] = graph[i][j];
    line();
    cout << YELLOW << " STARTING MAX FLOW CALCULATION\n" << RESET;
    line();
    while (bfs(s, t)) {
        int pathFlow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, rGraph[u][v]);
        }
        cout << BLUE << "\n[ STEP " << step++ << " ]\n" << RESET;
        int tempPath[MAX];
        int pathIdx = 0;
        for (int v = t; v != -1; v = parent[v]) {
            tempPath[pathIdx++] = v;
        }
        cout << " Augmenting Path: ";
        for (int i = pathIdx - 1; i >= 0; i--) {
            cout << GREEN << tempPath[i] << (i == 0 ? "" : " -> ") << RESET;
        }
        cout << "\n Path Capacity (Bottleneck): " << RED << pathFlow << RESET << endl;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= pathFlow;
            rGraph[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
        printResidual();
    }
    line();
    cout << GREEN << " FINAL MAXIMUM FLOW = " << maxFlow << RESET << endl;
    line();
}

int main() {
    int choice;
    do {
        line();
        cout << YELLOW << " FORD-FULKERSON MENU\n" << RESET;
        line();
        cout << "1. Input Graph (Undirected)\n";
        cout << "2. Set Source & Sink (Converts to Flow Network)\n";
        cout << "3. Run Ford-Fulkerson\n";
        cout << "4. Exit\n";
        line();
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            cout << "Enter number of vertices (max " << MAX << "): ";
            cin >> n;
            if (n > MAX) {
                cout << RED << "Error: Exceeds MAX capacity.\n" << RESET;
                continue;
            }
            for(int i=0; i<n; i++) for(int j=0; j<n; j++) graph[i][j] = 0;
            int edges;
            cout << "Enter number of edges: ";
            cin >> edges;
            cout << "Enter edges (u v capacity):\n";
            for (int i = 0; i < edges; i++) {
                int u, v, w;
                cin >> u >> v >> w;
                if (u < n && v < n) {
                    graph[u][v] = w;
                    graph[v][u] = w;
                }
            }
            graphEntered = true;
            source = -1; sink = -1;
        }
        else if (choice == 2) {
            if (!graphEntered) cout << RED << "Input graph first (Option 1)!\n" << RESET;
            else {
                cout << "Enter Source index: "; cin >> source;
                cout << "Enter Sink index: "; cin >> sink;
                if (source >= 0 && source < n && sink >= 0 && sink < n && source != sink) {
                    convertToFlowNetwork();
                } else {
                    cout << RED << "Invalid indices or Source = Sink!\n" << RESET;
                    source = -1; sink = -1;
                }
            }
        }
        else if (choice == 3) {
            if (source == -1 || sink == -1) cout << RED << "Specify Source & Sink (Option 2)!\n" << RESET;
            else fordFulkerson(source, sink);
        }
    } while (choice != 4);
    return 0;
}