

extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }

#include <iostream>
#include <iomanip>
#include <conio.h> 
#include <pthread.h>
#include <iostream>

using namespace std;
#include <iostream>
#include <cstdlib>
#include <pthread.h>


#include <stdio.h> 
#define NUM_THREADS 5 
using namespace std;
void *PrintHello(void *threadid) { 

   long *tid;
tid = (long*)threadid; 
//tid=*((int*)(&t));
cout<<"Hello World!  thread no:"<<tid<<endl;

	//pthread_exit(NULL); 
} 

int main () { 
	pthread_t threads[NUM_THREADS]; 
	int rc, t; 
	for(t=0; t<NUM_THREADS; t++){ 
		
		cout<<"In main: creating thread "<< t<<endl; 
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t); 
		if (rc) { 
			cout<<"ERROR code is\n"<<rc; 
			exit(-1); 
		} 
	} 
	pthread_exit(NULL); 
} 

