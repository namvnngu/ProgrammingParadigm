#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

const int N = 1000000;
const int MAX_VALUE = 1000;
const int NUM_THREAD = 5;

bool is_sorted(vector<int> &source, vector<int> &target) {
  sort(source.begin(), source.end());
  for(int i = 0; i < N; i++)
    if(source[i] != target[i])
      return false;
  return true;
}

int partition(vector<int> &v, int start, int end) {
  int pivotValue = v[end];
  int pivotIndex = start;

  for(int i = start; i < end; i++) {
    if(v[i] < pivotValue) {
      swap(v[i], v[pivotIndex]);
      pivotIndex++;
    }
  }

  swap(v[pivotIndex], v[end]);
  return pivotIndex;
}
void quick_sort(vector<int> &v, int start, int end) {
  if(start >= end)
    return;

  int index = partition(v, start, end);

  quick_sort(v, start, index - 1);
  quick_sort(v, index + 1, end);
}

int main() {
  srand(time(0));

  vector<int> v(N, 0);
  vector<int> test(N, 0);
  for(int i = 0; i < N; i++) {
    v[i] = rand() % MAX_VALUE;
    test[i] = v[i];
  }
  
  // Start measuring execution time of multiplication operation
  clock_t start = clock();

  int pivot;
  if(N > 1) pivot = partition(v, 0, N - 1);
  omp_set_num_threads(NUM_THREAD);
  #pragma omp sections
  {
    #pragma omp section 
    if(pivot != 0) quick_sort(v, 0, pivot);
    #pragma omp section 
    if(pivot != N) quick_sort(v, pivot+1, N-1);
  }

  // Stop measuring execution time of multiplication operation
  clock_t end = clock();
  // Print execution time
  double execution_time = double(end-start) / (CLOCKS_PER_SEC / 1000);
  cout << "Time of gaining the dot product of two above matrices: " << execution_time << "ms\n"; 

  // for(int i = 0; i < N; i++)
  //    cout << v[i] << ' ';
  if(is_sorted(test, v))
    cout << "\nThe vector is sorted\n";
  else 
    cout << "\nThe vector is not sorted\n";
}
