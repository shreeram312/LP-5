// HPC 3 MIN MAX SUM AVG
// To compile and run:
// g++ -fopenmp min_max.cpp -o min_max
// ./min_max 50 20  OR ./min_max and then enter values

#include <limits.h>
#include <omp.h>
#include <stdlib.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::copy

using namespace std;

// Sequential Average
void s_avg(int arr[], int n) {
    long sum = 0L;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    cout << "Average = " << sum / long(n) << endl;
}

// Parallel Average
void p_avg(int arr[], int n) {
    long sum = 0L;
    #pragma omp parallel for reduction(+ : sum) num_threads(16)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    cout << "Average = " << sum / long(n) << endl;
}

// Sequential Sum
void s_sum(int arr[], int n) {
    long sum = 0L;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    cout << "Sum = " << sum << endl;
}

// Parallel Sum
void p_sum(int arr[], int n) {
    long sum = 0L;
    #pragma omp parallel for reduction(+ : sum) num_threads(16)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    cout << "Sum = " << sum << endl;
}

// Sequential Max
void s_max(int arr[], int n) {
    int max_val = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    cout << "Max = " << max_val << endl;
}

// Parallel Max
void p_max(int arr[], int n) {
    int max_val = INT_MIN;
    #pragma omp parallel for reduction(max : max_val) num_threads(16)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    cout << "Max = " << max_val << endl;
}

// Sequential Min
void s_min(int arr[], int n) {
    int min_val = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    cout << "Min = " << min_val << endl;
}

// Parallel Min
void p_min(int arr[], int n) {
    int min_val = INT_MAX;
    #pragma omp parallel for reduction(min : min_val) num_threads(16)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    cout << "Min = " << min_val << endl;
}

// Benchmark execution time
string bench_traverse(function<void()> traverse_fn) {
    auto start = chrono::high_resolution_clock::now();
    traverse_fn();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    return to_string(duration.count());
}

// Main function
int main(int argc, const char **argv) {
    int n, rand_max;

    // Command-line argument parsing
    if (argc < 3) {
        cout << "Enter array length: ";
        cin >> n;
        cout << "Enter maximum random value: ";
        cin >> rand_max;
    } else {
        n = stoi(argv[1]);
        rand_max = stoi(argv[2]);
    }

    // Allocate and fill array
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
        a[i] = rand() % rand_max;
    }

    // Display generated array (first 20 elements only for readability)
    cout << "\nGenerated array (first 20 elements):\n";
    for (int i = 0; i < min(n, 20); i++) {
        cout << a[i] << "\t";
    }
    cout << "\n\n";

    // Create a copy for other operations
    int* b = new int[n];
    copy(a, a + n, b);

    cout << "Generated random array of length " << n
         << " with elements between 0 and " << rand_max << "\n\n";

    // Perform and benchmark operations
    cout << "Sequential Min: " << bench_traverse([&] { s_min(a, n); }) << " ms\n";
    cout << "Parallel (16 threads) Min: " << bench_traverse([&] { p_min(a, n); }) << " ms\n\n";

    cout << "Sequential Max: " << bench_traverse([&] { s_max(a, n); }) << " ms\n";
    cout << "Parallel (16 threads) Max: " << bench_traverse([&] { p_max(a, n); }) << " ms\n\n";

    cout << "Sequential Sum: " << bench_traverse([&] { s_sum(a, n); }) << " ms\n";
    cout << "Parallel (16 threads) Sum: " << bench_traverse([&] { p_sum(a, n); }) << " ms\n\n";

    cout << "Sequential Average: " << bench_traverse([&] { s_avg(a, n); }) << " ms\n";
    cout << "Parallel (16 threads) Average: " << bench_traverse([&] { p_avg(a, n); }) << " ms\n";

    // Free memory
    delete[] a;
    delete[] b;

    return 0;
}
