#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "FreeMemory.h"
#include "Project1.h"

void Allocate_Memory(struct UserData* USERDATA_PTR){
    //If the user calls the function twice, clear the memory
    if(USERDATA_PTR->Allocate_State==1){
        Free_Memory(USERDATA_PTR->GlobalPTR);
	#if LINUX_COMPILATION
	printf("Previously Allocated Memory was freed\n");
	#else
	PRINTF("Previously Allocated Memory was freed\n");
	#endif
    }
    
    USERDATA_PTR->GlobalPTR = (uint32_t*) malloc( (USERDATA_PTR->Bytes_To_Allocate) * MEMORY_WORD);
    if (USERDATA_PTR->GlobalPTR==NULL){
	#if LINUX_COMPILATION
	printf("Memory Allocation Failed\n");
	#else
	PRINTF("Memory Allocation Failed\n");
	#endif
    }
    else{
	#if LINUX_COMPILATION
	        printf("\n%d 32-bit chunks of memory allocated successfully starting at location%p\n", USERDATA_PTR->Bytes_To_Allocate, USERDATA_PTR->GlobalPTR);
    }
	#else
	        PRINTF("\n%d 32-bit chunks of memory allocated successfully starting at location%p\n", USERDATA_PTR->Bytes_To_Allocate, USERDATA_PTR->GlobalPTR);
    }
	#endif
    USERDATA_PTR->Allocate_State = 1;
}

void Interpret_Allocate_Input(char* User_Input, struct UserData *USERDATA_PTR){
    if( (StrStr(User_Input, "Allocate Memory") != NULL) ){
        char *token = strtok(User_Input, " "); 
        if ( StrCmp(token, "Allocate") != 0){
            return;
        }
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");
        if (token == NULL){
	    #if LINUX_COMPILATION
		printf("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
	    #else
		PRINTF("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
	    #endif
            return;
        }
        int Bytes_To_Allocate = atoi(token);
        //Ensure the user typed an integer
        if (Bytes_To_Allocate == 0){
	    #if LINUX_COMPILATION
		printf("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
	    #else
		PRINTF("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
#endif
            return;
        }
        else{
            USERDATA_PTR->Bytes_To_Allocate = Bytes_To_Allocate;
            Allocate_Memory(USERDATA_PTR);
            return;
        }
    }
}
