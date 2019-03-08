#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdint.h>
#include "Project1.h"
#include "WritePattern.h"


void Verify(struct UserData *USERDATA_PTR){
    if(USERDATA_PTR->Verify_Length == 0){
        uint32_t randomVal = *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset);
        
        uint32_t VerifyVal = ((USERDATA_PTR->Verify_Seed_Value^USERDATA_PTR->Seed_Time) * ((uint32_t)USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset) ) % 2147483648;
        
        if(randomVal!=VerifyVal){
            printf("Verify discrepancy at memory location %p, expected %d but found %d\n", USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset, VerifyVal, randomVal);
        }
        else{
            printf("Verified the value %d at location%p\n", VerifyVal, USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset);
        }
    }
    else{
        for (int i = USERDATA_PTR->Verify_Pattern_offset; i<(USERDATA_PTR->Verify_Pattern_offset+USERDATA_PTR->Verify_Length); i++){
            uint32_t randomVal = *(USERDATA_PTR->GlobalPTR + i);

            uint32_t VerifyVal = ((USERDATA_PTR->Verify_Seed_Value^USERDATA_PTR->Seed_Time) * ((uint32_t)USERDATA_PTR->GlobalPTR + i) ) % 2147483648;
            if(randomVal!=VerifyVal){
                printf("Verify discrepancy at memory location %p, expected %d but found %d\n", USERDATA_PTR->GlobalPTR + i, VerifyVal, randomVal);
            }
            else{
                printf("Verified the value %d at location%p\n", VerifyVal, USERDATA_PTR->GlobalPTR + i);
            }
        }
    }
    
   
    
    
}

void Interpret_Verify_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if ( StrCmp(token, "Verify") != 0){
        return;
    }
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    if (token == NULL){
            printf("ERROR: Please specify an address or offset of memory to verify\n");
            return;
        }
    //If the user specified to write an offset of memory 
    if ( (StrCmp(token, "-o") == 0) ){
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a memory address offset and an integer to verify to it\n");
            return;
        }
        
        //Check to see if the user input a character and not a number offset and that the offset is less than the number of bytes the user allocated
        int Offset = atoi(token);
        if(IsChar(token[0]) == 1){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return;
        }
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate || Offset < 0){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Verify_Pattern_offset=Offset;
        }
        token = strtok(NULL, " ");        
        if (token == NULL){
            printf("ERROR:Please specify a seed value to use to generate a number\n");
            return;
        }
        int Seed = atoi(token);
        //Ensure the user typed an integer
        if (Seed == 0){
            printf("ERROR: Please specify a nonzero seed to generate a number\n");
            return;
        }
        USERDATA_PTR->Verify_Seed_Value = Seed;
        if (USERDATA_PTR->Allocate_State == 0){
            printf("ERROR: You can't write to memory you have not allocated\n");
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
            int length = atoi(token);
            if(length == 0){
                printf("ERROR: Please specify a nonzero length to write a pattern\n");
            }
            else{
                USERDATA_PTR->Verify_Length = length;
            }
        }
        Verify(USERDATA_PTR);
        return;
    }
    //If the user provided a hexidecimal address
    else{
        token[strlen(token)] = '\0';
        if (token == NULL){
            printf("ERROR: Please specify an address to verify\n");
            return;
        }
        long int UserAddress = strtol(token, NULL, 16);
        //Check if the address the user provided is within range
        if ( (UserAddress >= (long int)USERDATA_PTR->GlobalPTR) && (UserAddress <= ((long int)USERDATA_PTR->GlobalPTR+USERDATA_PTR->Bytes_To_Allocate)) ){
            long int Offset = (UserAddress-(long int)USERDATA_PTR->GlobalPTR)/4;
            USERDATA_PTR->Verify_Pattern_offset=Offset;            
        }
        else{
            printf("ERROR: Invalid address specified\n");
            return;
        }
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a seed value\n");
            return;
        }
        int Seed = atoi(token);
        //Ensure the user typed an integer
        if (Seed == 0){
            printf("ERROR: Please specify a nonzero seed to generate a number\n");
            return;
        }
        USERDATA_PTR->Verify_Seed_Value = Seed;
        if (USERDATA_PTR->Allocate_State == 0){
            printf("ERROR: You can't write to memory you have not allocated\n");
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
            int length = atoi(token);
            if(length == 0){
                printf("ERROR: Please specify a nonzero length to write a pattern\n");
            }
            else{
                USERDATA_PTR->Verify_Length = length;
            }
        }
        Verify(USERDATA_PTR);
        return;
    }       
}