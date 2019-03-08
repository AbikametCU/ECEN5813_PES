#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdint.h>
#include "Project1.h"


uint32_t my_rand(struct UserData *USERDATA_PTR, int i) {
    
    uint32_t RandomVal = ( (USERDATA_PTR->Write_Seed_Value ^USERDATA_PTR->Seed_Time) * ((uint32_t)USERDATA_PTR->GlobalPTR + i) ) % 2147483648;
    return RandomVal; 
}

void Write_Pattern(struct UserData* USERDATA_PTR){
    
    if(USERDATA_PTR->Wpattern_Length == 0){
        uint32_t RandomValue=my_rand(USERDATA_PTR, USERDATA_PTR->Write_Pattern_Offset);
        *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Write_Pattern_Offset) = RandomValue;
    
        printf("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+USERDATA_PTR->Write_Pattern_Offset);
    }
    else{
        printf("made it\n");
        for (int i = USERDATA_PTR->Write_Pattern_Offset; i<(USERDATA_PTR->Wpattern_Length + USERDATA_PTR->Write_Pattern_Offset); i++){
            uint32_t RandomValue=my_rand(USERDATA_PTR, i);
            *(USERDATA_PTR->GlobalPTR + i) = RandomValue;

            printf("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+i);
        }   
    }
    
    
}

void Interpret_WPattern_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if ( StrCmp(token, "Wpattern") != 0){
        return;
    }
    token = strtok(NULL, " ");
    if (token == NULL){
            printf("ERROR:Please specify an address or offset of memory to write to\n");
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
            USERDATA_PTR->Write_Pattern_Offset=Offset;
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
        USERDATA_PTR->Write_Seed_Value = Seed;
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
                USERDATA_PTR->Wpattern_Length = length;
            }
        }
        Write_Pattern(USERDATA_PTR);
        USERDATA_PTR->Write_State = 1;
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
            USERDATA_PTR->Write_Pattern_Offset=Offset;            
        }
        else{
            printf("ERROR: Invalid address specified\n");
            return;
        }
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR:Please specify a seed to generate a number\n");
            return;
        }
         int Seed = atoi(token);
        //Ensure the user typed an integer
        if (Seed == 0){
            printf("ERROR: Please specify a nonzero seed to generate a number\n");
            return;
        }
        USERDATA_PTR->Write_Seed_Value = Seed;
        if (USERDATA_PTR->Allocate_State == 0){
            printf("ERROR: You can't write to memory you have not allocated\n");
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
            printf("length:%s", token);
            int length = atoi(token);
            if(length == 0){
                printf("ERROR: Please specify a nonzero length to write a pattern\n");
            }
            else{
                USERDATA_PTR->Wpattern_Length = length;
            }
        }
        Write_Pattern(USERDATA_PTR);
        USERDATA_PTR->Write_State = 1;
        return;
    }       
}