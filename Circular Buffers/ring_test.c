/*========================================================================
** ring_test.c
** Circular buffer testing
** ECEN5813
**========================================================================*/
#include <CUnit/CUnit.h>	//ASSERT macros for use in test cases, and includes other framework headers.
#include <CUnit/CUError.h>	//Error handing functions and data types. Included automatically by CUnit.h.
#include <CUnit/TestDB.h>	//Data type definitions and manipulation functions for the test registry, suites, and tests. Included automatically by CUnit.h.
#include <CUnit/TestRun.h>	//Data type definitions and functions for running tests and retrieving results. Included automatically by CUnit.h.
#include <CUnit/Automated.h>	//Automated interface with xml output.
#include <CUnit/Basic.h>	//Basic interface with non-interactive output to stdout.
#include <CUnit/Console.h>	//Interactive console interface.
//#include <CUnit/CUCurses.h>	//Interactive console interface (*nix).
//#include <CUnit/Win.h>	Windows interface (not yet implemented).
#include <time.h>

#include "ring.h"

ring_t *ring_buffer;
int test_init(void)
{ 
  	ring_buffer = Ring_init(30);
	return 0;
}

void test_insert(void){	
    for (int i = 0; i<29; i++){   
    	CU_ASSERT(insert(ring_buffer, 'a'+i)==0);
	printf("%d\n", i);
    }	
    CU_ASSERT(entries(ring_buffer)==29);
    char data;
    CU_ASSERT(remove_element(ring_buffer,&data)==0);
    CU_ASSERT(insert(ring_buffer, 'a')==0);
    printf("data removed:%c\n", data);

}

void test_remove(void){
    char data;
    CU_ASSERT(insert(ring_buffer, 'a')==0);
    CU_ASSERT(remove_element(ring_buffer,&data)==0);
    CU_ASSERT(entries(ring_buffer)==0);
    printf("data:%c\n", data);
}

void test_fullBuffer(void){
   //ring_t *ring_buffer2;
   ring_buffer = Ring_init(30);
   int i=0;
   for (i=0;i<ring_buffer->Length;i++){
		CU_ASSERT(insert(ring_buffer, i+97)==0);
	}//Buffer should be full now
	 CU_ASSERT(-1 == insert(ring_buffer, 'a'));	
}

void test_emptyBuffer(void){
   //ring_t *ring_buffer4;
   ring_buffer = Ring_init(30);
   char data;
   CU_ASSERT(remove_element(ring_buffer,&data)==-1);
   
}


void randomized_testing(void){
	//ring_t *ring_buffer3;
        ring_buffer = Ring_init(400);
	srand(time(0));
	int no_of_random_operations=100;
	int current_no_of_entries=0;
	int i,rand_value;
	char rand_char_ascii;
	for(i=0;i<no_of_random_operations;i++){
		printf("Current No of Entries:%d\n",current_no_of_entries);
		rand_value=rand()%4;
		rand_char_ascii=97+(rand()%26);//chooses random lowercase letter
		switch(rand_value){
			case 1 ... 2:{//Insert
				printf("Insert\n");
				if(current_no_of_entries<ring_buffer->Length){			
					CU_ASSERT(insert(ring_buffer, rand_char_ascii)==0);
					current_no_of_entries++;
				}
				else{
					CU_ASSERT(insert(ring_buffer, &rand_char_ascii)==-1);				
				} 
			}break;
			
			case 3:{//Remove
				printf("Remove\n");
				if(current_no_of_entries>0){
					CU_ASSERT(insert(ring_buffer, rand_char_ascii)==0);
					current_no_of_entries--;
				}
				else{
					
					CU_ASSERT(remove_element(ring_buffer,&rand_char_ascii)==-1);
									
				}

			}break;
		
			case 4:{//entries
				printf("Entries\n");
				CU_ASSERT(entries(ring_buffer)==current_no_of_entries);
			}break;
		}
	}
}


int clean_suite(void){
return 0;
}

int main(void){
    //test_init(); 
    CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry()){
    	 return CU_get_error();
   }
   pSuite = CU_add_suite("Suite_1", test_init, clean_suite);
   if (NULL == pSuite){
	   CU_cleanup_registry();
	   return CU_get_error();
   }
     /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test of remove()", test_remove)) || (NULL == CU_add_test(pSuite, "test of insert()", test_insert)) || (NULL == CU_add_test(pSuite, "Testing full buffer Insert", test_fullBuffer)) || (NULL == CU_add_test(pSuite, "Testing empty  buffer remove",test_emptyBuffer)) || (NULL == CU_add_test(pSuite, "Randomized testing",randomized_testing)))
   {

      CU_cleanup_registry();
      return CU_get_error();
   }
    printf("test2\n");
      /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
