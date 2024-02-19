#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 49

int x = 7;
int y = 7;
int m = 4;
int n = 4;
int matrix1[7][7];
int matrix2[4][4];
int result[7][7];
pthread_mutex_t mutex;




typedef struct
{
    int row;
    int col;
} element;

// Multiplication process function.
void *multip(void *args)
{
    element *elements = (element *)args;
    int row = elements->row;
    int col = elements->col;

    int sum = 0;
    for (int k = 0; k < m && row + k < x; k++)
    {
        for (int l = 0; l < n && col + l < y; l++)
        {
            sum += matrix1[row + k][col + l] * matrix2[k][l];
        }
    }

    // Locking before updating result matrix
    pthread_mutex_lock(&mutex);
    result[row][col] = sum;
    // Print the result matrix
    printf("Result matrix is:\n");
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    // Release the lock
    pthread_mutex_unlock(&mutex);

    
}

int main()
{
    srand(time(NULL));

    pthread_t threads[MAX_THREADS];
    element elements[MAX_THREADS];

    // Matrix 1 is creating
    printf("Matrix 1 is:\n");
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            matrix1[i][j] = rand() % 50;
            printf("%d ", matrix1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Matrix 2 is creating
    printf("Matrix 2 is:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix2[i][j] = rand() % 50;
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Initialize the mutex
    

    // Initially, result matrix items must be -1.
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            result[i][j] = -1;
        }
    }
    pthread_mutex_init(&mutex, NULL);
    int threadCount = 0;

    // Threads are creating to calculating.
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            elements[threadCount].row = i;
            elements[threadCount].col = j;

            pthread_create(&threads[threadCount], NULL, multip, &elements[threadCount]);

            threadCount++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < threadCount; i++)
    {
        pthread_join(threads[i], NULL);
    }

    

    

    return 0;
}