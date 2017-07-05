#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numbers.h"

struct Neuron {
  float weights[31];
};

struct Number{
  uint8_t id;
  uint8_t tiles[30];
};


void init_numbers(struct Number num[], uint8_t n_numbers);
void init_weights(uint8_t config, struct Neuron n[], uint8_t n_neurons);
void print_Neuron(struct Neuron n[], uint8_t n_neurons);
void print_Number(struct Number num[], uint8_t n_numbers);
void single_neuron_train(struct Neuron *n, struct Number *num1, struct Number *num2);

int main() {
  srand(666);
  uint8_t config = 0;
  printf("Perceptron\nComo os pesos inicias devem ser "
         "configurados?\n1-Zerados\n2-Aleatorios\n");
  while (config != 1 && config != 2) {
    scanf("%u", &config);
  }

  uint8_t n_numbers = 2;
  struct Number num[n_numbers];
  init_numbers(num, n_numbers);
  print_Number(num, n_numbers);

  uint8_t n_neurons =1;
  struct Neuron n[n_neurons];
  init_weights(config, n, n_neurons);
  print_Neuron(n, n_neurons);

  single_neuron_train(&n[0], &num[0], &num[1]);

}

void init_numbers(struct Number num[], uint8_t n_numbers){
  for (size_t i = 0; i < n_numbers; i++) {
    if(i==0){
      memcpy(num[i].tiles, zero, 30*sizeof(uint8_t));
    }else if(i==1){
      memcpy(num[i].tiles, one, 30*sizeof(uint8_t));
    }
  }
}

void init_weights(uint8_t config, struct Neuron n[], uint8_t n_neurons){
  if(config==1){
    for (size_t neurons_size = 0; neurons_size < n_neurons; neurons_size++) {
      for (size_t i = 0; i < 31; i++) {
        n[neurons_size].weights[i]=0;
      }
    }
  }else if(config==2){
    for (size_t neurons_size = 0; neurons_size < n_neurons; neurons_size++) {
      for (size_t i = 0; i < 31; i++) {
        n[neurons_size].weights[i]=((rand()%6001)/1000.0) -3.0;
      }
    }
  }
}

void print_Neuron(struct Neuron n[], uint8_t n_neurons){
  for (size_t neurons_size = 0; neurons_size < n_neurons; neurons_size++) {
    printf("\n");
    for (size_t i = 0; i < 31; i++) {
      printf("%f ", n[neurons_size].weights[i] );
    }
    printf("\n");
  }
}

void print_Number(struct Number num[], uint8_t n_numbers){
  for (size_t n_size = 0; n_size < n_numbers; n_size++) {
    printf("\n");
    for (size_t i = 0; i < 30; i++) {
      if((i%5)==0){
        printf("\n");
      }
      printf("%u ", num[n_size].tiles[i] );
    }
    printf("\n");
  }
}

void single_neuron_train(struct Neuron *n, struct Number *num1, struct Number *num2){
  uint8_t d0=0, d1=1;
  float output0 = n->weights[0]*1;
  float output1 = n->weights[0]*1;

  do{
    for (size_t i = 1; i < 31; i++) {
      output0 += n->weights[i]*num1->tiles[i-1];
    }
    output0 = (output0>0);

    if(d0!=output0){
      n->weights[0] = n->weights[0] + (d0 - output0)*1;
      for (size_t j = 1; j < 31; j++) {
        n->weights[j] = n->weights[j] + (d0 - output0)*num1->tiles[j-1];
      }
    }

    for (size_t i = 1; i < 31; i++) {
      output1 += n->weights[i]*num2->tiles[i-1];
    }
    output1 = (output1>0);

    if(d1!=output1){
      n->weights[0] = n->weights[0] + (d1 - output1)*1;
      for (size_t j = 1; j < 31; j++) {
        n->weights[j] = n->weights[j] + (d1 - output1)*num2->tiles[j-1];
      }
    }

  }while(d0!=output0 || d1!=output1);

  printf("output0: %f output1: %f \n",output0, output1 );
}
