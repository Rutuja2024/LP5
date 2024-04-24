#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Sequential merge sort
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void sequentialMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Parallel merge sort
void parallelMerge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, l, m);
#pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }

        parallelMerge(arr, l, m, r);
    }
}

int main() {
    const int n = 2050000; // Size of array
    vector<int> arr(n), arr_copy(n);

    // Generate random array
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }

    // Copy array for parallel sort
    copy(arr.begin(), arr.end(), arr_copy.begin());

    // Measure sequential merge sort time
    auto start_time = high_resolution_clock::now();
    sequentialMergeSort(arr, 0, n - 1);
    auto end_time = high_resolution_clock::now();
    auto sequential_time = duration_cast<duration<double>>(end_time - start_time).count();

    // Measure parallel merge sort time
    start_time = high_resolution_clock::now();
    parallelMergeSort(arr_copy, 0, n - 1);
    end_time = high_resolution_clock::now();
    auto parallel_time = duration_cast<duration<double>>(end_time - start_time).count();

    // Output execution times
    cout << "Sequential merge sort time: " << sequential_time << " seconds" << endl;
    cout << "Parallel merge sort time: " << parallel_time << " seconds" << endl;

    // Verify correctness of sorting
    for (int i = 0; i < n; i++) {
        if (arr[i] != arr_copy[i]) {
            cout << "Error: Sorting failed!" << endl;
            return 1;
        }
    }

    cout << "Sorting successful!" << endl;

    return 0;
}
