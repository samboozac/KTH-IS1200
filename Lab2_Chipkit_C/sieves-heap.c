/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COLUMNS 6
int counter = 0;


int is_prime(int n){
  int i = 0;

  if (n < 3) {
    return 1;
  }

  for(i = 2; i < n; i++) {
    if(n%i==0) {
      return 0;
    }
  }
  return 1;
}

void print_number(int n) {
  if(counter%6==0) {
    printf("\n");
  }
  printf("%d\t", n);
  counter++;
}


void print_sieves(int n) {
    bool *data = malloc (n);

    for(int i = 2; i < n; i++) {
      *(data+i) = 1;
    }

    for(int i = 2; i < sqrt(n); i++) {
      for(int j = pow(i, 2); j < n; j+=i){
        if(*(data+j)) {
          *(data+j) = 0;
        }
      }
    }
    for(int k = 2; k < n; k++) {
      if(*(data+k)) {
        print_number(k);
      }
    }
    free(data);
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}
