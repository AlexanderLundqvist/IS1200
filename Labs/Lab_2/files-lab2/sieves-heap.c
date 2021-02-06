/*
 sieves-heap.c
 By Alexander Lundqvist
 Last modified: 2020-02-05
 This file is in the public domain.

 This program implements the sieve of Eratosthenes algorithm
 for finding prime numbers.
 The user inputs a number, and the program will print out all
 prime numbers up to that input.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6

// Yuck, we don't want this. How could it be done in another way?
int column_count = 0;

void print_number(int number){
  printf("%10d ", number);
  column_count++;
  if(column_count%COLUMNS == 0){
    printf("\n");
  }
}

// The function that implements the sieve of Eratosthenes algorithm.
// This function has one parameter that states the max prime number values.
void print_sieves(int input){

    // Initialize the array we will work with
    int array_size = input - 2; // -1 for correct indexing, -1 for skipping 1
    // Allocate memory for the array since it is dynamic
    // malloc allocates memory in the heap
    int *numbers = malloc (sizeof (int) * array_size);

    // Populate the array with 1's (True values)
    for(int i = 0; i < array_size; i++){
      numbers[i] = 1;
    }

    // The algorithm starts here!
    for(int i = 2; i <= (int) sqrt(input); i++){
      if(numbers[i] == 1){
        for(int j = i*2; j <= input; j+=i){
          numbers[j] = 0;
        }
      }
    }

    // Printing out the true values from the array
    for(int i = 0; i <= array_size; i++){
      if(numbers[i] == 1){
        print_number(i + 2);
      }
    }

    // Deallocate the memory for the array
    free(numbers);
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  // Uncomment this segment bellow for using input in terminal
  /* if(argc == 2)
    print_primes(atoi(argv[1]));
  else
    printf("Please state an interger number.\n"); */

  // Comment out the line bellow for using input in terminal
  print_sieves(105);

  return 0;
}
