//
//  main.c
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include "utils.h"
#include "constants.h"
#include "graph.h"
//#include "bellman_ford.h"
#include "bellman_ford_improved.h"
#include "dijkstra_v1.h"
#include "dijkstra_v2.h"

int main(int argc, const char * argv[]) {
    
    int continueProcessing = 1;
    
    while (continueProcessing) {
        printf("Enter 0 to exit or any other number to process a file: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input. Exiting.\n");
            exit(EXIT_FAILURE);
        }
        
        if (choice == 0) {
            continueProcessing = 0;
            break;
        }

        // Choose a .dat or .stp file
        char *file = chooseFile();
        if (file == NULL) {
            fprintf(stderr, ERROR_MSG_SELECT_FILE);
            continue;
        }
        
        // Process the chosen file
        char *temp_file = process_file(file);
        if (temp_file == NULL) {
            fprintf(stderr, ERROR_MSG_PROCESS_FILE);
            free(file);
            continue;
        }

        // Open the .temp file
        FILE *fp = fopen(temp_file, "r");
        if (fp == NULL) {
            perror("Failed to open the temp file");
            free(temp_file);
            free(file);
            continue;
        }

        int numNodes, numEdges;
        fscanf(fp, "NODES %d\n", &numNodes);
        fscanf(fp, "EDGES %d\n", &numEdges);
        fscanf(fp, "EDGE_LIST\n");

        // Create a graph
        Graph* graph = createGraph(numNodes);

        // Add edges to the created graph
        int src, dest, weight;
        while (fscanf(fp, "%d %d %d\n", &src, &dest, &weight) == 3) {
            addEdge(graph, src, dest, weight);
        }

        fclose(fp);
        free(temp_file);
        
        // Print graph's adjacency list - Part 1
        // printf("Graph created:\n");
        // printAdjacencyList(graph);
    
        // Print graph's stats
        GraphStats stats = graphStatistics(graph);

        printf("---> Nodes: %d\n", stats.numNodes);
        printf("---> Edges: %d\n", stats.numEdges);
        printf("---> Total weight: %d\n", stats.totalWeight);
        
        // Run Bellman-Ford algorithm
        printf("\nRunning Bellman-Ford algorithm from source node 0...\n");
        clock_t bf_start = clock();
        bellmanFordImproved(graph, 0, graph->numNodes - 1); // Final implementation
        clock_t bf_end = clock();
        
        double bf_cpu_time = ((double) (bf_end - bf_start)) / CLOCKS_PER_SEC;
        printf("Bellman-Ford algorithm executed in %.6f seconds.\n", bf_cpu_time);
        
        // Run Dijkstra (v1) algorithm
        printf("\nRunning Dijkstra (v1) algorithm from source node 0...\n");
        clock_t d_v1_start = clock();
        dijkstraV1(graph, 0);
        clock_t d_v1_end = clock();
        
        double d_v1_cpu_time = ((double) (d_v1_end - d_v1_start)) / CLOCKS_PER_SEC;
        printf("Dijkstra (v1) algorithm executed in %.6f seconds.\n", d_v1_cpu_time);
        
        // Run Dijkstra (v2) algorithm
        printf("\nRunning Dijkstra (v2) algorithm from source node 0...\n");
        clock_t d_v2_start = clock();
        dijkstraV2(graph, 0);
        clock_t d_v2_end = clock();
        
        double d_v2_cpu_time = ((double) (d_v2_end - d_v2_start)) / CLOCKS_PER_SEC;
        printf("Dijkstra (v2) algorithm executed in %.6f seconds.\n", d_v2_cpu_time);
        
        // theoretical complexity of each algorithm
        int bf_complexity = (int)calculateBellmanFordComplexity(graph->numNodes, stats.numEdges);
        printf("\nTheoretical complexity of Bellman-Ford (improved): O(n x m) --> O(%d)\n", bf_complexity);

        int d_v1_complexity = (int)calculateDijkstraV1Complexity(graph->numNodes);
        printf("\nTheoretical complexity of Dijkstra (v1): O(n^2) --> O(%d)\n", d_v1_complexity);
        
        double d_v2_complexity = calculateDijkstraV2Complexity(graph->numNodes, stats.numEdges);
        printf("\nTheoretical complexity of Dijkstra (v2): O(m * log n) --> O(%.2f)\n", d_v2_complexity);

        // Free the graph memory
        for (int i = 0; i < graph->numNodes; i++) {
            AdjNode* adjList = graph->adjLists[i];
            while (adjList != NULL) {
                AdjNode* temp = adjList;
                adjList = adjList->next;
                free(temp);
            }
        }
        free(graph->adjLists);
        free(graph);

        free(file);
    }

    exit(EXIT_SUCCESS);
}
