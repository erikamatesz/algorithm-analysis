//
//  graph.c
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Creates a new node.
 * @param dest The destination node.
 * @param weight The weight of the edge.
 * @return A pointer to the newly created adjacency node.
 */
AdjNode* createNode(int dest, int weight) {
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

/**
 * @brief Creates a new graph with a specified number of nodes.
 * @param numNodes The number of nodes in the graph.
 * @return A pointer to the newly created graph.
 */
Graph* createGraph(int numNodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numNodes = numNodes;
    graph->adjLists = (AdjNode**)malloc(numNodes * sizeof(AdjNode*));

    for (int i = 0; i < numNodes; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

/**
 * @brief Adds an edge to the graph.
 * @param graph The graph to which the edge will be added.
 * @param src The source node of the edge.
 * @param dest The destination node of the edge.
 * @param weight The weight of the edge.
 */
void addEdge(Graph* graph, int src, int dest, int weight) {
    AdjNode* newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    graph->numEdges++;
}

/**
 * @brief Prints the adjacency lists of the graph.
 *
 * This function iterates over each node in the graph and prints its
 * adjacency list. Each adjacency list is represented as a list of
 * destination nodes and the corresponding edge weights.
 *
 * @param graph The graph whose adjacency lists are to be printed.
 *
 * The output format for each node is:
 * Node <node_index>: [(dest1, weight1), (dest2, weight2), ...]
 *
 * Example:
 * Node 0: [(1, 10), (2, 5)]
 * Node 1: [(3, 2)]
 * Node 2: []
 * Node 3: [(0, 7)]
 */
void printAdjacencyList(Graph* graph) {
    int numEdges = 0;

    for (int i = 0; i < graph->numNodes; i++) {
        AdjNode* adjList = graph->adjLists[i];
        printf("Node %d: [", i);
        int first = 1; // flag to handle comma placement
        while (adjList) {
            if (!first) {
                printf(", ");
            }
            printf("(%d, %d)", adjList->dest, adjList->weight);
            adjList = adjList->next;
            numEdges++;
            first = 0; // no longer the first element
        }
        printf("]\n");
    }
}

/**
 * @brief Calculates and returns the statistics of the graph.
 * @param graph The graph for which statistics will be calculated.
 * @return A GraphStats structure containing the number of nodes, number of edges, and the total weight of the graph.
 */
GraphStats graphStatistics(Graph* graph) {
    GraphStats stats;
    stats.numNodes = graph->numNodes;
    stats.numEdges = 0;
    stats.totalWeight = 0;
    
    for (int i = 0; i < graph->numNodes; i++) {
        AdjNode* adjList = graph->adjLists[i];
        while (adjList) {
            stats.numEdges++;
            stats.totalWeight += adjList->weight;
            adjList = adjList->next;
        }
    }
    return stats;
}
