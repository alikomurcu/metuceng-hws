 #include<iostream>

 using namespace std;

/*
sillySort() should count the number of $comparison$ and $swap$ executed during sorting proccess 
(Comparisons are only between the values to be sorted only, not your auxiliary comparisons) and 
return the number of calls of sillySort() (which is 1 in minimum).
crossMergeSort() should count the number of $comparison$ executed during sorting proccess 
(Comparisons are only between the values to be sorted only, not your auxiliary comparisons) and
 return the number of calls of crossMergeSort() (which is 1 in minimum).

Silly sorting algortihm (sillySort()) is as follows:
assume the input array A[1..N] is divided into 4 quarters as q1=A[1..N/4], q2=A[N/4+1..N/2], q3=A[N/2+1..3N/4], q4=A[3N/4+1..N]
do 6 recursive calls as follows when N>=4 otherwise sort the list with N<4 elements directly.
sillysort: q1 and q2 (sillysort A[1..N/2] )
sillysort: q2 and q3 (sillysort A[N/4+1..3N/4] )
sillysort: q3 and q4 (sillysort A[N/2+1..N] )
sillysort: q1 and q2
sillysort: q2 and q3
sillysort: q1 and q2
when the input size  N<=2 no recursion. (do nothing for N=0 or 1 and just apply swap when N =2)
to make things simpler we will only use N as a power of 2 on our tests 
(although not necessary with non-rec termination conditions).
It is an in-place algorithm, so no merging is needed. Nothing else is needed after the recursive calls.
count the swap between any 2 elements of the array A, such as swapping A[i] and A[j].
count the comparison between any 2 elements of the array A, such as A[i]>A[j]
return the total number of calls to sillySort()
*/

int sillySort(int* arr, long &comparison, long & swap, int size) 
{

    int num_of_calls=1;
	if(size <= 2)
    {
        if(size<2) return num_of_calls;
        comparison++;
        if(arr[0] > arr[1])
        {
            int tmp = arr[0];
            arr[0] = arr[1];
            arr[1] = tmp;
            swap++;
            return num_of_calls;
        }
    }
    else
    {   
        int count = 4; 
        num_of_calls += sillySort(arr, comparison, swap, size/2); // q1 and q2
        num_of_calls += sillySort(arr + size/4, comparison, swap, size/2); // q2 and q3
        num_of_calls += sillySort(arr + size/2, comparison, swap, size/2); // q3 and q4
        num_of_calls += sillySort(arr, comparison, swap, size/2); // q1 and q2
        num_of_calls += sillySort(arr + size/4, comparison, swap, size/2); // q2 and q3
        num_of_calls += sillySort(arr , comparison, swap, size/2); // q1 and q2
    }

//    num_of_calls += sillySort(arr, comparison, swap, size/2) + sillySort(arr + size/4, comparison, swap, size/2) + sillySort(arr + 3*size/4, comparison, swap, size/2) + sillySort(arr, comparison, swap, size/2) + sillySort(arr + size/4, comparison, swap, size/2) + sillySort(arr , comparison, swap, size/2); 
	return num_of_calls;
}


// Merge two subarrays L and M into arr
void merge(int *arr, int p, int q, int r, long &comparison) {

    // Create L ← A[p..q] and M ← A[q+1..r]
    int n1 = q - p + 1;
    int n2 = r - q;

    int L[n1], M[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    // Maintain current index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    // Until we reach either end of either L or M, pick larger among
    // elements L and M and place them in the correct position at A[p..r]
    
    while (i < n1 && j < n2) {
        comparison++;
        if (L[i] <= M[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    // When we run out of elements in either L or M,
    // pick up the remaining elements and put in A[p..r]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = M[j];
        j++;
        k++;
    }
}

/*
int mergeSort(int* arr, int l, int r, long &comparison)
{
  if (l < r) {
    // m is the point where the array is divided into two subarrays
    int m = l + (r - l) / 2;

    mergeSort(arr, l, m, comparison);
    mergeSort(arr, m + 1, r, comparison);

    // Merge the sorted subarrays
    merge(arr, l, m, r, comparison);
  }
  return 1;
}*/


int crossMergeSort(int *arr, long &comparison, int size)
{
    int num_of_calls = 1;
    if(size<=2) //i.e. size = 2 or size = 1 or size = 0
    {
        if(size<2) return num_of_calls;
        comparison++;
        if(arr[0] > arr[1])
        {
            int tmp = arr[0];
            arr[0] = arr[1];
            arr[1] = tmp;
        }
        return num_of_calls;
    }
    if(size >= 4)
    {
        num_of_calls += crossMergeSort(arr, comparison, size/4);
        num_of_calls += crossMergeSort(arr + size/4, comparison, size/4);
        num_of_calls += crossMergeSort(arr + size/2, comparison, size/4);
        num_of_calls += crossMergeSort(arr + 3*size/4, comparison, size/4);

        int * h1 = new int[size/2];
        int * h2 = new int[size/2];

        for (int i = 0; i < size/4; i++)
        {
            h1[i] = arr[i];  // q1
            h2[i] = arr[size/4 + i];  // q2
            h1[size/4 + i] = arr[size/2 + i];  // q3
            h2[size/4 + i] = arr[3*size/4 + i];  // q4
        }
        merge(h1, 0, (size/2-1)/2, size/2-1, comparison);
        merge(h2, 0, (size/2-1)/2, size/2-1, comparison);
        
        for (int i = 0; i < size/2; i++)
        {
            arr[i] = h1[i];
            arr[size/2 + i] = h2[i];
        }           

        merge(arr, 0, (size-1)/2, size-1, comparison);
        return num_of_calls;
    }
}

int main()
{
    int count = 16, num = count;
    int *arr = new int[count];
    for (int i = 0; i < count; i++) arr[i] = num--;
    for (int j = 0; j < count; j++) cout<< arr[j] << " ";
    cout << endl;
    
    cout << endl;
    long s, c = 0;
    int x = crossMergeSort(arr, c, count);
  
      cout << "comp: " << c << endl;

    cout << endl;

    for (int k = 0; k < count; k++)
    {
        cout<<arr[k]<<" ";
    }
/*
    int arr[] = {1,2,3,4,5,6,7,8};
    int *h1 = new int[4];
    int *h2 = new int[4];
    int size = 8;

    for (int i = 0; i < size/4; i++)
    {
        h1[i] = arr[i];  // q1
        h2[i] = arr[size/4 + i];  //q2
        h1[size/4 + i] = arr[size/2 + i];
        h2[size/4 + i] = arr[3*size/4 + i];
    }
    for (int i = 0; i < size/2; i++)
    {
        cout << h1[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < size/2; i++)
    {
        cout << h2[i] << " ";
    }

    for (int i = 0; i < size/2; i++)
    {
        arr[i] = h1[i];
        arr[size/2 + i] = h2[i];
    }       
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    } 
    */
}