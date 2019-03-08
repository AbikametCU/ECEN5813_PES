#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <stdint.h>
#include "AllocateMemory.h"
#include "DisplayMemory.h"
#include "VerifyPattern.h"
#include "WritePattern.h"
#include "InvertMemory.h"
#include "WriteMemory.h"
#include "FreeMemory.h"
#include "Project1.h"

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

void Interpret_User_Input(char* User_Input, struct UserData *USERDATA_PTR){
    if( (StrCmp(User_Input, "Help\n") == 0) ){ 
        printf("Please enter one of the following commands: Allocate Memory [Number of Bytes]\n"
               "                                            Free Memory-Frees all allocated memory\n"
               "                                            Display Memory (optional -o option to specify offset instead of hex address) [Memory Address as an integer offset starting from 0 if -o option was chosen or as a hex value] (optional number of memory words to display)\n"
               "                                            Write Memory (optional -o option to specify offset instead of hex address) [Memory Address as an integer offset starting from 0 if -o option was chosen or as a hex value] [Value to Write]\n"
               "                                            Invert [Address to Invert Memory at]\n"
               "                                            Wpattern(optional -o option to specify offset instead of hex address) [Memory Address as an integer offset starting from 0 if -o option was chosen or as a hex value] [seed value]\n"
               "                                            Verify Pattern \n"
               "                                            Exit\n");
    return; 
    }
    else if( (StrCmp(User_Input, "Exit\n") == 0) ){
        printf("Thank you!\n");
        exit(0);
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
   //Grab the currnt time 
    time_t Seed_Time = time(0);
    
    USERDATA.Seed_Time = (uint32_t)Seed_Time;
    while(1){
    //-------------Get input from the user on what action to request from the server.------------           
        int Welcome_Done_State = 0;
        int Input_State = 0;
//        USERDATA.
        //struct UserData USERDATA;
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
                                                    {.FunctionType = "Free", .FunctionName = &Interpret_Free_Input},
                                                    {.FunctionType = "Wpattern", .FunctionName = &Interpret_WPattern_Input},
                                                    {.FunctionType = "Verify", .FunctionName = &Interpret_Verify_Input},
                                                };
        
        while(Input_State==0){
            USERDATA.Wpattern_Length = 0;
            USERDATA.Verify_Length=0;
            char User_Input[100] = {0};
            char User_Input_CPY[100] = {0};
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