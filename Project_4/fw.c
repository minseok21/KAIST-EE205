// EE205 Project 4 fw.c 20190004 Minseok Kang

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define INFINITY INT_MAX

// Function to print the shortest path from vertex 'v' to vertex 'w'
void print_path(int** B, int n, int v, int w) {

    int i = 0;
    int j = 0;
    int path[n];
    
    path[0] = w;

    for(i = 1; i < n - 1; i++) {
        if(B[v][w] == v) {
            path[i] = v;
            break;
        }

        path[i] = B[v][w];
        w = B[v][w];
    }

    if(i == n-1) path[n-1] = v;

    for(j = i; j >= 1; j--)
        printf("%d ", path[j]);
    printf("%d", path[0]);
}

// Floyd-Warshall algorithm for finding shortest paths in a weighted graph
void Floyd_Warshall(int** graph, int V) {
    int **A_i = NULL; // Stores shortest path lengths from the previous iteration
    int **A_f = NULL; // Stores shortest path lengths for the current iteration
    int **B = NULL;   // Stores predecessors on the shortest paths
    int i, v, w;
    int temp;

    // Allocating memory for arrays
    A_i = (int **)malloc((V+1) * sizeof(int *));
    for(i = 0; i <= V; i++) {
        A_i[i] = (int *)malloc((V+1) * sizeof(int));
    }

    // Initialization of arrays with the input graph's weights and predecessors
    A_f = (int **)malloc((V+1) * sizeof(int *));
    for(i = 0; i <= V; i++) {
        A_f[i] = (int *)malloc((V+1) * sizeof(int));
    }

    B = (int **)malloc((V+1) * sizeof(int *));
    for(i = 0; i <= V; i++) {
        B[i] = (int *)malloc((V+1) * sizeof(int));
    }

    // Main loop of the Floyd-Warshall algorithm
    for(v = 1; v <= V; v++) {
        for(w = 1; w <= V; w++) {

            if(graph[v][w] != INFINITY) {
                A_i[v][w] = graph[v][w];
                B[v][w] = v; 
            }
            else if(v == w) {
                A_i[v][w] = 0;
                B[v][w] = -1;
            }
            else {
                A_i[v][w] = INFINITY;
                B[v][w] = -1;
            }
        }
    }

    for(i = 1; i <= V; i++) {
        for(v = 1; v <= V; v++) {
            for(w = 1; w <= V; w++) {

                A_f[v][w] = A_i[v][w];

                if(A_i[v][i] != INFINITY && A_i[i][w] != INFINITY) {
                    temp = A_i[v][i] + A_i[i][w];
                    if(temp < A_f[v][w]) {
                        A_f[v][w] = temp;
                        B[v][w] = B[i][w];
                    }
                }
            }
        }

        // Updating A_i for the next iteration
        for(v = 1; v <= V; v++) {
            for(w = 1; w <= V; w++) {
                A_i[v][w] = A_f[v][w];
            }
        }
    }

    // Check for negative-cost cycles in the graph
    for(v = 1; v <= V; v++) {
        if(A_f[v][v] < 0) {
            printf("Error: negative-cost cycle found\n");
            free(A_i);
            free(A_f);
            free(B);
            exit(0);
        }
    }

    // Printing shortest path lengths and paths for each pair of vertices
    for(v = 1; v <= V; v++) {
        for(w = 1; w <= V; w++) {
            if(v != w) {
                if(A_i[v][w] == INFINITY) 
                    printf("%d %d length: inf path: none", v, w);
                else {
                    printf("%d %d length: %d path: ", v, w, A_f[v][w]);
                    print_path(B, V, v, w);
                }
                printf("\n");
            }
        }
    }

    // Freeing allocated memory
    free(A_i);
    free(A_f);
    free(B);
}

int main(void) {
    int v, e;
    int i, j;
    int s, d, w;
    char line[1024]; // Buffer for each line of input
    int **graph = NULL; 

    // Read the number of vertices and edges
    if (!fgets(line, sizeof(line), stdin) || sscanf(line, "%d %d", &v, &e) != 2) {
        fprintf(stderr, "Error: Invalid input format for number of vertices and edges\n");
        return 1;
    }

    // Initialize the graph with INFINITY
    graph = (int **)malloc((v+1) * sizeof(int *));
    for(i = 0; i <= v; i++) {
        graph[i] = (int *)malloc((v+1) * sizeof(int));
        for(j = 0; j <= v; j++)
            graph[i][j] = INFINITY;
    }

    // Read each edge and populate the graph
    for(i = 0; i < e; i++) {
        if (!fgets(line, sizeof(line), stdin) || sscanf(line, "%d %d %d", &s, &d, &w) != 3) {
            fprintf(stderr, "Error: Invalid input format for edges\n");
            return 1;
        }
        if (s < 1 || s > v || d < 1 || d > v) {
            fprintf(stderr, "Error: Invalid vertex id\n");
            return 1;
        }
        graph[s][d] = w;
    }

    // Execute the Floyd-Warshall algorithm
    Floyd_Warshall(graph, v);

    // Free the allocated memory for the graph
    for(i = 0; i <= v; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
