#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <stdint.h>
#include "AllocateMemory.h"
#include "DisplayMemory.h"
#include "InvertMemory.h"
#include "WriteMemory.h"


typedef struct UserData {
    //For the Allocate command
    int Bytes_To_Allocate;
    
    //For the Write Command
    int Bytes_To_Write;
    int Write_Offset_Bytes;
    
    //For the display Command
    int Display_Offset_Bytes;
    int Words_To_Display;
    
    //For the Invert Command
    int Words_To_Invert;
    int Invert_Offset_Bytes;
    
    uint32_t* GlobalPTR;
    
    //State Variables to ensure a user doesn't write/display to memory before allocated
    int Allocate_State;
    int Write_State;
};

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

#define NUMBER_OF_USER_SELECTABLE_COMMANDS 7

#define ON 1
#define OFF 0

struct FunctionLUT{
    char *FunctionType;
    //Function Pointer used to point to the correct function to use based off the user's input
    void (*FunctionName)(char*, struct UserData*);
};

int IsChar(char c){
    if( (c>='a' && c<='z') || (c>='A' && c<='Z'))
        return 1;
    else
        return 0;
}

#define MY_RAND_MAX = 2147483647
static unsigned long my_rand_state = 1;

void my_srand(unsigned long seed) {
    my_rand_state = seed;
}

long my_rand() {
    my_rand_state = (my_rand_state * 1103515245 + 12345) % 2147483648;
    return my_rand_state;
}

static inline void Free_Memory(uint32_t *MemoryPTR){
    free(MemoryPTR);
}

void Interpret_User_Input(char* User_Input, struct UserData *USERDATA_PTR){
    if( (StrCmp(User_Input, "Help\n") == 0) ){ 
        printf("Please enter one of the following commands: Allocate Memory [Number of Bytes]\n"
               "                                            Free Memory-Frees all allocated memory\n"
               "                                            Display Memory (optional -o option to specify offset instead of hex address) [Memory Address as an integer offset starting from 0 if -o option was chosen or as a hex value] (optional number of memory words to display)\n"
               "                                            Write Memory (optional -o option to specify offset instead of hex address) [Memory Address as an integer offset starting from 0 if -o option was chosen or as a hex value] [Value to Write]\n"
               "                                            Invert [Address to Invert Memory at]\n"
               "                                            Write Pattern\n"
               "                                            Verify Pattern \n"
               "                                            Exit\n");
    return; 
    }
    else if( (StrCmp(User_Input, "Exit\n") == 0) ){
        printf("Thank you!\n");
        exit(0);
    }
    else if( (StrCmp(User_Input, "Free Memory\n") == 0) ){
        if(USERDATA_PTR->Allocate_State == 0){
            printf("ERROR: You can't free memory that hasn't been allocated\n");
        }
        Free_Memory(USERDATA_PTR->GlobalPTR);
        printf("Memory Successfully Freed\n");
        USERDATA_PTR->Allocate_State = 0;
        USERDATA_PTR->Write_State = 0;
        return;
    }
}

int Handle_User_Input(char* User_Command, char* User_Input, struct FunctionLUT *FUNCTION_LUT_ARRAY_PTR, struct UserData *USERDATA_PTR){
    for(int i = 0; i < NUMBER_OF_USER_SELECTABLE_COMMANDS; i++){
        if ( StrCmp(User_Command, FUNCTION_LUT_ARRAY_PTR[i].FunctionType) == 0 ){
            FUNCTION_LUT_ARRAY_PTR[i].FunctionName(User_Input, USERDATA_PTR);
            return 1;
        }
    }
    return 0;
}

int main() {
//Character array to hold the user input    
    while(1){
    //-------------Get input from the user on what action to request from the server.------------           
        int Welcome_Done_State = 0;
        int Input_State = 0;

        //Structure to access the data the user input for each command
        struct UserData USERDATA;
        USERDATA.Allocate_State=0;
        USERDATA.Write_State=0;
        //Lookup Table structure
        struct FunctionLUT FUNCTION_LUT_ARRAY[] = { 
                                                    {.FunctionType = "Allocate", .FunctionName = &Interpret_Allocate_Input},
                                                    {.FunctionType = "Write", .FunctionName = &Interpret_Write_Input},
                                                    {.FunctionType = "Invert", .FunctionName = &Interpret_Invert_Input},
                                                    {.FunctionType = "Display", .FunctionName = &Interpret_Display_Input},
                                                    {.FunctionType = "Help\n", .FunctionName = &Interpret_User_Input},
                                                    {.FunctionType = "Exit\n", .FunctionName = &Interpret_User_Input},
                                                    {.FunctionType = "Free", .FunctionName = &Interpret_User_Input},
                                                };
        
        while(Input_State==0){
            char User_Input[100] = {0};
            char User_Input_CPY[100] = {0};
            for(int i = 1; i<6; i++){
                my_srand(i);
                printf("Random Number:%ld\n", my_rand());
            }
            if (Welcome_Done_State == 1){
                printf("\nPlease enter a command or type 'Help' for a list of commands:");
            }
            if(Welcome_Done_State == 0){
                printf("ECEN5813-PES Project 1:Welcome to the Interactive Memory Manipulation Program, Please type a command. (or type help for Info):");
                Welcome_Done_State = 1;
            }
            //grab user input and store into User_Input Char array
            fgets(User_Input, 100, stdin);            
            //Make sure the user input the correct options
            memcpy(User_Input_CPY, User_Input, sizeof(User_Input_CPY));
            char *User_Command_Choice = strtok(User_Input_CPY, " ");
            int ReturnVal = Handle_User_Input(User_Command_Choice, User_Input, FUNCTION_LUT_ARRAY, &USERDATA);
            if (ReturnVal == INVALID){
                printf("INVALID INPUT: Please enter a proper command or type 'help' for a list of commands and usage:\n");
                continue;
            } 
        }
    }
}

//            if (ReturnVal == INVALID){
//                printf("INVALID INPUT: Please enter a proper command or type 'help' for a list of commands and usage:\n");
//                continue;
//            }
// else if(ReturnVal == HELP){
//printf("Please enter one of the following commands: Allocate Memory [Number of Bytes]\n"
//       "                                            Free Memory\n"
//       "                                            Display Memory [Memory Address as an integer starting from 0 or as a hex value] [(optional) Number of words to print]\n"
//       "                                            Write Memory [Memory Address as an integer starting from 0 or as a hex value] [Value to Write]\n"
//       "                                            Invert \n"
//       "                                            Write Pattern\n"
//       "                                            Verify Pattern \n"
//       "                                            exit\n");
//}

//else if(ReturnVal == WRITE){
//    if (USERDATA_PTR->Allocate_State == 0){
//        printf("ERROR: You can't write to memory you have not allocated\n");
//    }
//    else{
//        Write_Memory(USERDATA_PTR);
//        USERDATA_PTR->Write_State = 1;
//    }
//}
//else if(ReturnVal == FREE){
//                if(USERDATA_PTR->Allocate_State == 0){
//                    printf("ERROR: You can't free memory that hasn't been allocated\n");
//                }
//                else{
//                    Free_Memory(MemoryPTR);
//                    printf("Memory Successfully Freed\n");
//                    USERDATA_PTR->Allocate_State = 0;
//                    USERDATA_PTR->Write_State = 0;
//                }   
//            }
//else if(ReturnVal == DISPLAY){
//    if ( (USERDATA_PTR->Allocate_State == 0) || (USERDATA_PTR->Write_State == 0) ){
//        printf("ERROR: You can't display memory you haven't written or allocated");
//    }
//    else{
//        Display_Memory(USERDATA_PTR);
//    }
//}