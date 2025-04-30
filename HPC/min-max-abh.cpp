#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;

int main()
{
    const int SIZE = 100;
    vector<int> data(SIZE);

    srand(time(0));
    for (int i = 0; i < SIZE; i++)
        data[i] = rand() % 1000;

    int min_val = data[0];
    int max_val = data[0];
    long long sum = 0;

#pragma omp parallel for reduction(min : min_val) reduction(max : max_val) reduction(+ : sum)
    for (int i = 0; i < SIZE; i++)
    {
        if (data[i] < min_val)
            min_val = data[i];
        if (data[i] > max_val)
            max_val = data[i];
        sum += data[i];
    }

    double avg = static_cast<double>(sum) / SIZE;

    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0;
}
// g++ -fopenmp -o HPC1 HPC1.cpp