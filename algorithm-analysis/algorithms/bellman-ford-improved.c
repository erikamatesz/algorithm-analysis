//
//  bellman-ford-improved.c
//  algorithm-analysis
//
//  Created by Erika Matesz Bueno on 19/06/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bellman-ford-improved.h"

/**
 * @brief Initializes the distance and predecessor arrays.
 *
 * @param graph Pointer to the graph
 * @param src The source node
 * @param dist Array to store the shortest distance from the source node
 */
void improvedInitializeSingleSource(Graph* graph, int src, int* dist) {
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
}

/**
 * @brief Relaxes an edge and updates the distance array.
 *
 * @param u The source node of the edge
 * @param v The destination node of the edge
 * @param weight The weight of the edge
 * @param dist Array to store the shortest distance from the source node
 */
void improvedRelax(int u, int v, int weight, int* dist, int* newDist) {
    if (dist[u] != INT_MAX && dist[u] + weight < newDist[v]) {
        newDist[v] = dist[u] + weight;
    }
}

/**
 * @brief Implements the Bellman-Ford algorithm to find the shortest path
 * from a source node to a destination node in a given graph.
 *
 * @param graph Pointer to the graph
 * @param src The source node
 * @param dest The destination node
 */
void bellmanFordImproved(Graph* graph, int src, int dest) {
    int numNodes = graph->numNodes;
    int* dist = (int*) malloc(numNodes * sizeof(int));
    int* newDist = (int*) malloc(numNodes * sizeof(int));

    improvedInitializeSingleSource(graph, src, dist);

    // Relax all edges numNodes - 1 times
    for (int i = 1; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            newDist[j] = dist[j];
        }
        for (int u = 0; u < numNodes; u++) {
            AdjNode* node = graph->adjLists[u];
            while (node) {
                improvedRelax(u, node->dest, node->weight, dist, newDist);
                node = node->next;
            }
        }
        int* temp = dist;
        dist = newDist;
        newDist = temp;
    }

    // Check for negative weight cycles
    for (int u = 0; u < numNodes; u++) {
        AdjNode* node = graph->adjLists[u];
        while (node) {
            if (dist[u] != INT_MAX && dist[u] + node->weight < dist[node->dest]) {
                printf("Graph contains a negative weight cycle\n");
                free(dist);
                free(newDist);
                return;
            }
            node = node->next;
        }
    }

    // Print the shortest distance between the source and destination nodes
    printf("Shortest distance from node %d to node %d: ", src, dest);
    if (dist[dest] == INT_MAX) {
        printf("Infinity (no path)\n");
    } else {
        printf("%d\n", dist[dest]);
    }

    free(dist);
    free(newDist);
}
