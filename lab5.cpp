#include <cstdlib>
#include <iostream>
#include <fstream>
#include "timer.h"
#include <string>
#include <vector>

using namespace std;

void insertionSort(int arr[],int left,int right){
    int curr;
    int i, j;
    for(i=left+1;i<=right;i++){
        curr=arr[i];
        for(j=i;j>0 && arr[j-1] > curr;j--){
            arr[j]=arr[j-1];
        }
        arr[j]=curr;
    }
}

//quicksort has been slightly modified to take threshold as an argument
//when size of partition is at or below threshold, insertion sort is done
void quickSort(int arr[], int left, int right, string option, int threshold){
    if(right-left <= threshold){
        insertionSort(arr,left,right);
    }
    else{
        int i=left;
        int j=right-1;
        
        /* whatever you do to choose pivot, put the code here
         ensure pivot is at end of array.  For example if
         you choose middle of array as pivot then swap it
         with arr[right]*/
        
        //Part 2
        int pivot = -1;
        if(!option.compare("last")){
            pivot=arr[right];   //set last value pivot
        }
        if(!option.compare("middle")){
            //Set Middle as Pivot and swapping it with the last most element
            int middle = left + (right - left) / 2;
            swap(arr[middle], arr[right]);
            pivot = arr[right];
        }
        if(!option.compare("median")){
            //Median of 3
            int middle = left + (right - left) / 2;
            if(arr[left] > arr[middle]){
                swap(arr[left], arr[right]);
            }
            if(arr[left] > arr[right]){
                swap(arr[left], arr[right]);
            }
            if(arr[middle] > arr[right]){
                swap(arr[middle], arr[right]);
            }
            swap(arr[middle], arr[right]);
            pivot = arr[right];
        }
        
        int pivotpt=right;   //NOTE: don't change this, pivot needs
        //to be at end of array to be out of the way
        //regardless of pivot picking method
        
        while(i<j){
            while(i< right-1 && arr[i]<pivot) i++;
            while(j > 0 && arr[j]>pivot) j--;
            if(i<j)
                swap(arr[i++],arr[j--]);
        }
        if(i==j && arr[i] < arr[pivotpt])
            i++;
        swap(arr[i],arr[pivotpt]);
        quickSort(arr,left,i-1,option,threshold);
        quickSort(arr,i+1,right,option,threshold);
    }
}

//non-recursive interface to quicksort, real work
//done in recursive version.
void quickSort(int arr[],int size,string option, int threshold=3){
    quickSort(arr, 0, size-1, option, threshold);
}

/*This function is passed an empty array of size elements
 it will fill the array with random numbers*/
void generateRandom(int array[],int size){
    for(int i=0;i<size;i++){
        array[i]=rand();
    }
}

/*This function is passed an empty array with sz elements. It will
 fill this array with numbers where the numbers are ordered is ordered
 from big to small. Note that the values in here need not be random
 only that it goes from biggest to smallest*/
void generateReverse(int array[],int size){
    int random = size + (rand() % size);
    for(int i = size-1; i >= 0; i--){
        if(random >= 0){
            array[i] = random--;
        }
    }
}


/*This function is passed an empty array with size elements.
 This function generate 20 random numbers. Each element of the
 array will get one of these 20 numbers.*/
void generateManyDuplicates(int array[],int size){
    int length = 20;
    unsigned int random[length];
    for(int i = 0; i < length; i++){
        random[i] = rand();
    }
    for(int i = 0; i < size; i++){
        array[i] = random[rand() % length];
    }
}

/*This function is passed an empty array with size elements.
 This function generates a nearly sorted array (small to big).
 This can be done by creating a sorted array(doesn't have to be
 random numbers...just small to big) and then randomly
 performing a small number of swaps on the array. The small
 number can be based on percentage of size around 5%. Thus if
 sz was 100, you would do 5 swaps (making around 10 numbers
 out of place, leaving 90 sorted)*/

void generateNearlySorted(int array[],int size){
    double n = (rand() % 100); //Getting a percentage between 0 and 100
    int swaps = 0;
    if(n > 0){
        swaps = (n/100)*size;
    }
    for(int i = 0; i < size; i++){
        array[i] = i+1;
    }
    for(int i = 0; i < swaps; i++){
        int l = (rand() % size);
        int r = (rand() % size);
        swap(array[l], array[r]);
    }
}

void copy(int rhs[], int lhs[], int size){
    if(size > 0){
        for(int i = 0; i < size; i++){
            lhs[i] = rhs[i];
        }
    }
}

void print(int arr[], int size){
    if(size > 0){
        for(int i = 0; i < size; i++){
            cout << arr[i] << ", ";
        }
    }
    cout << endl;
}

int main(int argc, char* argv[]){
    unsigned int seed = 7;
    srand(seed);
    //int size=atoi(argv[1]);
    int size = 10000000;
    //int threshold = 3;
    ofstream log("quick.log");
    Timer stopwatch;
    vector<double> lastTime;
    vector<double> middleTime;
    vector<double> medianTime;
    
    //Part 3
    
    int threshold = 0;
    int* arr;
    string option = "median";
    for(threshold = 5; threshold <= 10; threshold+=5){ 
    	arr = new int[size];
    	generateReverse(arr, size);
    	stopwatch.start();
    	quickSort(arr, size, option, threshold);
    	stopwatch.stop();
    	//cout << "Threshold: " << threshold << endl;
    	cout << stopwatch.currtime() << endl;
    	delete[] arr;
    }
    threshold = 50;
    arr = new int[size];
    generateReverse(arr, size);
    stopwatch.start();
    quickSort(arr, size, option, threshold);
    stopwatch.stop();
    //cout << "Threshold: " << threshold << endl;
    cout << stopwatch.currtime() << endl;
    delete[] arr;
    
    for(threshold = 100; threshold <= 1000; threshold+=100){
        arr = new int[size];
        generateReverse(arr, size);
        stopwatch.start();
        quickSort(arr, size, option, threshold);
        stopwatch.stop();
        //cout << "Threshold: " << threshold << endl;
        cout << stopwatch.currtime() << endl;
        delete[] arr;
    }
    

    //Part 2
    /*
     if(size > ){
         for(int i = 0; i < 20; i++){
            cout << "Size: " << size << endl;
            int* lastPivot = new int[size];
            int* middlePivot = new int[size];
            int* medianPivot = new int[size];
     
            //Uncomment one of the below functions to generate an array of numbers.
            //generateRandom(lastPivot,size);
            //generateReverse(lastPivot, size); //Segmentation Fault
            //generateManyDuplicates(lastPivot, size);
            generateNearlySorted(lastPivot, size);
     
            //Copy
            copy(lastPivot, middlePivot, size);
            copy(lastPivot, medianPivot, size);
     
            stopwatch.start();
            quickSort(lastPivot, size, "", threshold);
            stopwatch.stop();
            lastTime.push_back(stopwatch.currtime());

            stopwatch.start();
            quickSort(middlePivot, size, "", threshold);
            stopwatch.stop();
            middleTime.push_back(stopwatch.currtime());
     
            stopwatch.start();
            quickSort(medianPivot, size, "", threshold);
            stopwatch.stop();
            medianTime.push_back(stopwatch.currtime());
     
            delete[] lastPivot;
            delete[] middlePivot;
            delete[] medianPivot;
            size+=5000;
        }
     }
     */
    
    
    
    
    
    if(lastTime.size() > 0){
        log << "Time Results for Last Value Pivot: " << endl;
        for(int i = 0; i < lastTime.size(); i++){
            log << lastTime.at(i) << endl;
        }
    }
    if(middleTime.size() > 0){
        log << "\nTime Results for Middle Value Pivot: " << endl;
        for(int i = 0; i < lastTime.size(); i++){
            log << middleTime.at(i) << endl;
        }
    }
    
    if(lastTime.size() > 0){
        log << "\nTime Results for Median Value Pivot: " << endl;
        for(int i = 0; i < lastTime.size(); i++){
            log << medianTime.at(i) << endl;
        }
    }
    for(int i=0;i<size;i++){
        //log << myarr[i]<< endl;
    }
    
    return 0;
}
