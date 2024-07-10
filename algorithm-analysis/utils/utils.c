//
//  utils.c
//  algorithm-analysis
//
//  Created by Erika Cristina Matesz Bueno on 18/04/24.
//

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "constants.h"

/**
 * @brief Selects a file from the given directory path.
 *
 * Reads the files from the specified directory path and allows the user to select one of them.
 *
 * @param path The path of the directory containing the files.
 * @return A dynamically allocated string representing the selected file's path.
 *         Returns NULL if an error occurs or if the user's choice is invalid.
 */
char* selectFileFromFileList(const char *path) {
    DIR *dir;
    struct dirent *directory_entry;
    int fileNumber = 1;

    printf("\nReading files from directory:\n%s\n\n", path);

    if ((dir = opendir(path)) != NULL) {
        while ((directory_entry = readdir(dir)) != NULL) {
            if (strstr(directory_entry->d_name, ".dat") != NULL || strstr(directory_entry->d_name, ".stp") != NULL) {
                printf("%d. %s\n", fileNumber, directory_entry->d_name);
                fileNumber++;
            }
        }
        closedir(dir);

        printf("\nEnter the number of the file you want to read: ");
        int choice;
        scanf("%d", &choice);

        dir = opendir(path);
        fileNumber = 1;

        while ((directory_entry = readdir(dir)) != NULL) {
            if (strstr(directory_entry->d_name, ".dat") != NULL || strstr(directory_entry->d_name, ".stp") != NULL) {
                if (fileNumber == choice) {
                    char* filePath = malloc(strlen(path) + strlen(directory_entry->d_name) + 2);
                    sprintf(filePath, "%s/%s", path, directory_entry->d_name);
                    closedir(dir);
                    return filePath;
                }
                fileNumber++;
            }
        }

        closedir(dir);
        printf(ERROR_MSG_INVALID_CHOICE);
        return NULL;
    } else {
        perror(ERROR_MSG_DIRECTORY);
        return NULL;
    }
}

/**
 * @brief Allows the user to choose a subdirectory and select a file from it.
 *
 * Displays a menu with options to choose a subdirectory containing files with .dat or .stp extensions.
 *
 * @return A dynamically allocated string representing the selected file's path.
 *         Returns NULL if an error occurs or if the user's choice is invalid.
 */
char* chooseFile(void) {
    int option;
    const char *dirPath;
    
    printf("There are 5 subdirectories with .dat or .stp files. Choose one of the subdirectories:\n");
    printf("1. TEST_SET1\n");
    printf("2. TEST_SET2\n");
    printf("3. ALUE\n");
    printf("4. ALUT\n");
    printf("5. DMXA\n");
    printf("Option: ");
    scanf("%d", &option);

    switch(option) {
        case 1:
            dirPath = DIR_PATH TEST_SET1;
            break;
        case 2:
            dirPath = DIR_PATH TEST_SET2;
            break;
        case 3:
            dirPath = DIR_PATH ALUE;
            break;
        case 4:
            dirPath = DIR_PATH ALUT;
            break;
        case 5:
            dirPath = DIR_PATH DMXA;
            break;
        default:
            printf("Invalid option!\n");
            return NULL;
    }

    return selectFileFromFileList(dirPath);
}


/**
 * Processes a .dat file, reads the number of nodes, the number of arcs, and the list of arcs,
 * and writes the results to an output file.
 *
 * @param input_file Pointer to the input file (.dat) opened for reading.
 * @param output_file Pointer to the output file where the results will be written.
 */
void process_dat_file(FILE *input_file, FILE *output_file) {
    char line[MAX_LINE_LENGTH];
    int nb_nodes = 0;
    int nb_arcs = 0;
    int arc_count = 0;

    while (fgets(line, sizeof(line), input_file)) {
        if (strncmp(line, "NB_NODES", 8) == 0) {
            sscanf(line, "NB_NODES %d", &nb_nodes);
        } else if (strncmp(line, "NB_ARCS", 7) == 0) {
            sscanf(line, "NB_ARCS %d", &nb_arcs);
        } else if (strncmp(line, "LIST_OF_ARCS", 12) == 0) {
            fprintf(output_file, "NODES %d\n", nb_nodes);
            fprintf(output_file, "EDGES %d\n", nb_arcs);
            fprintf(output_file, "EDGE_LIST\n");

            while (arc_count < nb_arcs && fgets(line, sizeof(line), input_file) && strncmp(line, "END", 3) != 0) {
                fprintf(output_file, "%s", line);
                arc_count++;
            }
            break;
        }
    }
}

/**
 * Processes a .stp file, reads the number of nodes, the number of edges, and the list of edges,
 * and writes the results to an output file.
 *
 * @param input_file Pointer to the input file (.stp) opened for reading.
 * @param output_file Pointer to the output file where the results will be written.
 */
void process_stp_file(FILE *input_file, FILE *output_file) {
    char line[MAX_LINE_LENGTH];
    int nodes = 0;
    int edges = 0;

    while (fgets(line, sizeof(line), input_file)) {
        if (strncmp(line, "Nodes", 5) == 0) {
            sscanf(line, "Nodes %d", &nodes);
        } else if (strncmp(line, "Edges", 5) == 0) {
            sscanf(line, "Edges %d", &edges);
        } else if (strncmp(line, "E ", 2) == 0) {
            if (nodes != 0 && edges != 0) {
                fprintf(output_file, "NODES %d\n", nodes);
                fprintf(output_file, "EDGES %d\n", edges);
                fprintf(output_file, "EDGE_LIST\n");
                do {
                    int v1, v2, cost;
                    sscanf(line, "E %d %d %d", &v1, &v2, &cost);
                    fprintf(output_file, "%d %d %d\n", v1 - 1, v2 - 1, cost);
                    fprintf(output_file, "%d %d %d\n", v2 - 1, v1 - 1, cost);
                } while (fgets(line, sizeof(line), input_file) && strncmp(line, "End", 3) != 0);
                break;
            }
        }
    }
}

/**
 * Processes an input file, determines its type by the file extension, and calls the appropriate
 * processing function. Supports .dat and .stp files.
 *
 * @param input_filename Name of the input file.
 * @return A dynamically allocated string representing the path of the output file.
 */
char* process_file(const char *input_filename) {
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        perror(ERROR_MSG_INPUT_FILE);
        exit(EXIT_FAILURE);
    }

    const char *dot = strrchr(input_filename, '.');
    if (dot == NULL) {
        fprintf(stderr, "No file extension found in: %s\n", input_filename);
        fclose(input_file);
        exit(EXIT_FAILURE);
    }

    size_t output_filename_length = dot - input_filename;

    char *output_filename = malloc(output_filename_length + strlen(".temp") + 1);
    if (output_filename == NULL) {
        perror(ERROR_MSG_MEMORY);
        fclose(input_file);
        exit(EXIT_FAILURE);
    }

    strncpy(output_filename, input_filename, output_filename_length);
    strcpy(output_filename + output_filename_length, ".temp");

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror(ERROR_MSG_OUTPUT_FILE);
        fclose(input_file);
        free(output_filename);
        exit(EXIT_FAILURE);
    }
    
    printf("Temp file created at: %s\n", output_filename);

    if (strcmp(dot, ".dat") == 0) {
        process_dat_file(input_file, output_file);
    } else if (strcmp(dot, ".stp") == 0) {
        process_stp_file(input_file, output_file);
    } else {
        fprintf(stderr, "Unsupported file format: %s\n", dot);
        fclose(input_file);
        fclose(output_file);
        free(output_filename);
        exit(EXIT_FAILURE);
    }

    fclose(input_file);
    fclose(output_file);

    return output_filename;
}

/**
 * @brief Calculate the theoretical complexity of Bellman-Ford algorithm.
 *
 * @param n Number of vertices in the graph.
 * @param m Number of edges in the graph.
 * @return Double representing the theoretical complexity O(n * m).
 */
double calculateBellmanFordComplexity(int n, int m) {
    return n * m;
}

/**
 * @brief Calculate the theoretical complexity of Dijkstra algorithm (version 1).
 *
 * @param n Number of vertices in the graph.
 * @return Double representing the theoretical complexity O(n^2).
 */
double calculateDijkstraV1Complexity(int n) {
    return n * n;
}

/**
 * @brief Calculate the theoretical complexity of Dijkstra algorithm (version 2).
 *
 * @param n Number of vertices in the graph.
 * @param m Number of edges in the graph.
 * @return Double representing the theoretical complexity O(m * log(n)).
 */
double calculateDijkstraV2Complexity(int n, int m) {
    return m * log2(n);
}

