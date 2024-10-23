//Fadi Bassous
//1221005
//sec 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUILDING_NUM 30
#define COURSE_NUM 30
#define INF INT_MAX



struct Graph {
    char Building[BUILDING_NUM][50];
    int matrix[BUILDING_NUM][BUILDING_NUM];
} typedef Graph;

typedef struct node *Stack;
struct node {
    char course[50];
    Stack Next;
};

struct Graph2 {
    char courses[COURSE_NUM][50];
    int matrix[COURSE_NUM][COURSE_NUM];
} typedef Graph2;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dijkstra algorithm ------------------------------------------------------------------------------------------------

Graph *createGraph();

void addPath(struct Graph *graph, char source[], char destination[], int distance);

void loadBuilding(Graph *graph);

int minDist(int dist[], int sptSet[]);

void printPath(int parent[], int j, Graph *graph, int dist[], int *preI);

void printResult(int dist[], int parent[], Graph *graph, int src, int dest);

void dijkstra(Graph *graph, char source[], char destination[]);

// Topological Sort ----------------------------------------------------------------------------------------------------
Stack CreateStack();

int IsEmpty(Stack S);

void MakeEmpty(Stack S);

void Pop(Stack S);

void Push(char *X, Stack S);

void DisposeStack(Stack S);

Graph2 *createGraph2();

void addCourse(Graph2 *graph, char course[], char prerequisites[]);

void loadCourses(Graph2 *graph);

void topologicalSortUtil(Graph2 *graph, int v, int visited[], Stack stack);

void topologicalSort(Graph2 *graph);

void printStack(Stack S);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    Graph *graph = createGraph();
    Graph2 *graph2 = createGraph2();

    int choice;
    char src[50];
    char dest[50];

    while (1) {
        printf("\n1. Load the two files\n");
        printf("2. Calculate the shortest distance between two buildings.\n");
        printf("3. Print the shortest route between two buildings and the total distance.\n");
        printf("4. Sort the courses using topological sort.\n");
        printf("5. Print the sorted courses.\n");
        printf("6. Exit the application.\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n");
                loadBuilding(graph);
                loadCourses(graph2);
                printf("The files have been loaded.\n");
                break;
            case 2:
                printf("Please enter source Building:\n ");
                scanf("%s", src);
                if (strcmp(src, "") == 0) {
                    printf("Please enter source Building first (option 1).\n");
                    break;
                }
                printf("Please enter the destination Building:\n ");
                scanf("%s", dest);
                break;
            case 3:
                printf("\n\nResults:\n");
                dijkstra(graph, src, dest);
                break;
            case 4:
                printf("the courses have been sorted\n");
                break;
            case 5:
                printf("Sorted Courses:\n");
                topologicalSort(graph2);
                break;
            case 6:
                exit(1);
        }
        printf("\n");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to create a graph:
Graph *createGraph() {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Out of memory!\n");
    }
    for (int i = 0; i < BUILDING_NUM; i++) {
        for (int j = 0; j < BUILDING_NUM; j++) {
            graph->matrix[i][j] = 0;
        }
        strcpy(graph->Building[i], "");
    }
    return graph;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addPath(struct Graph *graph, char source[], char destination[], int distance) {
    int sourceIndex = -1;
    int destinationIndex = -1;

    for (int i = 0; i < BUILDING_NUM; i++) {
        if (strcmp(graph->Building[i], source) == 0) {
            sourceIndex = i;
        }
        if (strcmp(graph->Building[i], destination) == 0) {
            destinationIndex = i;
        }
    }

    if (sourceIndex == -1) {
        for (int i = 0; i < BUILDING_NUM; i++) {
            if (strcmp(graph->Building[i], "") == 0) {
                strcpy(graph->Building[i], source);
                sourceIndex = i;
                break;
            }
        }
    }

    if (destinationIndex == -1) {
        for (int i = 0; i < BUILDING_NUM; i++) {
            if (strcmp(graph->Building[i], "") == 0) {
                strcpy(graph->Building[i], destination);
                destinationIndex = i;
                break;
            }
        }
    }

    graph->matrix[sourceIndex][destinationIndex] = distance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loadBuilding(Graph *graph) {
    FILE *file = fopen("input_buildings.txt", "r");
    if (file == NULL) {
        printf("*ERROR: File couldn't be opened*\n");
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char *srcCity = strtok(line, "#");
        char *disCity = strtok(NULL, "#");
        char *distanceStr = strtok(NULL, "#");
        int distance = atoi(distanceStr);
        addPath(graph, srcCity, disCity, distance);
    }
    fclose(file);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int minDist(int dist[], int sptSet[]) {
    int min = INF;
    int min_index = -1;

    for (int v = 0; v < BUILDING_NUM; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printPath(int parent[], int j, Graph *graph, int dist[], int *preI) {
    if (parent[j] == -1) {
        *preI = j;
        return;
    }
    printPath(parent, parent[j], graph, dist, preI);
    printf(" -> %s (%d)", graph->Building[j], dist[j] - dist[*preI]);
    *preI = j;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printResult(int dist[], int parent[], Graph *graph, int src, int dest) {
    printf("\nShortest Path from %s to %s \t Distance: %d", graph->Building[src], graph->Building[dest], dist[dest]);
    printf("\nPath: %s", graph->Building[src]);
    int j = src;
    printPath(parent, dest, graph, dist, &j);
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dijkstra(Graph *graph, char source[], char destination[]) {
    int dist[BUILDING_NUM];
    int sptSet[BUILDING_NUM];
    int parent[BUILDING_NUM];

    int srcIndex = -1;
    int destIndex = -1;

    for (int i = 0; i < BUILDING_NUM; i++) {
        if (strcmp(graph->Building[i], source) == 0) {
            srcIndex = i;
        }
        if (strcmp(graph->Building[i], destination) == 0) {
            destIndex = i;
        }
        parent[i] = -1;
        dist[i] = INF;
        sptSet[i] = 0;
    }

    if (srcIndex == -1) {
        printf("Source Building not found\n");
        return;
    }
    if (destIndex == -1) {
        printf("Destination Building not found\n");
        return;
    }

    dist[srcIndex] = 0;

    for (int count = 0; count < BUILDING_NUM - 1; count++) {
        int u = minDist(dist, sptSet);
        sptSet[u] = 1;

        if (u == destIndex) {
            break;
        }

        for (int v = 0; v < BUILDING_NUM; v++) {
            if (!sptSet[v] && graph->matrix[u][v] && dist[u] != INF && dist[u] + graph->matrix[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph->matrix[u][v];
            }
        }
    }

    printResult(dist, parent, graph, srcIndex, destIndex);
}

// Topological Sort ----------------------------------------------------------------------------------------------------

Graph2 *createGraph2() {
    Graph2 *graph = (Graph2 *) malloc(sizeof(Graph2));
    if (graph == NULL) {
        printf("Out of memory!\n");
    }
    for (int i = 0; i < COURSE_NUM; i++) {
        for (int j = 0; j < COURSE_NUM; j++) {
            graph->matrix[i][j] = 0;
        }
        strcpy(graph->courses[i], "");
    }
    return graph;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void addCourse(Graph2 *graph, char course[], char prerequisites[]) {
    int prerequisiteIndex = -1;
    int courseIndex = -1;

    for (int i = 0; i < BUILDING_NUM; i++) {
        if (strcmp(graph->courses[i], course) == 0) {
            prerequisiteIndex = i;
        }
        if (strcmp(graph->courses[i], prerequisites) == 0) {
            courseIndex = i;
        }
    }

    if (prerequisiteIndex == -1) {
        for (int i = 0; i < BUILDING_NUM; i++) {
            if (strcmp(graph->courses[i], "") == 0) {
                strcpy(graph->courses[i], course);
                prerequisiteIndex = i;
                break;
            }
        }
    }

    if (courseIndex == -1) {
        for (int i = 0; i < BUILDING_NUM; i++) {
            if (strcmp(graph->courses[i], "") == 0) {
                strcpy(graph->courses[i], prerequisites);
                courseIndex = i;
                break;
            }
        }
    }

    graph->matrix[prerequisiteIndex][courseIndex] = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loadCourses(Graph2 *graph) {
    FILE *file = fopen("input_courses.txt", "r");
    if (file == NULL) {
        printf(" File couldn't be opened*\n");
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char *course = strtok(line, "#");
        char *prerequisite = strtok(NULL, "#");
        addCourse(graph, course, prerequisite);
    }
    fclose(file);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void topologicalSortUtil(Graph2 *graph, int v, int visited[], Stack stack) {
    visited[v] = 1;

    for (int i = 0; i < COURSE_NUM; i++) {
        if (graph->matrix[v][i]!=0 && !visited[i]) {
            topologicalSortUtil(graph, i, visited, stack);
        }
    }

    Push(graph->courses[v], stack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void topologicalSort(Graph2 *graph) {
    Stack stack = CreateStack();
    int visited[COURSE_NUM] = {0};

    for (int i = 0; i < COURSE_NUM; i++) {
        if (!visited[i]&& strcmp(graph->courses[i], "") != 0) {
            topologicalSortUtil(graph, i, visited, stack);
        }
    }

    printf("Topologically sorted courses:\n");
    printStack(stack);
    DisposeStack(stack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Stack CreateStack() {
    Stack S = (Stack) malloc(sizeof(struct node));
    if (S == NULL) {
        printf("Out of space!");
    }
    S->Next = NULL;
    MakeEmpty(S);
    return S;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int IsEmpty(Stack S) {
    return S->Next == NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MakeEmpty(Stack S) {
    if (S == NULL) {
        printf("Out of space!");
    } else {
        while (!IsEmpty(S)) {
            Pop(S);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pop(Stack S) {
    Stack firstCell;
    if (IsEmpty(S)) {
        printf("Empty stack\n");
    } else {
        firstCell = S->Next;
        S->Next = S->Next->Next;
        free(firstCell);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Push(char *X, Stack S) {
    Stack temp = (Stack) malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("Out of space");
    } else {
        strcpy(temp->course, X);
        temp->Next = S->Next;
        S->Next = temp;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisposeStack(Stack S) {
    MakeEmpty(S);
    free(S);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printStack(Stack S) {
    Stack temp = S->Next;
    while (temp != NULL) {
        printf("%s\n", temp->course);
        temp = temp->Next;
    }
    printf("\n");
}
