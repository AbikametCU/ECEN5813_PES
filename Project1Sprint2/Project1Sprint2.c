#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

#define INVALID 0
#define HELP 1
#define ALLOCATE 2
#define FREE 3

struct UserData {
    int Bytes_To_Allocate;
    char * Word_To_Allocate;
};

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
        
    else {
        return INVALID;
    }  
}

void Allocate_Memory(void *MemoryPTR, struct UserData* USERDATA_PTR){
    MemoryPTR = (void*) malloc(USERDATA_PTR->Bytes_To_Allocate);
    if (MemoryPTR==NULL){
        printf("Memory Allocation Failed\n");
    }
    else{
        printf("Memory Allocation Successful\n");
    }
}

void Free_Memory(void *MemoryPTR){
    free(MemoryPTR);
}

int main() {
//Character array to hold the user input    
    while(1){
    //-------------Get input from the user on what action to request from the server.------------           
        int Welcome_Done_State = 0;
        int Input_State = 0;
        
        //Ensures the data is freed if the user calls allocate memory twice in a row, or if the user tries to free unallocated memory
        int Free_State = 0;
        void *MemoryPTR;
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
                       "                                            Display \n"
                       "                                            Write Memory \n"
                       "                                            Invert \n"
                       "                                            Write Pattern\n"
                       "                                            Verify Pattern \n"
                       "                                            exit\n");
            }
            else if(ReturnVal == ALLOCATE){
                if(Free_State==1){
                    Free_Memory(MemoryPTR);
                    printf("Previously Allocated Memory was freed\n");
                }
                Allocate_Memory(MemoryPTR, USERDATA_PTR);
                Free_State = 1;
            }
            else if(ReturnVal == FREE){
                if(Free_State == 0){
                    printf("You can't free memory that hasn't been allocated\n");
                }
                else{
                    Free_Memory(MemoryPTR);
                    printf("Memory Successfully Freed\n");
                    Free_State = 0;
                }
                
            }
        }
    }
}