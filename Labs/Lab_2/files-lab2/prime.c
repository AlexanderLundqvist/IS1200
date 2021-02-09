/*
 prime.c
 By David Broman.
 Last modified: 2020-02-05 (Alexander Lundqvist)
 This file is in the public domain.

*/


#include <stdio.h>

int is_prime(int n){
  int c; // Counter

  for(c=2; c<=n-1; c++){ // Se ifall parametern går att dela jämt på något
      if(n%c==0){        // heltal c < n. Om det stämmer return 0.
        return 0;
      }
  }
  if (c==n){
    return 1;
  }
}

// Add more statements if you want to check.
int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(4));   // 4 is not a prime.   Should print 0.
  printf("%d\n", is_prime(17));  // 17 is a prime.      Should print 1.
  printf("%d\n", is_prime(987)); // 800 is not a prime. Should print 0.
  printf("%d\n", is_prime(5));   // 5 is a prime.       Should print 1.
  printf("%d\n", is_prime(9));   // 9 is not a prime.   Should print 0.

}
