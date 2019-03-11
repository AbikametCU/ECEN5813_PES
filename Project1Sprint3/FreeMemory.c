#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "Project1.h"

void Free_Memory(uint32_t *MemoryPTR){
    free(MemoryPTR);
}

void Interpret_Free_Input(char* User_Input, struct UserData *USERDATA_PTR){
    if( (StrCmp(User_Input, "Free Memory\n") == 0) ){
        if(USERDATA_PTR->Allocate_State == 0){
	#if LINUX_COMPILATION
	printf("ERROR: You can't free memory that hasn't been allocated\n");
	#else
	PRINTF("ERROR: You can't free memory that hasn't been allocated\n");
	#endif
        }
        Free_Memory(USERDATA_PTR->GlobalPTR);
	#if LINUX_COMPILATION
	printf("Memory Successfully Freed\n");
	#else
	PRINTF("Memory Successfully Freed\n");
	#endif
        USERDATA_PTR->Allocate_State = 0;
        USERDATA_PTR->Write_State = 0;
        return;
    }
}
