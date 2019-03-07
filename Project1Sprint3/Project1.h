#ifndef PROJECT1_H_   /* Include guard */
#define PROJECT1_H_


//Structure to access the data the user input for each command
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
    
    //For the write Patter command
    int Write_Pattern_Offset;
    int Seed_Value;
    long Seed_Time;
    uint32_t* GlobalPTR;
    
    //State Variables to ensure a user doesn't write/display to memory before allocated
    int Allocate_State;
    int Write_State;
}UserData;

UserData USERDATA;

#define MEMORY_WORD 4

#define INVALID 0

#define NUMBER_OF_USER_SELECTABLE_COMMANDS 8

#define ON 1
#define OFF 0

#define MY_RAND_MAX = 2147483647

char* StrStr(char *str, char *substr);

int StrCmp(char *strg1, char *strg2);

int IsChar(char c);

#endif