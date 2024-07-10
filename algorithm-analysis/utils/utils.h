//
//  utils.h
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>

char* chooseFile(void);
char* process_file(const char *input_filename);
double calculateBellmanFordComplexity(int n, int m);
double calculateDijkstraV1Complexity(int n);
double calculateDijkstraV2Complexity(int n, int m);

#endif /* utils_h */
