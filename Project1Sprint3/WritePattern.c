#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdint.h>
#include "Project1.h"

static unsigned long my_rand_state = 1;

void my_srand(unsigned long seed) {
    my_rand_state = seed;
}

long my_rand(struct UserData *USERDATA_PTR) {
   
    //Grab the currnt time for the seed value
    time_t Seed_Time = time(0);
    
    USERDATA_PTR->Seed_Time = (long)Seed_Time;
    printf("Time:%ld",USERDATA_PTR->Seed_Time);
    
    
    my_rand_state = (my_rand_state * USERDATA_PTR->Seed_Time + 12345) % 2147483648;
    return my_rand_state;
}

void Write_Pattern(struct UserData* USERDATA_PTR){
    my_srand(USERDATA_PTR->Seed_Value);
    
    int RandomValue=my_rand(USERDATA_PTR);
    *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Write_Pattern_Offset) = RandomValue;
    
    printf("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+USERDATA_PTR->Write_Pattern_Offset);
    
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
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
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
            printf("ERROR: Please specify a nonzero integer to write to memory\n");
            return;
        }
        else{
            USERDATA_PTR->Seed_Value = Seed;
            if (USERDATA_PTR->Allocate_State == 0){
                printf("ERROR: You can't write to memory you have not allocated\n");
            }
            else{
                Write_Pattern(USERDATA_PTR);
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
            USERDATA_PTR->Write_Pattern_Offset=Offset;            
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
        int Seed = atoi(token);     
        //Ensure the user typed an integer
        if (Seed == 0){
            printf("ERROR: Please specify a nonzero integer to write to memory\n");
            return;
        }
        else{
            USERDATA_PTR->Seed_Value = Seed;
            if (USERDATA_PTR->Allocate_State == 0){
                printf("ERROR: You can't write to memory you have not allocated\n");
            }
            else{
                Write_Pattern(USERDATA_PTR);
                USERDATA_PTR->Write_State = 1;
            }
            return;
        }
    }       
}