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
#include <time.h> // To measure execution time

#define COLUMNS 6

// Yuck, we don't want this. How could it be done in another way?
int column_count = 0;

void print_number(int number){
  printf("%10d ", number);
  column_count++;
  if(column_count%COLUMNS == 0){ // Every 6th entry will yield 0
    printf("\n");
  }
}

// The function that implements the sieve of Eratosthenes algorithm.
// This function has one parameter that states the max prime number values.
void print_sieves(int input){

    // Initialize the array we will work with
    int array_size = input - 1; // -1 for correct indexing
    // Allocate memory for the array since it is dynamic
    // malloc allocates memory in the heap
    char *numbers = malloc (sizeof (char) * array_size);

    // Populate the array with 1's (True values)
    for(int entry = 0; entry <= array_size; entry++){
      numbers[entry] = 1;
    }

    // The algorithm starts here!
    // Check the factors up to the square of the input, since that is
    // the last number that can be squared to produce the input.
    for(int i = 2; i <= (int) sqrt(input); i++){
      if(numbers[i] == 1){ // This is to check if we already covered a number
        for(int j = i*i; j <= input; j+=i){ // For every multiple of that number
          numbers[j] = 0; // Mark it as non-prime
        }
      }
    }

    // Printing out the true values from the array
    for(int i = 2; i <= array_size; i++){
      if(numbers[i] == 1){
        print_number(i);
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

  // The code for time measurement is from
  // https://stackoverflow.com/questions/5248915/execution-time-of-c-program
  clock_t begin = clock();
  print_sieves(1000000);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\nExecuted in: %lf seconds\n", time_spent);
  return 0;
}
