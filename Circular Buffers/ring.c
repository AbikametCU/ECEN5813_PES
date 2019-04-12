/*========================================================================
** Circular buffer
** ECEN5813
**========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include "ring.h"

ring_t *Ring_init( int length ){
    //Initialize a structure to return
    ring_t *RingPTR = malloc(sizeof(ring_t));
   
    //allocate memory for the buffer
    char *buff = (char *)malloc(length * sizeof(char));
    
    //Populate the struct and return it
    RingPTR->Length = length;
    RingPTR->Ini = 0;
    RingPTR->Outi = 0;
    RingPTR->Buffer=buff;
    
    return RingPTR;
}
int inPlus(int in, ring_t *ring){
    return (in > ring->Length ? 0 : in++);
}

int insert( ring_t *ring, char data ){
    //if(inPlus(ring->Ini, ring) == ring->Outi){
      //  printf("Buffer is full, please remove an element");
       // return -1;
   // }
    if(ring->Ini>=ring->Length ){
            printf("Buffer is full\n");
    	ring->Ini = 0;
		return -1;
    }
    else{
        ring->Buffer[ring->Ini] = data;
        ring->Ini++;
//    	printf("Added\n");
    }    
    return 0;
    
}

int remove_element( ring_t *ring, char *data ){
    if(ring->Ini == ring->Outi){
        printf("cannot remove an item from an empty buffer\n");
        return -1;
    }
    else{
        *data = ring->Buffer[ring->Outi];
        ring->Outi++;
        if(ring->Outi >= ring->Length){
			ring->Outi=0;
		}
//        printf("Item successfully removed\n");
        if(ring->Ini>=ring->Length ){
            ring->Ini=0;
//            printf("Ini back to 0\n");
        }
        return 0;
    }
}

int entries( ring_t *ring ){
    if ( (ring->Ini-ring->Outi) < 0 ){
        return ( (ring->Ini-ring->Outi) + ring->Length);
    }
    else{
        return (ring->Ini-ring->Outi);
    }    
}
