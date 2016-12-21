#include <iostream>

using namespace std;

#define NUM_THREADS 5


int main () {
   int rc;
   int i;
	
   #pragma omp parallel
   for( i=0; i < 1000000; i++ ){
      cout << "HI" << endl;
   }
}