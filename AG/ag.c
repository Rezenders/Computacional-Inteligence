#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <string.h>

void generatePop(int pop[][11], int pop_size);
void sortRandom(int array[]);
void sortOrderAv(int array[][11], unsigned int array_size);
void getAv(int array[]);
void getMinMaxAv();
void tour(int parents[][11], int p_index[], int tour_size, int n_sons);
void crossOverAll(int parents[][11], int p_index[], int sons[][11], int n_sons,
                  int mutate_percent);
void crossOver(int parent1[], int parent2[], int son1[], int son2[],
               int mutate_percent);
void mutation(int array[]);
void updatePop(int parents[][11], int p_size, int sons[][11], int n_sons);

int av_min = 1000000000, av_max = 0;
int av_mini = 0, av_maxi = 0;

int main() {
  // sendmory
  int pop_size = 100;
  int n_sons = 80;
  int mutate_percent = 10;

  int p[pop_size][11];
  int p_index[80];
  int sons[n_sons][11];

  srand((unsigned)666);

  generatePop(p, pop_size);

  tour(p, p_index, 3, n_sons);
  crossOverAll(p, p_index, sons, n_sons, mutate_percent);

}

void generatePop(int pop[][11], int pop_size) {
  int n[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (int j = 0; j < pop_size; j++) {
    sortRandom(n);
    memcpy(pop[j], n, sizeof(int) * 10);

    getAv(pop[j]);

    if (pop[j][10] < av_min) {
      av_min = pop[j][10];
      av_mini = j;
    }

    if (pop[j][10] > av_max) {
      av_max = pop[j][10];
      av_maxi = j;
    }
  }

  sortOrderAv(pop, pop_size);

  printf("Av min: %i  Av min index: %i\n", av_min, av_mini);
  printf("Av max: %i  Av max index: %i\n", av_max, av_maxi);
}

void sortRandom(int array[]) {
  for (int i = 9; i > 0; --i) {
    // index
    int w = rand() % i;
    // swap
    int t = array[i];
    array[i] = array[w];
    array[w] = t;
  }
}

void sortOrderAv(int array[][11], unsigned int array_size){
  int a[11];
  for (int i = 0; i < array_size; ++i)
  {
      for (int j = i + 1; j < array_size; ++j)
      {
          if (array[i][10] > array[j][10])
          {
              memcpy(a, array[i], sizeof(int)*11);
              memcpy(array[i], array[j], sizeof(int)*11);
              memcpy(array[j], a, sizeof(int)*11);
          }
      }
  }
}

void getAv(int array[]) {
  int aux;
  aux = (array[0] * 1000 + array[1] * 100 + array[2] * 10 + array[3]) +
        (array[4] * 1000 + array[5] * 100 + array[6] * 10 + array[1]) -
        (array[4] * 10000 + array[5] * 1000 + array[2] * 100 + array[1] * 10 +
         array[7]);
  array[10] = abs(aux);
}

void tour(int parents[][11], int p_index[], int tour_size, int n_sons) {

  for (int i = 0; i < n_sons; i++) {
    int best_av = 0;
    int best_index = -1;

    for (int n = 0; n < 2 * tour_size; n += 2) {
      int aux_i = rand() % 100;
      int aux_av = parents[aux_i][10];

      if (aux_av > best_av) {
        best_av = aux_av;
        best_index = aux_i;
      }

      p_index[i] = best_index;
    }
  }
}

void crossOverAll(int parents[][11], int p_index[], int sons[][11], int n_sons,
                  int mutate_percent) {
  for (size_t i = 0; i < n_sons; i += 2) {
    crossOver(parents[i], parents[i + 1], sons[i], sons[i + 1], mutate_percent);
  }
}

void crossOver(int parent1[], int parent2[], int son1[], int son2[],
               int mutate_percent) {
  unsigned int index = rand() % 10;
  unsigned int cycle[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  while (!cycle[index]) {
    cycle[index] = 1;
    for (size_t i = 0; i < 10; i++) {
      if (parent2[index] == parent1[i]) {
        index = i;
        break;
      }
    }
  }

  for (size_t j = 0; j < 10; j++) {
    if (cycle[j] == 1) {
      son1[j] = parent1[j];
      son2[j] = parent2[j];
    } else {
      son1[j] = parent2[j];
      son2[j] = parent1[j];
    }
  }
  if ((rand() % 101) < mutate_percent) {
    mutation(son1);
  }
  if ((rand() % 101) < mutate_percent) {
    mutation(son2);
  }

  getAv(son1);
  getAv(son2);
}

void mutation(int array[]) {
  int index = rand() % 8;
  int temp = array[index];
  array[index] = array[8];
  array[8] = array[index];
}

void updatePop(int parents[][11], int p_size, int sons[][11], int n_sons){
  memcpy(parents[p_size-n_sons], sons, sizeof(int)*11*n_sons);
}
