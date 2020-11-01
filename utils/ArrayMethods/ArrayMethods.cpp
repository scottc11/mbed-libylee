#include "ArrayMethods.h"

int arr_max(int arr[], int n)
{
    int i;

    // Initialize maximum element
    int max = arr[0];

    // Traverse array elements from second and
    // compare every element with current max
    for (i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];

    return max;
}

int arr_min(int arr[], int n)
{
    int i;

    // Initialize min element
    int min = arr[0];

    // Traverse array elements from second and
    // compare every element with current min
    for (i = 1; i < n; i++)
        if (arr[i] < min)
            min = arr[i];

    return min;
}

int arr_average(int arr[], int n)
{
    // Find sum of array element
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += arr[i];

    return sum / n;
}