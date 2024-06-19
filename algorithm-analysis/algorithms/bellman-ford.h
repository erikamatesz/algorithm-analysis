//
//  bellman-ford.h
//  algorithm-analysis
//
//  Created by Erika Matesz Bueno on 19/06/24.
//

#ifndef bellman_ford_h
#define bellman_ford_h

#include <stdio.h>
#include "graph.h"

/**
 * @brief Executes the Bellman-Ford algorithm to find the shortest path
 * from a source node to a destination node in a given graph.
 *
 * @param graph Pointer to the graph
 * @param src The source node
 * @param dest The destination node
 */
void bellmanFord(Graph* graph, int src, int dest);

#endif /* bellman_ford_h */
