#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"

int main() {

   instrList globalInstr;

   instrListInit(&globalInstr);

   printf("~~~ Printing empty list ~~~\n\n");

   instrListPrintAll(&globalInstr);

   printf("\n\n~~~ Adding test1 ~~~\n\n");

   instrListAppend(&globalInstr, "test1", NULL, NULL, NULL);

   instrListPrintAll(&globalInstr);

   printf("\n\n~~~ Adding test2.. test 10 ~~~\n\n");

   instrListAppend(&globalInstr, "test2", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test3", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test4", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test5", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test6", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test7", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test8", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test9", NULL, NULL, NULL);
   instrListAppend(&globalInstr, "test10", NULL, NULL, NULL);

   instrListPrintAll(&globalInstr);

   
   printf("\n\n~~~ Creating new instr and adding test11.. test15 ~~~\n\n");

   instrList funInstr;

   instrListInit(&funInstr);

   instrListAppend(&funInstr, "test11", NULL, NULL, NULL);
   instrListAppend(&funInstr, "test12", NULL, NULL, NULL);
   instrListAppend(&funInstr, "test13", NULL, NULL, NULL);
   instrListAppend(&funInstr, "test14", NULL, NULL, NULL);
   instrListAppend(&funInstr, "test15", NULL, NULL, NULL);

   instrListPrintAll(&funInstr);


   printf("\n\n~~~ Appending second one to first ~~~\n\n");

   instrListAppendFunction(&globalInstr, &funInstr);

   instrListPrintAll(&globalInstr);

   instrListClear(&funInstr);

   instrListClear(&globalInstr);

   printf("\n\n~~~ Generating label test ~~~\n\n");

   instrListInit(&globalInstr);
   instrListAppendInstrLabel(&globalInstr,"LABEL","while", 1);
   instrListAppendInstrLabel(&globalInstr,"JUMP", "while", 2);
   instrListAppendInstrLabel(&globalInstr,"JUMPIFNEQS", "while", 3);
   instrListAppendInstrLabel(&globalInstr,"JUMPIFNEQS", "foo", -1);
   instrListPrintAll(&globalInstr);

   return 0;
}
