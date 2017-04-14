#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <string.h>

int main() {
  // sendmory
  int p[100][10];
  int n[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int av_min = 1000000000, av_max = 0;
  int av_mini = 0, av_maxi = 0;

  time_t current_time;
  srand((unsigned)666);
  // srand((unsigned) time(&current_time));

  for (int j = 0; j < 100; j++) {
    for (int i = 9; i > 0; --i) {
      // index
      int w = rand() % i;
      // swap
      int t = n[i];
      n[i] = n[w];
      n[w] = t;
    }

    memcpy(p[j], n, sizeof(int) * 10);

    p[j][11] =
        abs((p[j][0] * 1000 + p[j][1] * 100 + p[j][2] * 10 + p[j][3] * 1) +
            (p[j][4] * 1000 + p[j][5] * 100 + p[j][6] * 10 + p[j][1] * 1) -
            (p[j][4] * 10000 + p[j][5] * 1000 + p[j][2] * 100 + p[j][1] * 10 +
             p[j][7] * 1));

    if (p[j][11] < av_min) {
      av_min = p[j][11];
      av_mini = j;
    }

    if (p[j][11] > av_max) {
      av_max = p[j][11];
      av_maxi = j;
    }
  }
  // printf("size of int %i \n", sizeof(int));
  printf("Av min: %i  Av min index: %i\n", av_min, av_mini);
  printf("Av max: %i  Av max index: %i\n", av_max, av_maxi);
}
