//
//  graph.c
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

AdjNode* createNode(int dest, int weight) {
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int numNodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numNodes = numNodes;
    graph->numEdges = 0;
    graph->adjLists = (AdjNode**)malloc(numNodes * sizeof(AdjNode*));

    for (int i = 0; i < numNodes; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}


void addEdge(Graph* graph, int src, int dest, int weight) {
    AdjNode* newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    graph->numEdges++;
}

void printGraph(Graph* graph) {
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

