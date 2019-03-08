#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "Project1.h"

void Write_Memory(struct UserData* USERDATA_PTR){    
    //Shift the address by the offset and dereference it to assign the value
    *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Write_Offset_Bytes) = USERDATA_PTR->Bytes_To_Write;
    printf("Wrote %d at Memory Location:%p\n", *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Write_Offset_Bytes), USERDATA_PTR->GlobalPTR+USERDATA_PTR->Write_Offset_Bytes);
    //printf("MemoryPTR:%p, WritePTRaddr:%p, Data:%d\n", *MemoryPTR, *MemoryPTR+USERDATA_PTR->Write_Offset_Bytes, *(*MemoryPTR + USERDATA_PTR->Write_Offset_Bytes));
}

void Interpret_Write_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if ( StrCmp(token, "Write") != 0){
        return;
    }
    token = strtok(NULL, " ");
    if ( StrCmp(token, "Memory") != 0){
        printf("ERROR: Inccorect syntax of the Write Memory command, please type 'Help' for more info");
        return;
    }
    token = strtok(NULL, " ");
    if (token == NULL){
            printf("ERROR:Please specify an address to write to\n");
            return;
        }
    //If the user specified to write an offset of memory 
    if ( (StrCmp(token, "-o") == 0) ){
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a memory address offset and an integer to write to it\n");
            return;
        }
        
        //Check to see if the user input a character and not a number offset and that the offset is less than the number of bytes the user allocated
        int Offset = atoi(token);
        if(IsChar(token[0]) == 1){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return;
        }
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate || Offset < 0){
            printf("ERROR: Please specify a proper non-negative memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Write_Offset_Bytes=Offset;
        }
        token = strtok(NULL, " ");        
        if (token == NULL){
            printf("ERROR:Please specify a number to write to memory\n");
            return;
        }
        int Bytes_To_Write = atoi(token);
        //Ensure the user typed an integer
        if (Bytes_To_Write == 0){
            printf("ERROR: Please specify a nonzero integer to write to memory\n");
            return;
        }
        else{
            USERDATA_PTR->Bytes_To_Write = Bytes_To_Write;
            if (USERDATA_PTR->Allocate_State == 0){
                printf("ERROR: You can't write to memory you have not allocated\n");
            }
            else{
                Write_Memory(USERDATA_PTR);
                USERDATA_PTR->Write_State = 1;
            }
            return;
        }
    }
    //If the user provided a hexidecimal address
    else{
        token[strlen(token)] = '\0';
        if (token == NULL){
            printf("ERROR:Please specify an address to write to\n");
            return;
        }
        long int UserAddress = strtol(token, NULL, 16);
        //Check if the address the user provided is within range
        if ( (UserAddress >= (long int)USERDATA_PTR->GlobalPTR) && (UserAddress <= ((long int)USERDATA_PTR->GlobalPTR+USERDATA_PTR->Bytes_To_Allocate)) ){
            long int Offset = (UserAddress-(long int)USERDATA_PTR->GlobalPTR)/4;
            USERDATA_PTR->Write_Offset_Bytes=Offset;            
        }
        else{
            printf("ERROR: Invalid address specified\n");
            return;
        }
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR:Please specify a number to write to memory\n");
            return;
        }
        int Bytes_To_Write = atoi(token);        
        //Ensure the user typed an integer
        if (Bytes_To_Write == 0){
            printf("ERROR: Please specify a nonzero integer to write to memory\n");
            return;
        }
        else{
            USERDATA_PTR->Bytes_To_Write = Bytes_To_Write;
            if (USERDATA_PTR->Allocate_State == 0){
                printf("ERROR: You can't write to memory you have not allocated\n");
            }
            else{
                Write_Memory(USERDATA_PTR);
                USERDATA_PTR->Write_State = 1;
            }
            return;
        }
    }       
}
