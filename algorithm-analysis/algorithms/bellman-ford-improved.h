//
//  bellman-ford-improved.h
//  algorithm-analysis
//
//  Created by Erika Matesz Bueno on 19/06/24.
//

#ifndef bellman_ford_improved_h
#define bellman_ford_improved_h

#include <stdio.h>
#include "graph.h"

/**
 * @brief Executes the Bellman-Ford algorithm to find the shortest path
 * from a source node to a destination node in a given graph.
 * This is the memory-improved version.
 *
 * @param graph Pointer to the graph
 * @param src The source node
 * @param dest The destination node
 */
void bellmanFordImproved(Graph* graph, int src, int dest);

#endif /* bellman_ford_improved_h */
