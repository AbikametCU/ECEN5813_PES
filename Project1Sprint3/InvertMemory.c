#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "Project1.h"

void Invert_Memory(struct UserData* USERDATA_PTR){  
    if(USERDATA_PTR->Invert_Length ==0){
        clock_t invertStartTime=clock();
        *(USERDATA_PTR->GlobalPTR+USERDATA_PTR->Invert_Offset_Bytes)= ~ *(USERDATA_PTR->GlobalPTR+USERDATA_PTR->Invert_Offset_Bytes);
        clock_t invertEndTime=clock();
        printf("Inverted Data Located at Address:%p = %d\n", USERDATA_PTR->GlobalPTR+USERDATA_PTR->Invert_Offset_Bytes, *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Invert_Offset_Bytes ));
        double time_taken_to_invert = ((double)(invertEndTime-invertStartTime)*1000)/CLOCKS_PER_SEC;
        printf("Time taken to perform invert:%f ms\n",time_taken_to_invert);
    }
    else{
        clock_t invertStartTime=clock();
        for(int i = USERDATA_PTR->Invert_Offset_Bytes; i < (USERDATA_PTR->Invert_Offset_Bytes+USERDATA_PTR->Invert_Length); i++){
            *(USERDATA_PTR->GlobalPTR+i)= ~ *(USERDATA_PTR->GlobalPTR+i);
            printf("Inverted Data Located at Address:%p = %d\n", USERDATA_PTR->GlobalPTR+i, *(USERDATA_PTR->GlobalPTR + i ));
        }
        clock_t invertEndTime=clock();
        double time_taken_to_invert = ((double)(invertEndTime-invertStartTime)*1000)/CLOCKS_PER_SEC;
        printf("Time taken to perform invert:%f ms\n",time_taken_to_invert);
    }
    
}

void Interpret_Invert_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if (StrCmp(token, "Invert") != 0){
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
        
        //Check to see if the user input a character and that the offset is less than the number of bytes the user allocated
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
            USERDATA_PTR->Invert_Offset_Bytes=Offset;
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
            int length = atoi(token);
            if(length == 0){
                printf("ERROR: Please specify a nonzero length to invertn");
                return;
            }
            else{
                USERDATA_PTR->Invert_Length = length;
            }
        }        
        if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
            printf("ERROR: You can't invert memory you haven't written or allocated");
        }
        else{
            Invert_Memory(USERDATA_PTR);
        }
        return;                           
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
            USERDATA_PTR->Invert_Offset_Bytes=Offset;   
        }
        else{
            printf("You cannot invert memory you have not allocated");
            return;
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
            int length = atoi(token);
            if(length == 0){
                printf("ERROR: Please specify a nonzero length to invert\n");
                return;
            }
            else{
                USERDATA_PTR->Invert_Length = length;
            }
        }        
        if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
            printf("ERROR: You can't invert memory you haven't written or allocated");
        }
        else{
            Invert_Memory(USERDATA_PTR);
        }
        return;  
    }       
}
