#include "the4.h"



int recursive_sln(int i, int*& arr, int &number_of_calls){ //direct recursive
    number_of_calls+=1;

    if(i <= 2) //size = 3 or less
    {
        int max = 0;
        for(int j=0; j<i+1; j++)
            if(max < arr[j]) max = arr[j];
        return max;
    }
    int t = recursive_sln(i-3, arr, number_of_calls) + arr[i];
    int y = recursive_sln(i-1, arr, number_of_calls);


    return (t>y ? t:y);
}


int memoization_sln(int i, int*& arr, int*& mem){ //memoization
    int t, y;
    if(i <= 2) //size = 3 or less
    {
        int max = 0;
        for(int j=0; j<i+1; j++)
            if(max < arr[j]) max = arr[j];
        mem[i] = max;
        return max;
    }

    if(mem[i-3] == -1) 
            t = memoization_sln(i-3, arr, mem) + arr[i];
    else t = mem[i-3] + arr[i];

    if(mem[i-1] == -1) 
            y = memoization_sln(i-1, arr, mem);
    else y = mem[i-1];
    

    mem[i] = (t>y ? t:y);
    return mem[i];
}

int dp_sln(int size, int*& arr, int*& mem){ //dynamic programming
    int t, y, i = size-1;
    if(i <= 2) //size = 3 or less
    {
        int max = 0;
        for(int j=0; j<i+1; j++)
            if(max < arr[j]) max = arr[j];
        mem[i] = max;
        return max;
    }

    if(mem[i-3] == -1) 
            t = memoization_sln(i-3, arr, mem) + arr[i];
    else t = mem[i-3] + arr[i];

    if(mem[i-1] == -1) 
            y = memoization_sln(i-1, arr, mem);
    else y = mem[i-1];
    

    mem[i] = (t>y ? t:y);
    return mem[i];

    return 0; // this is a dummy return value. YOU SHOULD CHANGE THIS!
}

