//
// dijkstra_v2.c
// algorithm-analysis
//
// Created by Erika Matesz Bueno on 08/07/24.
//

#include "dijkstra_v2.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/**
 * @brief Structure to represent a node in the priority queue (min-heap).
 */
typedef struct {
    int vertex; // Vertex number
    int key;    // Distance/key value associated with the vertex
} HeapNode;

/**
 * @brief Structure of the priority queue (min-heap).
 */
typedef struct {
    HeapNode** array;   // Array to store heap nodes
    int size;           // Current size of the heap
    int capacity;       // Maximum capacity of the heap
    int* pos;           // To track the position of vertices in the heap
} MinHeap;

// Function prototypes for heap operations
MinHeap* createMinHeap(int capacity);
HeapNode* createHeapNode(int vertex, int key);
void swapHeapNodes(HeapNode** a, HeapNode** b);
void minHeapify(MinHeap* minHeap, int idx);
HeapNode* extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap, int v, int key);
bool isInMinHeap(MinHeap* minHeap, int v);
bool isEmpty(MinHeap* minHeap);
void freeMinHeap(MinHeap* minHeap);

/**
 * @brief Creates a new min-heap with the specified capacity.
 *
 * @param capacity Maximum number of nodes the heap can hold.
 * @return Pointer to the created MinHeap structure.
 */
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->pos = (int*) malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HeapNode**) malloc(capacity * sizeof(HeapNode*));
    return minHeap;
}

/**
 * @brief Creates a new heap node with the specified vertex and key.
 *
 * @param vertex Vertex number.
 * @param key Distance/key value associated with the vertex.
 * @return Pointer to the created HeapNode structure.
 */
HeapNode* createHeapNode(int vertex, int key) {
    HeapNode* heapNode = (HeapNode*) malloc(sizeof(HeapNode));
    heapNode->vertex = vertex;
    heapNode->key = key;
    return heapNode;
}

/**
 * @brief Swaps two heap nodes.
 *
 * @param a Pointer to the first HeapNode pointer.
 * @param b Pointer to the second HeapNode pointer.
 */
void swapHeapNodes(HeapNode** a, HeapNode** b) {
    HeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Maintains heap property in a min-heap.
 *
 * @param minHeap Pointer to the MinHeap structure.
 * @param idx Index of the node to heapify.
 */
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->key < minHeap->array[smallest]->key)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->key < minHeap->array[smallest]->key)
        smallest = right;

    if (smallest != idx) {
        // Swap nodes in the heap
        HeapNode* smallestNode = minHeap->array[smallest];
        HeapNode* idxNode = minHeap->array[idx];

        // Update positions in the pos[] array
        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;

        // Swap the nodes
        swapHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

/**
 * @brief Extracts the node with the minimum key from the heap.
 *
 * @param minHeap Pointer to the MinHeap structure.
 * @return Pointer to the extracted HeapNode.
 */
HeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node and remove it from the heap
    HeapNode* root = minHeap->array[0];
    HeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update the position of the last node in the pos[] array
    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[lastNode->vertex] = 0;

    // Decrease the size of the heap and maintain heap property
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

/**
 * @brief Decreases the key value of a vertex in the heap.
 *
 * @param minHeap Pointer to the MinHeap structure.
 * @param v Vertex number.
 * @param key New key value for the vertex.
 */
void decreaseKey(MinHeap* minHeap, int v, int key) {
    // Get the index of v in the heap array
    int idx = minHeap->pos[v];

    // Get the node and update its key
    minHeap->array[idx]->key = key;

    // Adjust the heap as necessary
    while (idx && minHeap->array[idx]->key < minHeap->array[(idx - 1) / 2]->key) {
        // Swap with the parent
        minHeap->pos[minHeap->array[idx]->vertex] = (idx - 1) / 2;
        minHeap->pos[minHeap->array[(idx - 1) / 2]->vertex] = idx;
        swapHeapNodes(&minHeap->array[idx], &minHeap->array[(idx - 1) / 2]);

        // Move to the parent
        idx = (idx - 1) / 2;
    }
}

/**
 * @brief Checks if a vertex is in the min-heap or not.
 *
 * @param minHeap Pointer to the MinHeap structure.
 * @param v Vertex number.
 * @return true if the vertex is in the heap, false otherwise.
 */
bool isInMinHeap(MinHeap* minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

/**
 * @brief Checks if the heap is empty.
 *
 * @param minHeap Pointer to the MinHeap structure.
 * @return true if the heap is empty, false otherwise.
 */
bool isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

/**
 * @brief Frees memory allocated for the binary heap.
 *
 * @param minHeap Pointer to the MinHeap structure.
 */
void freeMinHeap(MinHeap* minHeap) {
    for (int i = 0; i < minHeap->size; ++i) { // Using minHeap->size instead of minHeap->capacity
        free(minHeap->array[i]);
    }
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);
}

/**
 * @brief Implements Dijkstra's algorithm v2 for finding shortest paths from a source vertex.
 *
 * @param graph Pointer to the graph structure representing the graph.
 * @param src Source vertex from which shortest paths are computed.
 */
void dijkstraV2(Graph* graph, int src) {
    int numNodes = graph->numNodes;
    int dist[numNodes];
    bool shortestPathTreeSet[numNodes];

    // Initialize distances as INFINITE and shortestPathTreeSet[] as false
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INT_MAX;
        shortestPathTreeSet[i] = false;
    }

    // Distance from source vertex to itself is always 0
    dist[src] = 0;

    // Initialize the priority queue (min-heap)
    MinHeap* minHeap = createMinHeap(numNodes);
    for (int v = 0; v < numNodes; v++) {
        minHeap->array[v] = createHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    minHeap->size = numNodes;

    // Process Dijkstra's algorithm
    while (!isEmpty(minHeap)) {
        HeapNode* heapNode = extractMin(minHeap);
        int u = heapNode->vertex;

        shortestPathTreeSet[u] = true;

        AdjNode* adjNode = graph->adjLists[u];
        while (adjNode != NULL) {
            int v = adjNode->dest;

            if (!shortestPathTreeSet[v] && dist[u] != INT_MAX &&
                dist[u] + adjNode->weight < dist[v]) {
                dist[v] = dist[u] + adjNode->weight;

                // Update the key in the heap
                decreaseKey(minHeap, v, dist[v]);
            }
            adjNode = adjNode->next;
        }
    }
    
    // Print the result: distance from source node (0) to last node
    int last_node_index = numNodes - 1;
    printf("Distance from source node 0 to last node %d: ", last_node_index);
    if (dist[last_node_index] == INT_MAX) {
        printf("INFINITE\n");
    } else {
        printf("%d\n", dist[last_node_index]);
    }

    freeMinHeap(minHeap);
}
