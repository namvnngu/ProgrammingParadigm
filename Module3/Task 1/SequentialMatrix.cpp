#include <iostream>
#include <array>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <mpi.h>
using namespace std;

#define MASTER 0
#define MASTER_TAG 1
#define WORKER_TAG 2
const int MAX_N = 10;
const int MATRIX_SIZE = 2000;
int matrix_A[MATRIX_SIZE][MATRIX_SIZE];
int matrix_B[MATRIX_SIZE][MATRIX_SIZE];
int matrix_C[MATRIX_SIZE][MATRIX_SIZE];

void print_matrix(int matrix[][MATRIX_SIZE]) {
  cout << '\n';
  for(int r = 0; r < MATRIX_SIZE; r++) {
    for(int c = 0; c < MATRIX_SIZE; c++)
      cout << matrix[r][c] << ' ';
    cout << '\n';
  }
}

void generate_matrix(int matrix[][MATRIX_SIZE]) {
  for(int r = 0; r < MATRIX_SIZE; r++) 
    for(int c = 0; c < MATRIX_SIZE; c++)
      matrix[r][c] = rand() % MAX_N;
}

void save_file(string file_name, int matrix[][MATRIX_SIZE]) {
  ofstream output;
  output.open(file_name);

  for(int r = 0; r < MATRIX_SIZE; r++) {
    for(int c = 0; c < MATRIX_SIZE; c++) {
      output << matrix[r][c];
      if(c < MATRIX_SIZE - 1)
         output << ", ";
    }
    output << '\n';
  }

  output.close();
}

int main(int argc, char *argv[]) {
  int num_process, process_id, num_node_workers;
  int rows; // The number of rows sent from MASTER to Node Workers
  int row_number; // The row number which Node Worker starts working from
  int row_processed_by_worker, remainder_row;
  double start, end;
  MPI_Status status;

  // Seed rand function
  srand(time(0));

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_process);

  // Number of node workers
  num_node_workers = num_process - 1;

  /* 
   * MASTER Process
   */
  if(process_id == MASTER) {
    printf("There are %d processes and %d node workers\n", num_process, num_node_workers);

    // Generate two matrices
    generate_matrix(matrix_A);
    printf("\nMatrix A has been generated");
    print_matrix(matrix_A);
    generate_matrix(matrix_B);
    printf("\nMatrix B has been generated");
    print_matrix(matrix_B);

    // Start measuring execution time of multiplication operation
    start = MPI_Wtime();

    // Send matrices to Node Workers
    row_processed_by_worker = MATRIX_SIZE / num_node_workers;
    remainder_row = MATRIX_SIZE % num_node_workers;
    row_number = 0;
    for(int dest = 1; dest <= num_node_workers; dest++) {
      rows = dest <= remainder_row ? row_processed_by_worker + 1 : row_processed_by_worker;
      printf("\n%d rows are sent to node worker %d from row %d", rows, dest, row_number);
      
      // Send the number of rows
      MPI_Send(&rows, 1, MPI_INT, dest, MASTER_TAG, MPI_COMM_WORLD);
      // Send the row number
      MPI_Send(&row_number, 1, MPI_INT, dest, MASTER_TAG, MPI_COMM_WORLD);
      // Send matrix A
      MPI_Send(&matrix_A[row_number][0], rows * MATRIX_SIZE, MPI_INT, dest, MASTER_TAG, MPI_COMM_WORLD);
      // Send matrix B
      MPI_Send(&matrix_B, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, dest, MASTER_TAG, MPI_COMM_WORLD);

      row_number += rows;
    }


    // Receive results from node workers
    for(int i = 1; i <= num_node_workers; i++) {
      MPI_Recv(&rows, 1, MPI_INT, i, WORKER_TAG, MPI_COMM_WORLD, &status);
      MPI_Recv(&row_number, 1, MPI_INT, i, WORKER_TAG, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_C[row_number][0], rows * MATRIX_SIZE, MPI_INT, i, WORKER_TAG, MPI_COMM_WORLD, &status);
      printf("\nMASTER %d received result of %d rows with row number %d from node worker %d", MASTER, rows, row_number, i);
    }

    // Stop measuring execution time of multiplication operation
    end = MPI_Wtime();
    cout << '\n';
    print_matrix(matrix_C);
    // Print execution time
    double execution_time = double(end-start) * 1000;
    cout << "\nTime of gaining the dot product of two above matrices: " << execution_time << "ms\n";
  }

  /* 
   * Node Worker
   */

  if(process_id != MASTER) {
    MPI_Recv(&rows, 1, MPI_INT, MASTER, MASTER_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(&row_number, 1, MPI_INT, MASTER, MASTER_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_A, rows * MATRIX_SIZE, MPI_INT, MASTER, MASTER_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_B, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, MASTER, MASTER_TAG, MPI_COMM_WORLD, &status);
    printf("\nNode worker %d received %d rows with row number %d from MASTER %d", process_id, rows, row_number, MASTER);


    for(int c = 0; c < MATRIX_SIZE; c++) {
      for(int r = 0; r < rows; r++) {
        matrix_C[r][c] = 0;
        for(int i = 0; i < MATRIX_SIZE; i++) {
          matrix_C[r][c] += (matrix_A[r][i] * matrix_B[i][c]);
        }
      }
    }

    printf("\nNode worker %d sent result of %d rows with row number %d to MASTER %d", process_id, rows, row_number, MASTER);
    MPI_Send(&rows, 1, MPI_INT, MASTER, WORKER_TAG, MPI_COMM_WORLD);
    MPI_Send(&row_number, 1, MPI_INT, MASTER, WORKER_TAG, MPI_COMM_WORLD);
    MPI_Send(&matrix_C, rows * MATRIX_SIZE, MPI_INT, MASTER, WORKER_TAG, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  // save_file("output.txt", matrix_C);
  cout << '\n';
  return 0;
}
