#include "numbers.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Neuron {
  float weights[31];
};

struct Number {
  uint8_t id;
  uint8_t tiles[30];
};

void init_numbers(struct Number num[], uint8_t n_numbers);
void init_numbers_zeros(struct Number num[]);
void init_numbers_ones(struct Number num[]);
void init_weights(uint8_t config, struct Neuron n[], uint8_t n_neurons);
void print_Neuron(struct Neuron n[], uint8_t n_neurons);
void print_Number(struct Number num[], uint8_t n_numbers);
void double_neuron_train(struct Neuron *n, struct Number num1,
                         struct Number num2, uint8_t d0, uint8_t d1);
void single_neuron_train(struct Neuron *n, struct Number num, uint8_t d);
void test_neuron(struct Neuron n, struct Number num);
void test_neuron_numbers(struct Neuron n, struct Number num[], uint8_t n_nums);

int main() {
  srand(666);
  uint8_t config = 0;
  printf("Perceptron\nSelect inital weights "
         "configuration?\n1-Zeros\n2-Random\n");
  while (config != 1 && config != 2) {
    scanf("%u", &config);
  }

  uint8_t n_numbers = 6;
  struct Number num[n_numbers];
  init_numbers(num, n_numbers);

  struct Number bunch_zeros[10];
  init_numbers_zeros(bunch_zeros);
  struct Number bunch_ones[10];
  init_numbers_ones(bunch_ones);
  // print_Number(num, n_numbers);

  uint8_t n_neurons = 1;
  struct Neuron n[n_neurons];
  init_weights(config, n, n_neurons);
  // print_Neuron(n, n_neurons);

  double_neuron_train(&n[0], num[0], num[1], 0, 1);
  print_Neuron(n, n_neurons);
  printf("Teste dos numeros de 0-5\n");
  test_neuron_numbers(n[0], num, 6);

  printf("Teste das variações de 0\n");
  test_neuron_numbers(n[0], bunch_zeros, 10);

  printf("Teste das variações de 1\n");
  test_neuron_numbers(n[0], bunch_ones, 10);

  printf("\n------------------EXERCICIO 2 ------------------\n");
  uint8_t n_neurons_2 = 2;
  struct Neuron n2[n_neurons_2];
  init_weights(config, n2, n_neurons_2);

  double_neuron_train(&n2[0], num[0], num[1], 1,0);
  double_neuron_train(&n2[1], num[0], num[1], 0,1);
  printf("Pesos neuronios\n");
  print_Neuron(n2, n_neurons_2);
  printf("Teste dos numeros de 0-5\n");
  printf("neuronio do 0\n");
  test_neuron_numbers(n2[0], num, 6);
  printf("neuronio do 1\n");
  test_neuron_numbers(n2[1], num, 6);
  printf("Teste das variações de 0\n");
  printf("neuronio do 0\n");
  test_neuron_numbers(n2[0], bunch_zeros, 10);
  printf("neuronio do 1\n");
  test_neuron_numbers(n2[1], bunch_zeros, 10);
  printf("Teste das variações de 1\n");
  printf("neuronio do 0\n");
  test_neuron_numbers(n2[0], bunch_ones, 10);
  printf("neuronio do 1\n");
  test_neuron_numbers(n2[1], bunch_ones, 10);
}

void init_numbers(struct Number num[], uint8_t n_numbers) {
  for (size_t i = 0; i < n_numbers; i++) {
    switch (i) {
    case 0:
      num[i].id = i;
      memcpy(num[i].tiles, zero, 30 * sizeof(uint8_t));
      break;
    case 1:
      num[i].id = i;
      memcpy(num[i].tiles, one, 30 * sizeof(uint8_t));
      break;
    case 2:
      num[i].id = i;
      memcpy(num[i].tiles, two, 30 * sizeof(uint8_t));
      break;
    case 3:
      num[i].id = i;
      memcpy(num[i].tiles, three, 30 * sizeof(uint8_t));
      break;
    case 4:
      num[i].id = i;
      memcpy(num[i].tiles, four, 30 * sizeof(uint8_t));
      break;
    case 5:
      num[i].id = i;
      memcpy(num[i].tiles, five, 30 * sizeof(uint8_t));
      break;
    }
  }
}
void init_numbers_zeros(struct Number num[]) {
  for (size_t i = 0; i < 10; i++) {
    num[i].id =0;
  }
  memcpy(num[0].tiles, zero_1, 30 * sizeof(uint8_t));
  memcpy(num[1].tiles, zero_2, 30 * sizeof(uint8_t));
  memcpy(num[2].tiles, zero_3, 30 * sizeof(uint8_t));
  memcpy(num[3].tiles, zero_4, 30 * sizeof(uint8_t));
  memcpy(num[4].tiles, zero_5, 30 * sizeof(uint8_t));
  memcpy(num[5].tiles, zero_6, 30 * sizeof(uint8_t));
  memcpy(num[6].tiles, zero_7, 30 * sizeof(uint8_t));
  memcpy(num[7].tiles, zero_8, 30 * sizeof(uint8_t));
  memcpy(num[8].tiles, zero_9, 30 * sizeof(uint8_t));
  memcpy(num[9].tiles, zero_10, 30 * sizeof(uint8_t));
}
void init_numbers_ones(struct Number num[]) {
  for (size_t i = 0; i < 10; i++) {
    num[i].id =1;
  }
  memcpy(num[0].tiles, one_1, 30 * sizeof(uint8_t));
  memcpy(num[1].tiles, one_2, 30 * sizeof(uint8_t));
  memcpy(num[2].tiles, one_3, 30 * sizeof(uint8_t));
  memcpy(num[3].tiles, one_4, 30 * sizeof(uint8_t));
  memcpy(num[4].tiles, one_5, 30 * sizeof(uint8_t));
  memcpy(num[5].tiles, one_6, 30 * sizeof(uint8_t));
  memcpy(num[6].tiles, one_7, 30 * sizeof(uint8_t));
  memcpy(num[7].tiles, one_8, 30 * sizeof(uint8_t));
  memcpy(num[8].tiles, one_9, 30 * sizeof(uint8_t));
  memcpy(num[9].tiles, one_10, 30 * sizeof(uint8_t));
}

void init_weights(uint8_t config, struct Neuron n[], uint8_t n_neurons) {
  if (config == 1) {
    for (size_t neurons_size = 0; neurons_size < n_neurons; neurons_size++) {
      for (size_t i = 0; i < 31; i++) {
        n[neurons_size].weights[i] = 0;
      }
    }
  } else if (config == 2) {
    for (size_t neurons_size = 0; neurons_size < n_neurons; neurons_size++) {
      for (size_t i = 0; i < 31; i++) {
        n[neurons_size].weights[i] = ((rand() % 6001) / 1000.0) - 3.0;
      }
    }
  }
}

void print_Neuron(struct Neuron n[], uint8_t n_neurons) {
  for (size_t neurons_size = 0; neurons_size < n_neurons; neurons_size++) {
    printf("Neuron\n");
    for (size_t i = 0; i < 31; i++) {
      if ((i==1) || (i % 5) == 1) {
        printf("\n");
      }
      printf("%f ", n[neurons_size].weights[i]);
    }
    printf("\n");
  }
}

void print_Number(struct Number num[], uint8_t n_numbers) {
  for (size_t n_size = 0; n_size < n_numbers; n_size++) {
    printf("\n");
    for (size_t i = 0; i < 30; i++) {
      if ((i % 5) == 0) {
        printf("\n");
      }
      printf("%u ", num[n_size].tiles[i]);
    }
    printf("\n");
  }
}

void double_neuron_train(struct Neuron *n, struct Number num1,
                         struct Number num2, uint8_t d0, uint8_t d1) {
  float output0 = n->weights[0] * 1;
  float output1 = n->weights[0] * 1;
  int epoca = 0;
  do {
    for (size_t i = 1; i < 31; i++) {
      output0 += n->weights[i] * num1.tiles[i - 1];
    }
    output0 = (output0 > 0);

    if (d0 != output0) {
      n->weights[0] = n->weights[0] + (d0 - output0) * 1;
      for (size_t j = 1; j < 31; j++) {
        n->weights[j] = n->weights[j] + (d0 - output0) * num1.tiles[j - 1];
      }
    }

    for (size_t i = 1; i < 31; i++) {
      output1 += n->weights[i] * num2.tiles[i - 1];
    }
    output1 = (output1 > 0);

    if (d1 != output1) {
      n->weights[0] = n->weights[0] + (d1 - output1) * 1;
      for (size_t j = 1; j < 31; j++) {
        n->weights[j] = n->weights[j] + (d1 - output1) * num2.tiles[j - 1];
      }
    }
    ++epoca;
  } while (d0 != output0 || d1 != output1);

  printf("output%u: %f output%u: %f n_epocas: %d \n", num1.id,num2.id,output0, output1, epoca);
}

void single_neuron_train(struct Neuron *n, struct Number num, uint8_t d) {
  float output = n->weights[0] * 1;
  int epoca = 0;

  do {
    for (size_t i = 1; i < 31; i++) {
      output += n->weights[i] * num.tiles[i - 1];
    }
    output = (output > 0);

    if (d != output) {
      n->weights[0] = n->weights[0] + (d - output) * 1;
      for (size_t j = 1; j < 31; j++) {
        n->weights[j] = n->weights[j] + (d - output) * num.tiles[j - 1];
      }
    }

    ++epoca;
  } while (d != output);
  printf("id: %d output: %f n_epocas: %d \n",num.id, output, epoca);
}

void test_neuron_numbers(struct Neuron n, struct Number num[], uint8_t n_nums) {
  for (size_t i = 0; i < n_nums; i++) {
    test_neuron(n, num[i]);
  }
}
void test_neuron(struct Neuron n, struct Number num) {
  float output = n.weights[0] * 1;

  for (size_t i = 1; i < 31; i++) {
    output += n.weights[i] * num.tiles[i - 1];
  }
  output = (output > 0);
  printf("Test result id %d: %f \n", num.id, output);
}
