#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdint.h>
#include "Project1.h"


	#if LINUX_COMPILATION
	#else
	#endif

uint32_t my_rand(struct UserData *USERDATA_PTR, int i) {
    
    uint32_t RandomVal = ( (USERDATA_PTR->Write_Seed_Value ^USERDATA_PTR->Seed_Time) * ((uint32_t)USERDATA_PTR->GlobalPTR + i) ) % 2147483648;
    return RandomVal; 
}

void Write_Pattern(struct UserData* USERDATA_PTR){
    
    if(USERDATA_PTR->Wpattern_Length == 0){
        uint32_t RandomValue=my_rand(USERDATA_PTR, USERDATA_PTR->Write_Pattern_Offset);
        *(USERDATA_PTR->GlobalPTR + USERDATA_PTR->Write_Pattern_Offset) = RandomValue;
	#if LINUX_COMPILATION
	printf("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+USERDATA_PTR->Write_Pattern_Offset);
	#else
	PRINTF("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+USERDATA_PTR->Write_Pattern_Offset);
	#endif    
        
    }
    else{
	#if LINUX_COMPILATION
	printf("made it\n");
	#else
	PRINTF("made it\n");
	#endif
        for (int i = USERDATA_PTR->Write_Pattern_Offset; i<(USERDATA_PTR->Wpattern_Length + USERDATA_PTR->Write_Pattern_Offset); i++){
            uint32_t RandomValue=my_rand(USERDATA_PTR, i);
            *(USERDATA_PTR->GlobalPTR + i) = RandomValue;
	#if LINUX_COMPILATION
	printf("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+i);
	#else
	PRINTF("Wrote %d at Memory Location:%p\n", RandomValue, USERDATA_PTR->GlobalPTR+i);
	#endif
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
	#if LINUX_COMPILATION
	printf("ERROR:Please specify an address or offset of memory to write to\n");
	#else
	PRINTF("ERROR:Please specify an address or offset of memory to write to\n");
	#endif
            return;
        }
    //If the user specified to write an offset of memory 
    if ( (StrCmp(token, "-o") == 0) ){
        token = strtok(NULL, " ");
        if (token == NULL){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a memory address offset and an integer to write to it\n");
	#else
	PRINTF("ERROR: Please specify a memory address offset and an integer to write to it\n");
	#endif
            return;
        }
        
        //Check to see if the user input a character and not a number offset and that the offset is less than the number of bytes the user allocated
        int Offset = atoi(token);
        if(IsChar(token[0]) == 1){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
	#else
	PRINTF("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
	#endif
            return;
        }
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate || Offset < 0){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a proper non-negative memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
	#else
	PRINTF("ERROR: Please specify a proper non-negative memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
	#endif
            return;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Write_Pattern_Offset=Offset;
        }
        token = strtok(NULL, " ");        
        if (token == NULL){
	#if LINUX_COMPILATION
	printf("ERROR:Please specify a seed value to use to generate a number\n");
	#else
	PRINTF("ERROR:Please specify a seed value to use to generate a number\n");
	#endif
            return;
        }
        int Seed = atoi(token);
        //Ensure the user typed an integer
        if (Seed == 0){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a nonzero seed to generate a number\n");
	#else
	PRINTF("ERROR: Please specify a nonzero seed to generate a number\n");
	#endif
            return;
        }
        USERDATA_PTR->Write_Seed_Value = Seed;
        if (USERDATA_PTR->Allocate_State == 0){
	#if LINUX_COMPILATION
	printf("ERROR: You can't write to memory you have not allocated\n");
	#else
	PRINTF("ERROR: You can't write to memory you have not allocated\n");
	#endif
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
            int length = atoi(token);
            if(length == 0){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a nonzero length to write a pattern\n");
	#else
	PRINTF("ERROR: Please specify a nonzero length to write a pattern\n");
	#endif
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
	#if LINUX_COMPILATION
	printf("ERROR:Please specify an address to write to\n");
	#else
	PRINTF("ERROR:Please specify an address to write to\n");
	#endif
            return;
        }
        long int UserAddress = strtol(token, NULL, 16);
        //Check if the address the user provided is within range
        if ( (UserAddress >= (long int)USERDATA_PTR->GlobalPTR) && (UserAddress <= ((long int)USERDATA_PTR->GlobalPTR+USERDATA_PTR->Bytes_To_Allocate)) ){
            long int Offset = (UserAddress-(long int)USERDATA_PTR->GlobalPTR)/4;
            USERDATA_PTR->Write_Pattern_Offset=Offset;            
        }
        else{
	#if LINUX_COMPILATION
	printf("ERROR: Invalid address specified\n");
	#else
	PRINTF("ERROR: Invalid address specified\n");
	#endif

            return;
        }
        token = strtok(NULL, " ");
        if (token == NULL){
	#if LINUX_COMPILATION
	printf("ERROR:Please specify a seed to generate a number\n");
	#else
	PRINTF("ERROR:Please specify a seed to generate a number\n");
	#endif
            return;
        }
         int Seed = atoi(token);
        //Ensure the user typed an integer
        if (Seed == 0){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a nonzero seed to generate a number\n");
	#else
	PRINTF("ERROR: Please specify a nonzero seed to generate a number\n");
	#endif
            return;
        }
        USERDATA_PTR->Write_Seed_Value = Seed;
        if (USERDATA_PTR->Allocate_State == 0){
	#if LINUX_COMPILATION
	printf("ERROR: You can't write to memory you have not allocated\n");
	#else
	PRINTF("ERROR: You can't write to memory you have not allocated\n");
	#endif
            
        }
        token = strtok(NULL, " ");
        //If the user specified a length of addresses to write
        if (token != NULL){
	#if LINUX_COMPILATION
	printf("length:%s", token);
	#else
	PRINTF("length:%s", token);
	#endif
            int length = atoi(token);
            if(length == 0){
	#if LINUX_COMPILATION
	printf("ERROR: Please specify a nonzero length to write a pattern\n");
	#else
	PRINTF("ERROR: Please specify a nonzero length to write a pattern\n");
	#endif
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
