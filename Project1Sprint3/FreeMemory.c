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
            printf("ERROR: You can't free memory that hasn't been allocated\n");
        }
        Free_Memory(USERDATA_PTR->GlobalPTR);
        printf("Memory Successfully Freed\n");
        USERDATA_PTR->Allocate_State = 0;
        USERDATA_PTR->Write_State = 0;
        return;
    }
}