//
//  main.c
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"
#include "constants.h"
#include "graph.h"

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

        char *file = chooseFile();
        if (file == NULL) {
            fprintf(stderr, ERROR_MSG_SELECT_FILE);
            continue;
        }
        
        char *temp_file = process_file(file);
        if (temp_file == NULL) {
            fprintf(stderr, ERROR_MSG_PROCESS_FILE);
            free(file);
            continue;
        }

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

        Graph* graph = createGraph(numNodes);

        int src, dest, weight;
        while (fscanf(fp, "%d %d %d\n", &src, &dest, &weight) == 3) {
            addEdge(graph, src, dest, weight);
        }

        fclose(fp);
        free(temp_file);
        
        printf("Graph created:\n");
        printGraph(graph);
    
        GraphStats stats = graphStatistics(graph);

        printf("---> Nodes: %d\n", stats.numNodes);
        printf("---> Edges: %d\n", stats.numEdges);
        printf("---> Total weight: %d\n", stats.totalWeight);

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
