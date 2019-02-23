#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <stdint.h>

int StrCmp(char *strg1, char *strg2)
{
    while( ( *strg1 != '\0' && *strg2 != '\0' ) && *strg1 == *strg2 )
    {
        strg1++;
        strg2++;
    }
 
    if(*strg1 == *strg2)
    {
        return 0; // strings are identical
    }
 
    else
    {
        return *strg1 - *strg2;
    }
}

char* StrStr(char *str, char *substr)
{
	  while (*str != '\0') 
	  {
		    char *Begin = str;
		    char *pattern = substr;
		    
		    // If first character of sub string match, check for whole string
		    while ( (*str != '\0') && (*pattern != '\0') && (*str == *pattern) ) 
                    {
			      str++;
			      pattern++;
		    }
		    // If complete sub string match, return starting address 
		    if (*pattern == '\0'){
                        return Begin;
                    }
		    	  
		    str = Begin + 1;	// Increament main string 
	  }
	  return NULL;
}
#define MEMORY_WORD 4

#define INVALID 0
#define HELP 1
#define ALLOCATE 2
#define FREE 3
#define WRITE 4
#define DISPLAY 5

#define ON 1
#define OFF 0

struct UserData {
    //For the Allocate command
    int Bytes_To_Allocate;
    
    //For the Write Command
    int Bytes_To_Write;
    int Write_Offset_Bytes;
    
    //For the display Command
    int Display_Offset_Bytes;
    
    int Invert_Bytes;
    
    uint32_t* GlobalPTR;
    
};

int IsChar(char c){
    if( (c>='a' && c<='z') || (c>='A' && c<='Z'))
        return 1;
    else
        return 0;
}

int Interpret_Display_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if ( StrCmp(token, "Display") != 0){
        return INVALID;
    }
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    //If the user specified to write an offset of memory 
    if ( (StrCmp(token, "-o") == 0) ){
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a memory address offset and an integer to write to it\n");
            return INVALID;
        }
        
        //Check to see if the user input a character and that the offset is less than the number of bytes the user allocated
        int Offset = atoi(token);
        if(IsChar(token[0]) == 1){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return INVALID;
        }
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return INVALID;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Display_Offset_Bytes=Offset;
            return DISPLAY;
        }
    }
    //If the user provided a hexidecimal address
    else{
        token[strlen(token)] = '\0';
        
        uint32_t UserAddress = (uint32_t)strtol(token, NULL, 16);
        //Check if the address the user provided is within range
        if ( ((uint32_t*)UserAddress >= USERDATA_PTR->GlobalPTR) && ((uint32_t*)UserAddress <= (USERDATA_PTR->GlobalPTR+USERDATA_PTR->Bytes_To_Allocate)) ){
            uint32_t Offset = (UserAddress-(uint32_t)USERDATA_PTR->GlobalPTR)/4;
            USERDATA_PTR->Display_Offset_Bytes=Offset;
            return DISPLAY;
        }
        else{
            printf("ERROR: Invalid address specified\n");
            return INVALID;
        }    
    }       
}

int Interpret_Write_Input(char* User_Input, struct UserData *USERDATA_PTR){
    char *token = strtok(User_Input, " "); 
    if ( StrCmp(token, "Write") != 0){
        return INVALID;
    }
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    //If the user specified to write an offset of memory 
    if ( (StrCmp(token, "-o") == 0) ){
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a memory address offset and an integer to write to it\n");
            return INVALID;
        }
        
        //Check to see if the user input a character and not a number offset and that the offset is less than the number of bytes the user allocated
        int Offset = atoi(token);
        if(IsChar(token[0]) == 1){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return INVALID;
        }
        else if(Offset > USERDATA_PTR->Bytes_To_Allocate){
            printf("ERROR: Please specify a proper memory address via an integer offset with 0 being the starting location up to %d\n", USERDATA_PTR->Bytes_To_Allocate);
            return INVALID;
        }
        else{
            //Store the offset the user specified
            USERDATA_PTR->Write_Offset_Bytes=Offset;
        }
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR:Please specify a number to write to memory\n");
            return INVALID;
        }
        int Bytes_To_Write = atoi(token);
        //Ensure the user typed an integer
        if (Bytes_To_Write == 0){
            printf("ERROR: Please specify a nonzero integer to write to memory\n");
            return INVALID;
        }
        else{
            USERDATA_PTR->Bytes_To_Write = Bytes_To_Write;
            return WRITE;
        }
    }
    //If the user provided a hexidecimal address
    else{
        token[strlen(token)] = '\0';
        
        uint32_t UserAddress = (uint32_t)strtol(token, NULL, 16);
        printf("UserAddress:%x\n", UserAddress);
        
        //Check if the address the user provided is within range, if it is find the offset
        if ( ((uint32_t*)UserAddress >= USERDATA_PTR->GlobalPTR) && ((uint32_t*)UserAddress <= (USERDATA_PTR->GlobalPTR+USERDATA_PTR->Bytes_To_Allocate)) ){
            uint32_t Offset = (UserAddress-(uint32_t)USERDATA_PTR->GlobalPTR)/4;
            USERDATA_PTR->Write_Offset_Bytes = Offset;
        }
        else{
            printf("ERROR: Invalid address specified\n");
            return INVALID;
        }
        token = strtok(NULL, " ");
        int Bytes_To_Write = atoi(token);
        
        //Ensure the user typed an integer
        if (Bytes_To_Write == 0){
            printf("ERROR: Please specify a nonzero integer to write to memory\n");
            return INVALID;
        }
        else{
            USERDATA_PTR->Bytes_To_Write = Bytes_To_Write;
            return WRITE;
        }
    }       
}


int Interpret_User_Input(char* User_Input, struct UserData *USERDATA_PTR){
    if( (StrCmp(User_Input, "help\n") == 0) ){ 
        return HELP;
    }
    else if( (StrCmp(User_Input, "exit\n") == 0) ){
        printf("Thank you!\n");
        exit(0);
    }
    else if( (StrStr(User_Input, "Allocate Memory") != NULL) ){
        char *token = strtok(User_Input, " "); 
        if ( StrCmp(token, "Allocate") != 0){
            return INVALID;
        }
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");
        if (token == NULL){
            printf("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
            return INVALID;
        }
        int Bytes_To_Allocate = atoi(token);
        //Ensure the user typed an integer
        if (Bytes_To_Allocate == 0){
            printf("ERROR: Please specify a nonzero number of bytes of memory to allocate\n");
            return INVALID;
        }
        else{
            USERDATA_PTR->Bytes_To_Allocate = Bytes_To_Allocate;
            return ALLOCATE;
        }
    }
    else if( (StrCmp(User_Input, "Free Memory\n") == 0) ){
        return FREE;
    }
        
    else if( StrStr(User_Input, "Write Memory") != NULL){
        int WriteReturnVal = Interpret_Write_Input(User_Input, USERDATA_PTR);
        if (WriteReturnVal == INVALID){
            return INVALID;
        }
        else if(WriteReturnVal == WRITE){
            return WRITE;
        }
    }
    else if( StrStr(User_Input, "Display Memory") != NULL){
        int DisplayReturnVal = Interpret_Display_Input(User_Input, USERDATA_PTR);
        if (DisplayReturnVal == INVALID){
            return INVALID;
        }
        else if(DisplayReturnVal == DISPLAY){
            return DISPLAY;
        }
    }
    else {
        return INVALID;
    }  
}

static inline void Allocate_Memory(uint32_t **MemoryPTR, struct UserData* USERDATA_PTR){
    *MemoryPTR = (uint32_t*) malloc( (USERDATA_PTR->Bytes_To_Allocate) * MEMORY_WORD);
    if (*MemoryPTR==NULL){
        printf("Memory Allocation Failed\n");
    }
    else{
        printf("\n%d 32-bit chunks of memory allocated successfully starting at location%p\n", USERDATA_PTR->Bytes_To_Allocate, (void*)*MemoryPTR);
    }
}

static inline void Free_Memory(uint32_t *MemoryPTR){
    free(MemoryPTR);
}

static inline void Display_Memory(uint32_t **MemoryPTR, struct UserData* USERDATA_PTR){  
    printf("Data Located at Address:%p = %d\n", *MemoryPTR+USERDATA_PTR->Write_Offset_Bytes, *(*MemoryPTR + USERDATA_PTR->Write_Offset_Bytes));
}

static inline void Write_Memory(uint32_t **MemoryPTR, struct UserData* USERDATA_PTR){    
    //Shift the address by the offset and dereference it to assign the value
    *(*MemoryPTR + USERDATA_PTR->Write_Offset_Bytes) = USERDATA_PTR->Bytes_To_Write;
    printf("Wrote %d at Memory Location:%p\n", *(*MemoryPTR + USERDATA_PTR->Write_Offset_Bytes), *MemoryPTR+USERDATA_PTR->Write_Offset_Bytes);
    //printf("MemoryPTR:%p, WritePTRaddr:%p, Data:%d\n", *MemoryPTR, *MemoryPTR+USERDATA_PTR->Write_Offset_Bytes, *(*MemoryPTR + USERDATA_PTR->Write_Offset_Bytes));
}

int main() {
//Character array to hold the user input    
    while(1){
    //-------------Get input from the user on what action to request from the server.------------           
        int Welcome_Done_State = 0;
        int Input_State = 0;
        
        //Ensures the data is freed if the user calls allocate memory twice in a row, or if the user tries to free unallocated memory
        int Allocate_State = 0;
        int Write_State = 0;
        uint32_t *MemoryPTR;
        while(Input_State==0){
            //Structure to access the data the user input for each command
            struct UserData USERDATA;
            struct UserData *USERDATA_PTR = &USERDATA;
            
            char User_Input[100] = {0};
            if (Welcome_Done_State == 1){
                printf("\nPlease enter a command or type 'help' for a list of commands:");
            }
            if(Welcome_Done_State == 0){
                printf("ECEN5813-PES Project 1:Welcome to the Interactive Memory Manipulation Program, Please type a command. (or type help for Info):");
                Welcome_Done_State = 1;
            }
            //grab user input and store into User_Input Char array
            fgets(User_Input, 100, stdin);            
            
            //Make sure the user input the correct options
            int ReturnVal=Interpret_User_Input(User_Input, USERDATA_PTR);
            if (ReturnVal == INVALID){
                printf("INVALID INPUT: Please enter a proper command or type 'help' for a list of commands and usage:\n");
                continue;
            }
            else if(ReturnVal == HELP){
                printf("Please enter one of the following commands: Allocate Memory [Number of Bytes]\n"
                       "                                            Free Memory\n"
                       "                                            Display Memory [Memory Address as an integer starting from 0 or as a hex value]\n"
                       "                                            Write Memory [Memory Address as an integer starting from 0 or as a hex value] [Value to Write]\n"
                       "                                            Invert \n"
                       "                                            Write Pattern\n"
                       "                                            Verify Pattern \n"
                       "                                            exit\n");
            }
            else if(ReturnVal == ALLOCATE){
                if(Allocate_State==1){
                    Free_Memory(MemoryPTR);
                    printf("Previously Allocated Memory was freed\n");
                }
                Allocate_Memory(&MemoryPTR, USERDATA_PTR);
                Allocate_State = 1;
                printf("MemoryPointer Address Int:%p\n", (void*)MemoryPTR);
                USERDATA_PTR->GlobalPTR=MemoryPTR;
            }
            else if(ReturnVal == FREE){
                if(Allocate_State == 0){
                    printf("ERROR: You can't free memory that hasn't been allocated\n");
                }
                else{
                    Free_Memory(MemoryPTR);
                    printf("Memory Successfully Freed\n");
                    Allocate_State = 0;
                    Write_State = 0;
                }   
            }
            else if(ReturnVal == WRITE){
                if (Allocate_State == 0){
                    printf("ERROR: You can't write to memory you have not allocated\n");
                }
                else{
                    Write_Memory(&MemoryPTR, USERDATA_PTR);
                    Write_State = 1;
                }
            }
            else if(ReturnVal == DISPLAY){
               if ( (Allocate_State == 0) || (Write_State == 0) ){
                   printf("ERROR: You can't display memory you haven't written or allocated");
               }
               else{
                   Display_Memory(&MemoryPTR, USERDATA_PTR);
               }
            }
        }
    }
}

