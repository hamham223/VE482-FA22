#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    int i = 0;
    int * large_array = NULL;

    while ( 1 ) {
    	usleep(1);
        large_array = malloc(sizeof(int)*100000000);
        printf("%d-th times of allocation: %lld\n",++i,(long long)large_array);
    }
    
    return 0;
}
