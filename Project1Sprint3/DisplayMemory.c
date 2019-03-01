#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "Project1.h"

void Display_Memory(struct UserData* USERDATA_PTR){  
    if(USERDATA_PTR->Words_To_Display==0){
        printf("Data Located at Address:%p = %d\n", USERDATA_PTR->GlobalPTR + USERDATA_PTR->Display_Offset_Bytes, *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Display_Offset_Bytes));
    }
    else{
        for(int i = 0; i < USERDATA_PTR->Words_To_Display; i++){
            printf("Data Located at Address:%p = %d\n", USERDATA_PTR->GlobalPTR+(USERDATA_PTR->Display_Offset_Bytes+i), *(USERDATA_PTR->GlobalPTR + (USERDATA_PTR->Display_Offset_Bytes + i) ));
        } 
    }       
}

void Interpret_Display_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if ( StrCmp(token, "Display") != 0){
        return;
    }
    token = strtok(NULL, " ");
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
        
        //Check to see if the user input a character and that the offset is less than the number of bytes the user allocated
        int Offset = atoi(token);
        if(IsChar(token[0]) == 1){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return;
        }
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Display_Offset_Bytes=Offset;            
        }
        token = strtok(NULL, " ");
        if (token != NULL){
            int Words_To_Display = atoi(token);
            //Ensure the user typed an integer
            if (Words_To_Display == 0){
                printf("ERROR: Please specify a nonzero integer to write to memory\n");
                return;
            }
            else if( (Words_To_Display+USERDATA_PTR->Display_Offset_Bytes) > USERDATA_PTR->Bytes_To_Allocate ){
                printf("ERROR: You cannot display words past the memory you allocated\n");
                return;
            }
            USERDATA_PTR->Words_To_Display=Words_To_Display;
            if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
                printf("ERROR: You can't display memory you haven't written or allocated");
            }
            else{
                Display_Memory(USERDATA_PTR);
            }
            return;
        }
        else{
            USERDATA_PTR->Words_To_Display=0;
            if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
                printf("ERROR: You can't display memory you haven't written or allocated");
            }
            else{
                Display_Memory(USERDATA_PTR);
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
            USERDATA_PTR->Display_Offset_Bytes=Offset;            
        }
        else{
            printf("ERROR: Invalid address specified\n");
            return;
        }
        token = strtok(NULL, " ");
        if (token != NULL){
            int Words_To_Display = atoi(token);
            //Ensure the user typed an integer
            if (Words_To_Display == 0){
                printf("ERROR: Please specify a nonzero integer to write to memory\n");
                return;
            }
            else if( (Words_To_Display+USERDATA_PTR->Display_Offset_Bytes) > USERDATA_PTR->Bytes_To_Allocate ){
                printf("ERROR: You cannot display words past the memory you allocated\n");
                return;
            }
            USERDATA_PTR->Words_To_Display=Words_To_Display;            
            if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
                printf("ERROR: You can't display memory you haven't written or allocated");
            }
            else{
                Display_Memory(USERDATA_PTR);
            }
            return;
        }
        else{
            USERDATA_PTR->Words_To_Display=0;
            if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
                printf("ERROR: You can't display memory you haven't written or allocated");
            }
            else{
                Display_Memory(USERDATA_PTR);
            }
            return;
        }
    }       
}