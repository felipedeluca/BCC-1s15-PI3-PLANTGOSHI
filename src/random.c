#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//------------------------------------------------------------------------------
int randomInt( int min, int max ){

//    printf("\n-------RAND(): %d", (rand() % max) + 1 );
//    return ((rand() / (RAND_MAX * 1.00)) * 10 * ( max - min )) + min;
//    printf("\nMIN: %d   MAX: %d", min, max);
    return( (rand() % max) + 1);

}
//------------------------------------------------------------------------------
float randomFloat( float min, float max ){

    return ( ( rand() % (int)max) + 1 );

}
//------------------------------------------------------------------------------
