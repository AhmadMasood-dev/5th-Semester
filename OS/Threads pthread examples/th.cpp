//#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//int main(int argc, char** argv) {
//	return 0;
//}
extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }
#include <iostream>
#include <pthread.h>
using namespace std;

void * fun_thread1(void *data)
{
    for(int i=1;i<5;i++)
    { 
        cout<<endl<<"In Thread 1"<<endl;
        //sleep(1);
    }     
}
void * fun_thread2(void *data)
{
    for(int i=1;i<5;i++)
    { 
        cout<<endl<<"In Thread 2"<<endl;
        //sleep(1);
    }     
}

//int main(int argc, char *argv[])
int main()
{
    int status;
    // creating thread objects
    pthread_t thrd_1;
   pthread_t thrd_2;
    // create thread
   // pthread_create(&thrd_1,NULL,fun_thread1,(void *)0);
    pthread_create(&thrd_1,NULL,fun_thread1,NULL);
    
    pthread_create(&thrd_2,NULL,fun_thread2,NULL);    
  
pthread_join(thrd_2, NULL);
//pthread_join(thrd_1, NULL);

//pthread_join(thrd_2, (void **)&status);
//pthread_join(thrd_1, (void **)&status);
    //system("PAUSE");
    //return EXIT_SUCCESS;
    return 0;
}
