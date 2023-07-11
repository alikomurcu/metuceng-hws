#include<iostream>

using namespace std;
/*
Quicksort with Classical Partitioning[30pts] is called using the function quickSort() 
with $hoare$=false. It should sort the array in descending order, count the number of $swap$
 executed during sorting process, calculate the average distance between swap positions $avg_dist$, 
 find the max distance between swap positions $max_dist$(which are all 0 if there are no swaps).

Quicksort with Hoare Partitioning[30pts] is called using the function quickSort() with
 $hoare$=true. It should sort the array in descending order, count the number of $swap$ 
 executed during sorting process, calculate the average distance between swap positions $avg_dist$,
  find the max distance between swap positions $max_dist$.

3-Way Quicksort[40pts] is called using the function quickSort3(). It should sort the 
array in descending order, count the number of $swap$ executed during sorting process
 and count the number of $comparison$ executed during sorting proccess (Comparisons are only
  between the values to be sorted only, not your auxiliary comparisons) (which are all 0 if there are no swaps and comparisons).
*/

void fswap(unsigned short* x, unsigned short* y)  // Reference de hata var mı bak.
{
    unsigned short tmp = *x;
    *x = *y;
    *y = tmp;
}

unsigned short abs(unsigned short n)
{
    if(n < 0)  return -n;
    return n;
}

int partition(unsigned short* arr, int size, long &swap, double &avg_dist, double &max_dist)
{
    unsigned short X = arr[size-1];
    int i = -1;
    double dist = 0;
    for (int j = 0; j <= size-2; j++)
    {
        if(arr[j]>= X)
        {
            i++;
            fswap(&arr[i], &arr[j]); swap++;
            dist = j-i;
            avg_dist += dist;
            if(dist >= max_dist) max_dist = dist;
        }
    }
    fswap(&arr[i+1], &arr[size-1]); swap++;
    dist = size - 2 - i;  // abs(i+2-size)
    avg_dist += dist;
    if(dist >= max_dist) max_dist = dist;

    return i+1;
}

int func_hoare(unsigned short* arr, int size, long &swap, double &avg_dist, double &max_dist)
{
    unsigned short X = arr[(size-1)/2]; // floor is handled by integer division in the expression (size-1)/2
    int i = -1, j = size, dist;
    while (true)
    {
        while (true)
        {
            j--;
            if(arr[j] >= X)  break;
        }
        while (true)
        {
            i++;
            if(arr[i] <= X)  break;
        }
        if(i < j)
        {
            fswap(&arr[i], &arr[j]);
            swap++;
            dist = j-i; 
            avg_dist += dist;
            if(dist >= max_dist) max_dist = dist;
        }
        else return j;
    }
    
}

void help_quickSort(unsigned short* arr, long &swap, double & avg_dist, double & max_dist, bool hoare, int size)
{
    static int total = 0, total2 = 0, count = 0;
    count++;
    if(!hoare)  // Partition
    {
        if(size>1)
        {
            int p = partition(arr, size, swap, avg_dist, max_dist);
            help_quickSort(arr, swap, avg_dist, max_dist, hoare, p);
            help_quickSort(arr + p + 1, swap, avg_dist, max_dist, hoare, size-p-1);
        }
    }
    else  // Hoare
    {
        if(size>1)
        {
            int p = func_hoare(arr, size, swap, avg_dist, max_dist);
            help_quickSort(arr, swap, avg_dist, max_dist, hoare, p + 1);
            help_quickSort(arr + p + 1, swap, avg_dist, max_dist, hoare, size-p-1);
        }
    }
}

void quickSort(unsigned short* arr, long &swap, double & avg_dist, double & max_dist, bool hoare, int size)
{
    help_quickSort(arr, swap, avg_dist, max_dist, hoare, size);
    if(swap != 0) avg_dist = avg_dist / swap;
}


int get_min(int x, int y)
{
    return x <= y  ? x : y;
}

void partition_3_way(unsigned short *arr, long &swap, long &comparison, int size, int *L, int *R)
{
    int i = 0, j = 0, p = size - 1, m;
    while(i < p)
    {
        if(arr[i] > arr[size - 1])
        {
            comparison++;
            fswap(&arr[i], &arr[j]);
            swap++;
            i++; j++;
        }
        else if(arr[i] == arr[size-1])
        {
            comparison += 2;
            p--;
            fswap(&arr[i], &arr[p]);
            swap++;
        }
        else
        {
            i++;
            comparison += 2;
        }
    }
    m = get_min(p-j, size-p);

    for(int k = j, t = size-m; k <= j + m - 1; k++, t++)
    {
        fswap(&arr[k], &arr[t]);
        swap++;
    }
    *L = j;
    *R = p-j;
}


void quickSort3(unsigned short *arr, long &swap, long &comparison, int size) 
{
    if(size > 1)
    {
        int *L = new int();
        int *R = new int();
        partition_3_way(arr, swap, comparison, size, L, R);
        quickSort3(arr, swap, comparison, *L);
        quickSort3(arr + (size) - *R, swap, comparison, *R);
    }
}