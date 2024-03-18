// EE205 Project 4 bf.c 20190004 Minseok Kang

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define INFINITY INT_MAX

// Function to print the shortest path from source vertex 's' to vertex 'v'
void print_path(int* B, int s, int v) {
    if(v == s) {
        printf("%d", s);
        return;
    }

    print_path(B, s, B[v]);
    printf(" ");
    printf("%d", v);
}

// Bellman-Ford algorithm to find shortest paths from a source vertex 's'
void Bellman_Ford(int** graph, int V, int s) {
    int *A_i = NULL; // Array to store distances from the source in the previous iteration
    int *A_f = NULL; // Array to store distances from the source in the current iteration
    int *B = NULL;   // Array to store predecessors on the shortest paths
    int i, v, w;
    int stable;
    int temp;

    // Allocating memory for arrays
    A_i = (int *)malloc((V+1) * sizeof(int));
    A_f = (int *)malloc((V+1) * sizeof(int));
    B = (int *)malloc((V+1) * sizeof(int));

    // Initialization of arrays
    for(v = 1; v <= V; v++) {
        if(v == s) A_i[v] = 0; // Distance from source to itself is 0
        else A_i[v] = INFINITY; // Initialize distances to INFINITY
        
        B[v] = -1; // Initialize predecessors to -1 (undefined)
    }

    // Main loop of the Bellman-Ford algorithm
    for(i = 1; i <= V; i++) {
        stable = 1; // Flag to check if any distance was updated

        // Relaxation step
        for(v = 1; v <= V; v++) {
            A_f[v] = A_i[v];

            for(w = 1; w <= V; w++) {
                if(graph[w][v] != INFINITY && A_i[w] != INFINITY) {
                    temp = A_i[w] + graph[w][v];
                    if(temp < A_f[v]) {
                        A_f[v] = temp;
                        B[v] = w;
                        stable = 0;
                    }
                }
            }
        }

        // Early stopping if no update in this iteration
        if(stable) {
            for(v = 1; v <= V; v++) {
                if(v != s) {
                    if(A_i[v] == INFINITY) 
                        printf("%d %d length: inf path: none", s, v);
                    else {
                        printf("%d %d length: %d path: ", s, v, A_i[v]);
                        print_path(B, s, v);
                    }
                    printf("\n");
                }
            }
            free(A_i);
            free(A_f);
            free(B);
            return;
        }
        
        // Update distances for the next iteration
        for(v = 1; v <= V; v++) {
            A_i[v] = A_f[v];
        }
    }

    // If the algorithm did not converge, it indicates a negative cycle
    printf("Error: negative-cost cycle found\n");
    free(A_i);
    free(A_f);
    free(B);
    exit(0);
}


int main(int argc, char *argv[]) {
    int v, e;
    int i, j;
    int s, d, w;
    int source = 0;
    int **graph = NULL; 

    // Reading graph input
    char line[1024];
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

    // Execute the Bellman-Ford algorithm
    if(argc > 2) {
        fprintf(stderr, "Error: Too many arguments provided\n");
        return 1;
    } else if(argc == 2) {
        source = atoi(argv[1]);
        if(source < 1 || source > v) {
            fprintf(stderr, "Error: Invalid source vertex\n");
            return 1;
        }
        Bellman_Ford(graph, v, source);
    } else {
        for(i = 1; i <= v; i++) {
            Bellman_Ford(graph, v, i);
        }
    }

    // Freeing allocated memory
    for(i = 0; i <= v; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
