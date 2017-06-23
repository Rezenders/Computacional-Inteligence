#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include <string.h>

struct rule {
  double weights[34];
  int att[34];
  int operators[34]; // =, !=, <, >=
  double sp, se;
  double av;
  double tst;
};

const char *diseases[34] = {"erythema",
                            "scaling",
                            "definite borders",
                            "itching",
                            "koebner phenomenon ",
                            "polygonal papules ",
                            "follicular papules ",
                            "oral mucosal involvement ",
                            "knee and elbow involvement ",
                            "scalp involvement",
                            "family history",
                            "melanin incontinence",
                            "eosinophils in the infiltrate",
                            "PNL infiltrate",
                            "fibrosis of the papillary dermis",
                            "exocytosis",
                            "acanthosis",
                            "hyperkeratosis",
                            "parakeratosis",
                            "clubbing of the rete ridges",
                            "elongation of the rete ridges",
                            "thinning of the suprapapillary epidermis",
                            "spongiform pustule",
                            "munro microabcess",
                            "focal hypergranulosis",
                            "disappearance of the granular layer",
                            "vacuolisation and damage of basal layer",
                            "spongiosis",
                            "saw-tooth appearance of retes",
                            "follicular horn plug",
                            "perifollicular parakeratosis",
                            "inflammatory monoluclear inflitrate",
                            "band-like infiltrate",
                            "Age"};

void readFile(int p_tre[][35], int tre_size, int p_tst[][35], int tst_size);
void quickSort(struct rule r[], int low, int high);
int partition(struct rule r[], int low, int high);
void swap(struct rule *a, struct rule *b);

void genPop(struct rule r[], int n_rules);
void getAv(struct rule r[], int n_rules, int patients[][35], int n_patients,
           int class);
void getTst(struct rule r[], int n_rules, int patients[][35], int n_patients,
            int class);
void tourEst(struct rule r[], int n_rules, int p_index[], int n_sons,
             int tour_size);
void crossOver(struct rule r[], struct rule r_sons[], int p_index[], int n_sons,
               double mutate_percent);
void updatePop(struct rule r[], struct rule r_sons[], int n_rules, int n_sons);
void printRule(struct rule r, int class);
void printRulesMembers(struct rule r[], int n_rules);
void printBestRuleAv(struct rule r[], int n_rules);
struct rule getBest(struct rule r[], int n_rules);

double max_weight = 0.2;

int main() {
  // PARAMETROS DO AG
  int n_rules = 50;
  double cross_over = 1;
  int n_sons = cross_over * n_rules;
  double mutate_percent = 0.3;
  int n_ger = 50;
  int n_exe = 10;

  // patients de treinamento e teste
  int p_tre[239][35];
  int p_tst[119][35];
  readFile(p_tre, 239, p_tst, 119);

  // Pais e Filhos
  struct rule r[n_rules];
  struct rule r_sons[n_sons];
  int p_index[n_sons];

  srand(666);

  for (int class = 1; class <= 6; class ++) {
    struct rule best;
    double mean_av =0;
    double mean_test =0;
    best.av = 0;
    for (int exec = 0; exec < n_exe; exec++) {
      genPop(r, n_rules);
      getAv(r, n_rules, p_tre, 239, class);

      for (size_t geracao = 0; geracao < n_ger; geracao++) {
        tourEst(r, n_rules, p_index, n_sons, 3);
        crossOver(r, r_sons, p_index, n_sons, mutate_percent);
        getAv(r_sons, n_sons, p_tre, 239, class);
        updatePop(r, r_sons, n_rules, n_sons);
      }
      getTst(r, n_rules, p_tst, 119, class);
      struct rule aux = getBest(r, n_rules);
      mean_av += aux.av;
      mean_test += aux.tst;
      printRule(aux, class);
      if (aux.av > best.av) {
        best = aux;
      }
    }
    printf("Classe %d --- Média Av: %f Média test %f \n",class, mean_av/n_exe, mean_test/n_exe);
    printf("\n\n\n");

    // printf("class numero %d \n",class);
    // printBestRuleAv(r, n_rules);
    // printRule(best, class);
  }
}

void genPop(struct rule r[], int n_rules) {
  for (int i = 0; i < n_rules; i++) {
    for (int j = 0; j < 34; j++) {
      r[i].weights[j] = (rand() % 1001) / 1000.0;
      r[i].att[j] = rand() % 4;
      r[i].operators[j] = rand() % 4;
    }
    r[i].att[10] = rand() % 2;
    r[i].att[33] = rand() % 71;
  }
}

void getAv(struct rule r[], int n_rules, int patients[][35], int n_patients,
           int class) {

  for (size_t i = 0; i < n_rules; i++) {
    int tp = 0, tn = 0, fp = 0, fn = 0;
    for (size_t j = 0; j < n_patients; j++) {
      bool is_class = true;

      for (size_t w = 0; w < 34; w++) {
        if (r[i].weights[w] < max_weight) {
          switch (r[i].operators[w]) {
          case 0:
            is_class &= (patients[j][w] == r[i].att[w]);
            break;
          case 1:
            is_class &= (patients[j][w] != r[i].att[w]);
            break;
          case 2:
            is_class &= (patients[j][w] < r[i].att[w]);
            break;
          case 3:
            is_class &= (patients[j][w] >= r[i].att[w]);
            break;
          }

          if (!is_class)
            break;
        }
      }

      if (patients[j][34] == class && is_class) {
        tp++;
      } else if (patients[j][34] != class && is_class) {
        fp++;
      } else if (patients[j][34] != class && !is_class) {
        tn++;
      } else if ((patients[j][34] == class) && !is_class) {
        fn++;
      }
    }
    // printf("tp: %d, fp: %d, tn: %d, fn: %d \n",tp,fp,tn,fn);

    // r[i].sp = (double)tp / (tp + fp + .1);
    // r[i].se = (double)tn / (tn + fn + .1);
    r[i].sp = (double)tp / (tp + fn + .1);
    r[i].se = (double)tn / (tn + fp + .1);
    r[i].av = r[i].sp * r[i].se;
    // printf("Av: %f \n",r[i].av);
  }
  // printf("Avaliando o conjunto de regras\n");
}
void getTst(struct rule r[], int n_rules, int patients[][35], int n_patients,
            int class) {

  for (size_t i = 0; i < n_rules; i++) {
    int tp = 0, tn = 0, fp = 0, fn = 0;
    for (size_t j = 0; j < n_patients; j++) {
      bool is_class = true;

      for (size_t w = 0; w < 34; w++) {
        if (r[i].weights[w] < max_weight) {
          switch (r[i].operators[w]) {
          case 0:
            is_class &= (patients[j][w] == r[i].att[w]);
            break;
          case 1:
            is_class &= (patients[j][w] != r[i].att[w]);
            break;
          case 2:
            is_class &= (patients[j][w] < r[i].att[w]);
            break;
          case 3:
            is_class &= (patients[j][w] >= r[i].att[w]);
            break;
          }

          if (!is_class)
            break;
        }
      }

      if (patients[j][34] == class && is_class) {
        tp++;
      } else if (patients[j][34] != class && is_class) {
        fp++;
      } else if (patients[j][34] != class && !is_class) {
        tn++;
      } else if ((patients[j][34] == class) && !is_class) {
        fn++;
      }
    }
    // printf("tp: %d, fp: %d, tn: %d, fn: %d \n",tp,fp,tn,fn);

    // r[i].sp = (double)tp / (tp + fp + .1);
    // r[i].se = (double)tn / (tn + fn + .1);
    r[i].sp = (double)tp / (tp + fn + .1);
    r[i].se = (double)tn / (tn + fp + .1);
    r[i].tst = r[i].sp * r[i].se;
    // printf("Av: %f \n",r[i].av);
  }
  // printf("Avaliando o conjunto de regras\n");
}

void tourEst(struct rule r[], int n_rules, int p_index[], int n_sons,
             int tour_size) {
  for (size_t i = 0; i < n_sons; i++) {
    int index_tour[tour_size];
    double roulette[tour_size];
    double av_total = 0;

    for (size_t j = 0; j < tour_size; j++) {
      index_tour[j] = (rand() % n_rules);
      av_total += r[(rand() % n_rules)].av;
    }

    if (av_total != 0) {

      for (size_t w = 0; w < tour_size; w++) {
        roulette[w] = r[index_tour[w]].av / av_total;
      }

      double chance = (rand() % 1001) / 1000;
      for (size_t k = 0; k < tour_size; k++) {
        if (chance <= roulette[k]) {
          p_index[i] = index_tour[k];
        }
      }
    } else {
      p_index[i] = index_tour[0];
    }

    for (size_t m = 0; m < i; m++) {
      if (p_index[i] == p_index[m]) {
        --i;
        break;
      }
    }
  }
  // printf("\n");
  // for (size_t i = 0; i < n_sons; i++) {
  //   printf("%d\n", p_index[i]);
  // }
}

void crossOver(struct rule r[], struct rule r_sons[], int p_index[], int n_sons,
               double mutate_percent) {
  int n_points = 2;
  int points[n_points];
  for (size_t i = 0; i < n_sons - 1; i += 2) {

    for (size_t j = 0; j < n_points; j++) {
      points[j] = rand() % 33;
      for (size_t w = 0; w < j; w++) {
        if (points[j] == points[w]) {
          --j;
          break;
        }
      }
    }

    r_sons[i] = r[i];
    r_sons[i + 1] = r[i + 1];
    for (size_t m = 0; m < n_points; m++) {
      struct rule aux;
      aux = r_sons[i];
      memcpy(r_sons[i].weights, r_sons[i + 1].weights,
             sizeof(double) * points[m]);
      memcpy(r_sons[i].att, r_sons[i + 1].att, sizeof(int) * points[m]);
      memcpy(r_sons[i].operators, r_sons[i + 1].operators,
             sizeof(int) * points[m]);

      memcpy(r_sons[i + 1].weights, aux.weights, sizeof(double) * points[m]);
      memcpy(r_sons[i + 1].att, aux.att, sizeof(int) * points[m]);
      memcpy(r_sons[i + 1].operators, aux.operators, sizeof(int) * points[m]);
    }
    if ((rand() % 1001 / 1000.0) <= mutate_percent) {
      r_sons[i].weights[(rand() % 34)] = (rand() % 1001 / 1000.0);
    }
    if ((rand() % 1001 / 1000.0) <= mutate_percent) {
      int index = (rand() % 34);
      if (index != 10 && 33) {
        r_sons[i].att[index] = (rand() % 4);
      } else if (index == 10) {
        r_sons[i].att[index] = (rand() % 2);
      } else if (index == 33) {
        r_sons[i].att[index] = (rand() % 71);
      }
    }
    if ((rand() % 1001 / 1000.0) <= mutate_percent) {
      r_sons[i].operators[(rand() % 34)] = (rand() % 4);
    }
    if ((rand() % 1001 / 1000.0) <= mutate_percent) {
      r_sons[i + 1].weights[(rand() % 34)] = (rand() % 1001 / 1000.0);
    }
    if ((rand() % 1001 / 1000.0) <= mutate_percent) {
      int index = (rand() % 34);
      if (index != 10 && 33) {
        r_sons[i + 1].att[index] = (rand() % 4);
      } else if (index == 10) {
        r_sons[i + 1].att[index] = (rand() % 2);
      } else if (index == 33) {
        r_sons[i + 1].att[index] = (rand() % 71);
      }
    }
    if ((rand() % 1001 / 1000.0) <= mutate_percent) {
      r_sons[i + 1].operators[(rand() % 34)] = (rand() % 4);
    }
  }
  // printf("\npai1: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r[0].att[i]);
  // }
  // printf("\npai2: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r[1].att[i]);
  // }
  // printf("\nfil1: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r_sons[0].att[i]);
  // }
  // printf("\nfil2: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r_sons[1].att[i]);
  // }
  // printf("\n");
}

void updatePop(struct rule r[], struct rule r_sons[], int n_rules, int n_sons) {
  struct rule aux[n_rules + n_sons];
  memcpy(aux, r, sizeof(struct rule) * n_rules);
  memcpy(&aux[50], r_sons, sizeof(struct rule) * n_sons);

  quickSort(aux, 0, n_rules + n_sons - 1);
  memcpy(r, aux, sizeof(struct rule) * n_rules);
}

void printRule(struct rule r, int class) {
  printf("\nTraining av is %f Test av is %f \n", r.av, r.tst);
  printf("The resulting rule for class %d is: \n", class);
  for (size_t i = 0; i < 34; i++) {
    if (r.weights[i] < max_weight) {
      printf("att: %s ", diseases[i]);
      switch (r.operators[i]) {
      case 0:
        printf("== ");
        break;
      case 1:
        printf("!= ");
        break;
      case 2:
        printf("< ");
        break;
      case 3:
        printf(">= ");
        break;
      }
      printf("%d \n", r.att[i]);
    }
  }
}

void printRulesMembers(struct rule r[], int n_rules) {
  for (size_t i = 0; i < n_rules; i++) {
    // for (size_t j = 0; j < 34; j++) {
    // printf("%d ",r[i].operators[j]);
    // printf("%f ",r[i].weights[j]);
    // }
    printf("%f ", r[i].av);
    printf("\n");
  }
}

struct rule getBest(struct rule r[], int n_rules) {
  struct rule aux = r[0];
  for (size_t i = 1; i < n_rules; i++) {
    if (r[i].av > aux.av) {
      aux = r[i];
    }
  }
  return aux;
}
void printBestRuleAv(struct rule r[], int n_rules) {
  struct rule best = getBest(r, n_rules);
  printf("Best Av: %f\n", best.av);
}

void readFile(int p_tre[][35], int tre_size, int p_tst[][35], int tst_size) {
  FILE *fp = fopen("data_base", "r");

  for (size_t i = 0; i < tre_size; i++) {
    for (size_t j = 0; j < 35; j++) {
      fscanf(fp, "%d,", &p_tre[i][j]);
    }
  }
  for (size_t i = 0; i < tst_size; i++) {
    for (size_t j = 0; j < 35; j++) {
      fscanf(fp, "%d,", &p_tst[i][j]);
    }
  }

  // for (size_t i = 0; i < tre_size; i++) {
  //   printf("\nleu: ");
  //   for (size_t j = 0; j < 35; j++) {
  //     printf("%d ", p_tre[i][j]);
  //   }
  // }
  // printf("sp\n");
  // for (size_t i = 0; i < tst_size; i++) {
  //   printf("\nleu: ");
  //   for (size_t j = 0; j < 35; j++) {
  //     printf("%d ", p_tst[i][j]);
  //   }
  // }
  fclose(fp);
}

void quickSort(struct rule r[], int low, int high) {
  if (low < high) {
    /* pi is partitioning index, arr[p] is now
       at right place */
    int pi = partition(r, low, high);

    // Separately sort elements before
    // partition and after partition
    quickSort(r, low, pi - 1);
    quickSort(r, pi + 1, high);
  }
}

int partition(struct rule r[], int low, int high) {
  // int pivot = arr[high];    // pivot
  double pivot = r[high].av; // pivot

  int i = (low - 1); // Index of smaller element

  for (int j = low; j <= high - 1; j++) {
    // If current element is smaller than or
    // equal to pivot
    // printf("Pivot: %f av:%f \n", pivot, r[j].av);
    if (r[j].av >= pivot) {
      i++; // increment index of smaller element
      swap(&r[i], &r[j]);
    }
  }
  swap(&r[i + 1], &r[high]);
  return (i + 1);
}

void swap(struct rule *a, struct rule *b) {
  struct rule t = *a;
  *a = *b;
  *b = t;
}
