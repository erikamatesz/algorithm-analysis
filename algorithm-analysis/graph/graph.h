//
//  graph.h
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#ifndef graph_h
#define graph_h

#include <stdio.h>

typedef struct AdjNode {
    int dest;
    int weight;
    struct AdjNode* next;
} AdjNode;

typedef struct Graph {
    int numNodes;
    int numEdges;
    AdjNode** adjLists;
} Graph;

typedef struct {
    int numNodes;
    int numEdges;
    int totalWeight;
} GraphStats;

AdjNode* createNode(int dest, int weight);
Graph* createGraph(int numNodes);
void addEdge(Graph* graph, int src, int dest, int weight);
void printAdjacencyList(Graph* graph);
GraphStats graphStatistics(Graph* graph);

#endif
