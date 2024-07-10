//
//  dijkstra_v1.c
//  algorithm-analysis
//
//  Created by Erika Matesz Bueno on 08/07/24.
//

#include "dijkstra_v1.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/**
 * @brief Find the vertex with the minimum distance value, from the set of vertices
 * not yet included in the shortest path tree.
 *
 * @param dist An array storing the shortest distance from the source to each vertex.
 * @param shortestPathTreeSet Boolean array indicating whether vertex i is included in
 * the shortest path tree or not.
 * @param numNodes Number of vertices in the graph.
 * @return The index of the vertex with the minimum distance value.
 */
int minDistance(int dist[], bool shortestPathTreeSet[], int numNodes) {
    int min = INT_MAX, min_index = 0;

    for (int v = 0; v < numNodes; v++) {
        if (!shortestPathTreeSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

/**
 * @brief Implements Dijkstra's algorithm (v1) to find shortest paths from a source vertex src to all
 * other vertices in the graph represented by 'graph'.
 *
 * @param graph Pointer to the graph structure.
 * @param src Source vertex from which shortest paths are to be calculated.
 */
void dijkstraV1(Graph* graph, int src) {
    int numNodes = graph->numNodes;
    int dist[numNodes];
    bool shortestPathTreeSet[numNodes];

    // Initialize all distances as INFINITE and shortestPathTreeSet[] as false
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INT_MAX;
        shortestPathTreeSet[i] = false;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < numNodes - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, shortestPathTreeSet, numNodes);

        // Mark the picked vertex as processed
        shortestPathTreeSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        AdjNode* adjNode = graph->adjLists[u];
        while (adjNode != NULL) {
            int v = adjNode->dest;

            // Update dist[v] only if it's not in shortestPathTreeSet, there is an edge
            // from u to v, and total weight of path from src to v through u is smaller
            if (!shortestPathTreeSet[v] && dist[u] != INT_MAX && dist[u] + adjNode->weight < dist[v]) {
                dist[v] = dist[u] + adjNode->weight;
            }
            adjNode = adjNode->next;
        }
    }
    
    // Print the result: distance from the source node (0) to the last node
    int last_node_index = numNodes - 1;
    printf("Distance from the source node 0 to the last node %d: ", last_node_index);
    if (dist[last_node_index] == INT_MAX) {
        printf("INFINITE\n");
    } else {
        printf("%d\n", dist[last_node_index]);
    }
}
