#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <string.h>

void sort(int array[]);
int getAv(int array[]);
void getMinMaxAv();
void selectParents(int p_index[]);

int main() {
  // sendmory
  int p[100][11];
  int p_index[80];

  int n[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int av_min = 1000000000, av_max = 0;
  int av_mini = 0, av_maxi = 0;

  srand((unsigned)666);

  for (int j = 0; j < 100; j++) {
    sort(n);
    memcpy(p[j], n, sizeof(int) * 10);

    p[j][10] = getAv(p[j]);

    if (p[j][10] < av_min) {
      av_min = p[j][10];
      av_mini = j;
    }

    if (p[j][10] > av_max) {
      av_max = p[j][10];
      av_maxi = j;
    }
  }
  // printf("size of int %i \n", sizeof(int));
  printf("Av min: %i  Av min index: %i\n", av_min, av_mini);
  printf("Av max: %i  Av max index: %i\n", av_max, av_maxi);

  selectParents(p_index);
}

void sort(int array[]){
  for (int i = 9; i > 0; --i) {
    // index
    int w = rand() % i;
    // swap
    int t = array[i];
    array[i] = array[w];
    array[w] = t;
  }
}

int getAv(int array[]) {
  int aux;
  aux = (array[0] * 1000 + array[1] * 100 + array[2] * 10 + array[3]) +
        (array[4] * 1000 + array[5] * 100 + array[6] * 10 + array[1]) -
        (array[4] * 10000 + array[5] * 1000 + array[2] * 100 + array[1] * 10 +
         array[7]);
  return abs(aux);
}

void selectParents(int p_index[]){
  printf("INDEXS \n");
  for(int n =0; n <80; n++){
    p_index[n] = rand()%80;
    printf("%i \n", p_index[n]);
  }
}
