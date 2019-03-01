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
        printf("Previously Allocated Memory was freed\n");
    }
    
    USERDATA_PTR->GlobalPTR = (uint32_t*) malloc( (USERDATA_PTR->Bytes_To_Allocate) * MEMORY_WORD);
    if (USERDATA_PTR->GlobalPTR==NULL){
        printf("Memory Allocation Failed\n");
    }
    else{
        printf("\n%d 32-bit chunks of memory allocated successfully starting at location%p\n", USERDATA_PTR->Bytes_To_Allocate, USERDATA_PTR->GlobalPTR);
    }
    USERDATA_PTR->Allocate_State = 1;
}

void Interpret_Allocate_Input(char* User_Input, struct UserData *USERDATA_PTR){
    printf("Made it2\n");
    if( (StrStr(User_Input, "Allocate Memory") != NULL) ){
        char *token = strtok(User_Input, " "); 
        if ( StrCmp(token, "Allocate") != 0){
            return;
        }
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
            return;
        }
        int Bytes_To_Allocate = atoi(token);
        //Ensure the user typed an integer
        if (Bytes_To_Allocate == 0){
            printf("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
            return;
        }
        else{
            USERDATA_PTR->Bytes_To_Allocate = Bytes_To_Allocate;
            Allocate_Memory(USERDATA_PTR);
            return;
        }
    }
}