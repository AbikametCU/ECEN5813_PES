#ifndef WRITEPATTERN_H_   /* Include guard */
#define WRITEPATTERN_H_
#include "Project1.h"

void Write_Pattern(struct UserData* USERDATA_PTR);

void Interpret_WPattern_Input(char* User_Input, struct UserData *USERDATA_PTR);

//void my_srand(unsigned long seed);

uint32_t my_rand(struct UserData *USERDATA_PTR);

#endif