#include <iostream>
#include <array>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <pthread.h>
using namespace std;

const int MAX_RANDOM_NUM = 100;
int NUM_THREADS = 10;
const int N = 1000;

void print_matrix(vector<vector<int>> matrix) {
  int column = matrix[0].size();
  int row = matrix.size();
  for(int r = 0; r < row; r++) {
    for(int c = 0; c < column; c++)
      cout << matrix[r][c] << ' ';
    cout << '\n';
  }
}

vector<vector<int>> generate_matrix(int row, int column) {
  vector<vector<int>> matrix(row, vector<int>(column));
  
  for(int r = 0; r < row; r++) 
    for(int c = 0; c < column; c++)
      matrix[r][c] = rand() % MAX_RANDOM_NUM;

  return matrix;
}

void save_file(string file_name, vector<vector<int>> matrix) {
  ofstream output;
  output.open(file_name);

  int column = matrix[0].size();
  int row = matrix.size();
  for(int r = 0; r < row; r++) {
    for(int c = 0; c < column; c++) {
      output << matrix[r][c];
      if(c < column - 1)
         output << ", ";
    }
    output << '\n';
  }

  output.close();
}

struct thread_data {
  int thread_id;
  vector<vector<int>> A;
  vector<vector<int>> B;
  vector<vector<int>> *C;
};

void *multiply_matrix(void *threadarg) {
  struct thread_data *data;
  data = (struct thread_data *) threadarg;
  vector<vector<int>> matrix_A = data->A;
  vector<vector<int>> matrix_B = data->B;
  vector<vector<int>> &result = *(data->C);
  int thread_id = data->thread_id;
  int col = matrix_B[0].size();
  
  int group_num = N % NUM_THREADS ? (N / NUM_THREADS) + 1 : (N / NUM_THREADS);
  int start_row = thread_id * group_num;
  int end_row = (thread_id + 1) * group_num;
  if(end_row > N)
    end_row = N;

  for(int r = start_row; r < end_row; r++)
    for(int c = 0; c < col; c++)
      for(int i = 0; i < int(matrix_B.size()); i++)
        result[r][c] += matrix_A[r][i] * matrix_B[i][c];
  pthread_exit(NULL);
}

int main() {
  srand(time(0));

  // Initialize row and column size
  int row_A, row_B, col_A, col_B;
  row_A = row_B = col_A = col_B = N;

  // Create two matrices
  vector<vector<int>> matrix_A = generate_matrix(col_A, row_A);
  cout << "Matrix A has been generated\n";
  vector<vector<int>> matrix_B = generate_matrix(col_B, row_B);
  cout << "Matrix B has been generated\n";
  vector<vector<int>> matrix_C(row_A,vector<int>(col_B));

  // Start measuring execution time of multiplication operation
  clock_t start = clock();

  // THREADING: Start
  NUM_THREADS = N < NUM_THREADS ? N : NUM_THREADS;
  pthread_t threads[NUM_THREADS];
  thread_data td[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++) {
    td[i].thread_id = i;
    td[i].A = matrix_A;
    td[i].B = matrix_B;
    td[i].C = &matrix_C;
    int rc = pthread_create(&threads[i], NULL, multiply_matrix, (void *)&td[i]);
    if(rc) {
      cout << "Error: Unable to create thread, " << rc << '\n';
      exit(-1);
    }
  }
  
  for(int i = 0; i < NUM_THREADS; i++) {
    int rc = pthread_join(threads[i], NULL);
    if(rc) {
      cout << "Error: Unable to join thread, " << rc << '\n';
      exit(-1);
    }
  }
  // THREADING: End

  // Stop measuring execution time of multiplication operation
  clock_t end = clock();
  // Print execution time
  double execution_time = double(end-start) / (CLOCKS_PER_SEC / 1000);
  cout << "Time of gaining the dot product of two above matrices: " << execution_time << "ms\n";

  save_file("output.txt", matrix_C);
  return 0;
}
