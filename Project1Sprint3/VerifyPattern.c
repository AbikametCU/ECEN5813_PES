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
	#if LINUX_COMPILATION
	printf("Verify discrepancy at memory location %p, expected %d but found %d\n", USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset, VerifyVal, randomVal);	
	#else
	PRINTF("Verify discrepancy at memory location %p, expected %d but found %d\n", USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset, VerifyVal, randomVal);
	#endif
            
        }
        else{
	#if LINUX_COMPILATION
	printf("Verified the value %d at location%p\n", VerifyVal, USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset);
	#else
	PRINTF("Verified the value %d at location%p\n", VerifyVal, USERDATA_PTR->GlobalPTR + USERDATA_PTR->Verify_Pattern_offset);
	#endif 
        }
    }
    else{
        for (int i = USERDATA_PTR->Verify_Pattern_offset; i<(USERDATA_PTR->Verify_Pattern_offset+USERDATA_PTR->Verify_Length); i++){
            uint32_t randomVal = *(USERDATA_PTR->GlobalPTR + i);

            uint32_t VerifyVal = ((USERDATA_PTR->Verify_Seed_Value^USERDATA_PTR->Seed_Time) * ((uint32_t)USERDATA_PTR->GlobalPTR + i) ) % 2147483648;
            if(randomVal!=VerifyVal){
		#if LINUX_COMPILATION
		printf("Verify discrepancy at memory location %p, expected %d but found %d\n", USERDATA_PTR->GlobalPTR + i, VerifyVal, randomVal);
		#else
		PRINTF("Verify discrepancy at memory location %p, expected %d but found %d\n", USERDATA_PTR->GlobalPTR + i, VerifyVal, randomVal);
		#endif
            }
            else{
		#if LINUX_COMPILATION
		printf("Verified the value %d at location%p\n", VerifyVal, USERDATA_PTR->GlobalPTR + i);
		#else
		PRINTF("Verified the value %d at location%p\n", VerifyVal, USERDATA_PTR->GlobalPTR + i);
		#endif
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
	#if LINUX_COMPILATION
	printf("ERROR: Please specify an address or offset of memory to verify\n");
	#else
	PRINTF("ERROR: Please specify an address or offset of memory to verify\n");
	#endif
            return;
        }
    //If the user specified to write an offset of memory 
    if ( (StrCmp(token, "-o") == 0) ){
        token = strtok(NULL, " ");
        if (token == NULL){
	#if LINUX_COMPILATION
	    printf("ERROR: Please specify a memory address offset and an integer to verify to it\n");
	#else
	    PRINTF("ERROR: Please specify a memory address offset and an integer to verify to it\n");
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
		printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
	#else
		PRINTF("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
	#endif
            return;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Verify_Pattern_offset=Offset;
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
        USERDATA_PTR->Verify_Seed_Value = Seed;
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
	    #if LINUX_COMPILATION
		printf("ERROR: Please specify an address to verify\n");
	    #else
		PRINTF("ERROR: Please specify an address to verify\n");
	    #endif
            return;
        }
        long int UserAddress = strtol(token, NULL, 16);
        //Check if the address the user provided is within range
        if ( (UserAddress >= (long int)USERDATA_PTR->GlobalPTR) && (UserAddress <= ((long int)USERDATA_PTR->GlobalPTR+USERDATA_PTR->Bytes_To_Allocate)) ){
            long int Offset = (UserAddress-(long int)USERDATA_PTR->GlobalPTR)/4;
            USERDATA_PTR->Verify_Pattern_offset=Offset;            
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
		printf("ERROR: Please specify a seed value\n");
	    #else
		PRINTF("ERROR: Please specify a seed value\n");
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
        USERDATA_PTR->Verify_Seed_Value = Seed;
        if (USERDATA_PTR->Allocate_State == 0){
	    #if LINUX_COMPILATION
		printf("ERROR: You can't write to memory you have not allocated\n");
	    #else
		printf("ERROR: You can't write to memory you have not allocated\n");
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
                USERDATA_PTR->Verify_Length = length;
            }
        }
        Verify(USERDATA_PTR);
        return;
    }       
}
