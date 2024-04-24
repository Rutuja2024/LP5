#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Sequential bubble sort
void sequentialBubbleSort(vector<int>& array , int n) {
    // int n = arr.size();
    // for (int i = 0; i < n - 1; ++i) {
    //     for (int j = 0; j < n - i - 1; ++j) {
    //         if (arr[j] > arr[j + 1]) {
    //             swap(arr[j], arr[j + 1]);
    //         }
    //     }
    // }
     int var = 0;

	for(int i=0; i<n; i++)
	{
		for(int j=var; j<n-1; j+=2)
		{
			if(array[j] > array[j+1])
			{
				int temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
		}
		
		if(var == 0) var = 1;
		else var = 0;
}
}

// Parallel bubble sort
void parallelBubbleSort(vector<int>& array, int n) {
//     int n = arr.size();
//     for (int i = 0; i < n - 1; ++i) {
// #pragma omp parallel for
//         for (int j = 0; j < n - i - 1; ++j) {
//             if (arr[j] > arr[j + 1]) {
//                 swap(arr[j], arr[j + 1]);
//             }
//         }
//     }
 int var = 0;

	for(int i=0; i<n; i++)
	{
        
		#pragma omp parallel for
		for(int j=var; j<n-1; j+=2)
		{ 
            
			if(array[j] > array[j+1])
			{
				int temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
		}
		
		if(var == 0) var = 1;
		else var = 0;
}
}
void printArray(vector<int> arr, int n){
    for(int i = 0; i < 30; i++) cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    const int n = 1500; // Size of array
    vector<int> arr(n), arr_copy(n);

    // Generate random array
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 500;
    }

    // Copy array for parallel sort
    copy(arr.begin(), arr.end(), arr_copy.begin());

    // Measure sequential bubble sort time
    auto start_time = high_resolution_clock::now();
    sequentialBubbleSort(arr, n);
    auto end_time = high_resolution_clock::now();
    auto sequential_time = duration_cast<duration<double>>(end_time - start_time).count();
 printArray(arr, n);
    // Measure parallel bubble sort time
    start_time = high_resolution_clock::now();
    parallelBubbleSort(arr_copy, n);
    end_time = high_resolution_clock::now();
    auto parallel_time = duration_cast<duration<double>>(end_time - start_time).count();

    // Output execution times
    cout << "Sequential bubble sort time: " << sequential_time << " seconds" << endl;
    cout << "Parallel bubble sort time: " << parallel_time << " seconds" << endl;
 printArray(arr_copy, n);
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
