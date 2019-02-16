#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
//#include <string.h>

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

int main() {
    while(1){
    //-------------Get input from the user on what action to do.------------
        int Welcome_Done_State = 0;
        //Character array to hold the user input
	int Input_State = 0;
        while(Input_State==0){
            char User_Input[100] = {0};
            if (Welcome_Done_State == 1){
                printf("Please enter a command or type 'help' for a list of commands\n");
            }
            if(Welcome_Done_State == 0){
                printf("ECEN5813-PES Project 1:Welcome to the Interactive Memory Manipulation Program, Please type a command. (or type help for Info):\n");
                Welcome_Done_State = 1;
            }
            //grab user input and store into User_Input Char array
            fgets(User_Input, 100, stdin);
            //Make sure the user input the correct options
            if( (StrStr(User_Input, "help") != NULL) && (StrStr(User_Input, " ") == NULL) ){
                printf("Please enter one of the following commands: Allocate\n"
                       "                                            Free \n"
                       "                                            Display \n"
                       "                                            Write Memory \n"
                       "                                            Invert \n"
                       "                                            Write Pattern\n"
                       "                                            Verify Pattern \n"
                       "                                            exit\n");
            }
            else if( (StrStr(User_Input, "exit") != NULL) && (StrStr(User_Input, " ") == NULL) ){
                printf("Thank you!\n");
                exit(0);
            }
            else {
                printf("INVALID INPUT: Please enter a proper command or type 'help' for a list of commands:\n");
            }

        }
    }
}
