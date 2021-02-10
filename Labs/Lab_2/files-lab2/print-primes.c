/*
 print-prime.c
 By David Broman.
 Last modified: 2020-02-05 (Alexander Lundqvist)
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h> // To measure execution time

#define COLUMNS 6


int is_prime(int input){
  // Used to incrementaly divide the input until the remainder is 0
  int divider;

  // Loop to check if the input can be divided by any number other than
  // 1 or the input itself
  for(divider=2; divider <= input - 1; divider++){
      if(input%divider == 0){
        return 0;
      }
  }

  // If the loop didn't find any factor other than 1
  if (divider == input){
    return 1;
  }
}

// Yuck, we don't want this. How could it be done in another way?
int column_count = 0;

void print_number(int number){
  printf("%10d ", number);
  column_count++;
  if(column_count%COLUMNS == 0){
    printf("\n");
  }

}

void print_primes(int input){
  // Should print out all prime numbers less than 'n'
  // with the following formatting. Note that
  // the number of columns is stated in the define
  // COLUMNS

  int candidate;
  for(candidate = 2; candidate < input; candidate++){
    if(is_prime(candidate) == 1){
        print_number(candidate);
    }
  }
}



// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  /* if(argc == 2)
    print_primes(atoi(argv[1]));
  else
    printf("Please state an interger number.\n"); */

  // The code for time measurement is from
  // https://stackoverflow.com/questions/5248915/execution-time-of-c-program
  clock_t begin = clock();
  print_primes(100000);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\nExecuted in: %lf seconds\n", time_spent);
  return 0;
}
