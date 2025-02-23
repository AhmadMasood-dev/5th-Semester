#include <bits/stdc++.h>
using namespace std;
 
int maxBit(int a[], int n)
{
    int max = a[0], count = 0;
    for (int i = 1; i < n; i++){
        if (max < a[i])
            max = a[i];
    }
    while(max){
        max = max / 10;
        count++;
    }
    return count;
}

 
void redixSort(int arr[], int n, int count)
{
    queue<int> q[10];
    for (int r = 1; r <= count; r++){
        int k = 0;
        for (int i = 0; i < n; i++)
            q[(arr[i]/(int)pow(10,r-1))%10].push(arr[i]);
        for (int j = 0; j <= 9; j++)
            while (!q[j].empty()){
                arr[k++] = q[j].front();
                q[j].pop();
            }
    }
}
 
void printArray(int arr[], int size)
{
    for (int i=0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}
 
int main()
{
    cout<<"enter the number of the element you wanna to enter ";
	int n;
	cin>>n;
    int arr[n];
    for(int i=0;i<n;i++)
	{cout<<"\nEnter the value "<<i+1<<" ";
		cin>>arr[i];

	}
    
    int count = maxBit(arr, n);
    redixSort(arr, n, count);
    cout << "Sorted array: \n";
    printArray(arr, n);
    return 0;
}