#include<iostream>
/*
In this exam, you are asked to complete the function definition to sort 
the given array $arr$ with ascending order. Your function should also 
count the number of $comparison$ and $swap$ executed during this sorting 
proccess (Comparisons are only between the values to be sorted only, not 
your auxiliary comparisons).
*/

using namespace std;

void insertionSort(int* arr, long &comparison, long & swap, int size) 
{
    for(int i = 1; i < size; i++)
    {
        int j = i-1;
        int X = arr[i];
        while(j>=0 && arr[j]>X)
        {
            comparison++;
            arr[j+1] = arr[j];
            swap++;
            j--;
        }
        comparison++;
        arr[j+1] = X;
    }
}

int main()
{
    int *arr = new int[6];
    long swap = 0, comparison = 0;
    arr[0] = 5;arr[1] = 6;arr[2] = 7;arr[3] = 3;arr[4] = 3;arr[5] = 0;arr[6] = 10;
    insertionSort(arr, swap, comparison, 7);
    cout << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << " " << arr[4] << " " << arr[5] << " " << arr[6] << endl;
    cout << "Swaps: " << swap << " Comparisons: " << comparison; 

}