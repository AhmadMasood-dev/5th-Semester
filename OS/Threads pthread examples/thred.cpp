extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }

#include <iostream>
#include <pthread.h>

using namespace std ;

void  * function1(void * argument);
void  * function2(void * argument);

int main( void )
{
    pthread_t t1, t2 ; // declare 2 threads.
    pthread_create( &t1, NULL, function1,NULL); // create a thread running function1
    pthread_create( &t2, NULL, function2,NULL); // create a thread running function2

    // Because all created threads are terminated when main() finishes, we have
    // to give the threads some time to finish. Unfortunately for function1, main()
    // will give only 1 second, but function1 needs at least 2 seconds. So function1 will
    // probably be terminated before it can finish. This is a BAD way to manage threads.
    sleep(0);
//pthread_join(&t2, NULL);
    return 0;
}

void * function1(void * argument)
{
    cout << " hello nnnnnnn " << endl ;
    sleep(2); // fall a sleep here for 2 seconds...
    return 0;
}

void * function2(void * argument)
{
    
	cout << " world "  << endl ;
     
    return 0;
}
