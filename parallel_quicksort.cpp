#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Partition function for quick sort
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Sequential quick sort
void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sequentialQuickSort(arr, low, pi - 1);
        sequentialQuickSort(arr, pi + 1, high);
    }
}

// Parallel quick sort
void parallelQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
#pragma omp parallel sections
        {
#pragma omp section
            parallelQuickSort(arr, low, pi - 1);
#pragma omp section
            parallelQuickSort(arr, pi + 1, high);
        }
    }
}

int main() {
    const int n = 10000; // Size of array
    vector<int> arr(n), arr_copy(n);

    // Generate random array
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % n;
    }

    // Copy array for parallel sort
    copy(arr.begin(), arr.end(), arr_copy.begin());

    // Measure sequential quick sort time
    auto start_time = high_resolution_clock::now();
    sequentialQuickSort(arr, 0, n - 1);
    auto end_time = high_resolution_clock::now();
    auto sequential_time = duration_cast<duration<double>>(end_time - start_time).count();

    // Measure parallel quick sort time
    start_time = high_resolution_clock::now();
    parallelQuickSort(arr_copy, 0, n - 1);
    end_time = high_resolution_clock::now();
    auto parallel_time = duration_cast<duration<double>>(end_time - start_time).count();

    // Output execution times
    cout << "Sequential quick sort time: " << sequential_time << " seconds" << endl;
    cout << "Parallel quick sort time: " << parallel_time << " seconds" << endl;

    // Verify correctness of sorting
    for (int i = 0; i < n; ++i) {
        if (arr[i] != arr_copy[i]) {
            cout << "Error: Sorting failed!" << endl;
            return 1;
        }
    }

    cout << "Sorting successful!" << endl;

    return 0;
}
