#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------
int randomInt( int min, int max ){

    srand( time(NULL) );
    return ( (rand() / ( int ) RAND_MAX) * ( max - min ) ) + min;
    
}
//------------------------------------------------------------------------------
int randomFloat( float min, float max ){

    srand( time(NULL) );
    return ( (rand() / (float) RAND_MAX) * (max - min) + min );

}
//------------------------------------------------------------------------------
