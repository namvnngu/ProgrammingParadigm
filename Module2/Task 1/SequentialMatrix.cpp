#include <iostream>
#include <array>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

const int MAX_N = 100;

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
      matrix[r][c] = rand() % MAX_N;

  return matrix;
}

vector<vector<int>> multiply_matrix(vector<vector<int>> matrix_A, vector<vector<int>> matrix_B) {
  int row = matrix_A.size();
  int col = matrix_B[0].size();
  vector<vector<int>> result(row, vector<int>(col));
  
  for(int r = 0; r < row; r++)
    for(int c = 0; c < col; c++)
      for(int i = 0; i < int(matrix_B.size()); i++)
        result[r][c] += matrix_A[r][i] * matrix_B[i][c];
  return result;
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

int main() {
  srand(time(0));

  // Initialize row and column size
  int row_A, row_B, col_A, col_B;
  row_A = row_B = col_A = col_B = 1000;

  // Create two matrices
  vector<vector<int>> matrix_A = generate_matrix(col_A, row_A);
  cout << "Matrix A has been generated\n";
  vector<vector<int>> matrix_B = generate_matrix(col_B, row_B);
  cout << "Matrix B has been generated\n";

  // Start measuring execution time of multiplication operation
  clock_t start = clock();
  vector<vector<int>> matrix_C = multiply_matrix(matrix_A, matrix_B);
  // Stop measuring execution time of multiplication operation
  clock_t end = clock();
  // Print execution time
  double execution_time = double(end-start) / (CLOCKS_PER_SEC / 1000);
  cout << "Time of gaining the dot product of two above matrices: " << execution_time << "ms\n";


  save_file("output.txt", matrix_C);
  return 0;
}
