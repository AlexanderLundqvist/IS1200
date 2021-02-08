/*
 sieves.c
 By Alexander Lundqvist
 Last modified: 2020-02-05
 This file is in the public domain.
*/


#include <stdio.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";
int  list1[30];  // Needs to be int to get the hex value
int  list2[30];  // and printlist also calls for int array
int   count = 0; // Global count defined in memory

void copycodes(char* text, int* list, int* count){
  // Dereference the pointer in text to get the value at
  // the pointer to do the check if we've reached NUL
  // In assembly: beq	$t0,$0,done
  while(*text != 0){
    // Dereference and copy value from text to list
    // In assembly: (lb	$t0,0($a0)) & (sw	$t0,0($a1))
    *list = *text;

    // Incrementing the pointers
    // In assembly: (addi	$a0,$a0,1) & (addi $a1,$a1,4)
    text += 1;
    list += 1;

    // Order of operation for dereferencing and incrementing are the same,
    // therefore we have to put count in parentheses first.
    // In assembly: addi	$t1,$t1,1	# Increase the count
    (*count)++;
  }
}

void work(){
  // jal copycodes($a0, $a1, $a2);
  // Arrays as parameter give a pointer to first element
  // Count needs to be as adress
  copycodes(text1, list1, &count); // Arrays as parameter

  // jal copycodes($a0, $a1, $a2);
  // Arrays as parameter give a pointer to first element
  // Count needs to be as adress
  copycodes(text2, list2, &count);

}

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n",
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));

}

int main(void){
  work();

  printf("\nlist1: ");
  printlist(list1);
  printf("\nlist2: ");
  printlist(list2);
  printf("\nCount = %d\n", count);

  endian_proof((char*) &count);
}
