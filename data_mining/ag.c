#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include <string.h>

struct regra{
  double pesos[34];
  int atributos[34];
  int operadores[34]; // =, !=, <, >=
  double sp, se;
  double av;
};

void readFile(int p_tre[][35], int tre_size, int p_tst[][35], int tst_size);
void quickSort(struct regra r[], int low, int high);
int partition (struct regra r[], int low, int high);
void swap(struct regra *a, struct regra *b);

void geraPop(struct regra r[], int n_regras);
void geraAv(struct regra r[], int n_regras, int pacientes[][35], int n_pacientes, int classe);
void tourEst(struct regra r[], int n_regras,  int p_index[], int n_sons, int tour_size);
void crossOver(struct regra r[], struct regra r_sons[], int p_index[], int n_sons, double mutate_percent);
void updatePop(struct regra r[], struct regra r_sons[], int n_regras, int n_sons);
void printRegras(struct regra r[], int n_regras);

int main(){
  //PARAMETROS DO AG
  int n_regras = 50;
  double cross_over = 1;
  int n_sons = cross_over*n_regras;
  double mutate_percent = 0.3;
  int n_ger = 50;

  //Pacientes de treinamento e teste
  int p_tre[239][35];
  int p_tst[119][35];
  readFile(p_tre,239,p_tst,119);

  //Pais e Filhos
  struct regra r[n_regras];
  struct regra r_sons[n_sons];
  int p_index[n_sons];

  srand(999);

  for(int classe=1; classe<=6; classe++){
    geraPop(r, n_regras);
    geraAv(r, n_regras, p_tre, 239, classe);

    for (size_t geracao = 0; geracao <n_ger; geracao++) {
      tourEst(r, n_regras, p_index, n_sons, 3);
      crossOver(r, r_sons, p_index, n_sons, mutate_percent);
      geraAv(r_sons, n_sons, p_tre, 239, classe);
      updatePop(r, r_sons, n_regras, n_sons);
    }
    printf("Classe numero %d \n",classe);
    printRegras(r, n_regras);
  }
}

void geraPop(struct regra r[], int n_regras){
  for(int i=0; i < n_regras; i++){
    for(int j=0; j<34; j++){
        r[i].pesos[j] =(rand()%1001)/1000.0;
        r[i].atributos[j] = rand()%4;
        r[i].operadores[j] = rand()%4;
    }
    r[i].atributos[10] = rand()%2;
    r[i].atributos[33] = rand()%71;
  }
}

void geraAv(struct regra r[], int n_regras, int pacientes[][35], int n_pacientes, int classe){
  double max_peso = 0.3;
  for (size_t i = 0; i < n_regras; i++) {
    int tp=0, tn=0, fp=0, fn=0;
    for (size_t j = 0; j < n_pacientes; j++) {
      bool is_class = true;

      for (size_t w = 0; w < 34; w++) {
        if(r[i].pesos[w]<max_peso){
          switch (r[i].operadores[w]) {
            case 0:
              is_class &= (pacientes[j][w] == r[i].atributos[w]);
            break;
            case 1:
              is_class &= (pacientes[j][w] != r[i].atributos[w]);
            break;
            case 2:
              is_class &= (pacientes[j][w] < r[i].atributos[w]);
            break;
            case 3:
              is_class &= (pacientes[j][w] >= r[i].atributos[w]);
            break;
          }

          if(!is_class) break;
        }
      }

      if(pacientes[j][34]==classe && is_class){
        tp++;
      }else if(pacientes[j][34]!=classe && is_class){
        fp++;
      }else if(pacientes[j][34]!=classe && !is_class){
        tn++;
      }else if((pacientes[j][34]==classe) && !is_class){
        fn++;
      }
    }
    // printf("tp: %d, fp: %d, tn: %d, fn: %d \n",tp,fp,tn,fn);

    r[i].sp = (double)tp/(tp+fn+.1);
    r[i].se= (double)tn/(tn+fp+.1);
    r[i].av = r[i].sp * r[i].se;
    // printf("Av: %f \n",r[i].av);
  }
  // printf("Avaliando o conjunto de regras\n");
}

void tourEst(struct regra r[], int n_regras,  int p_index[], int n_sons, int tour_size){
  for (size_t i = 0; i < n_sons; i++) {
    int index_tour[tour_size];
    double roulette[tour_size];
    double av_total = 0;

    for (size_t j = 0; j < tour_size; j++) {
      index_tour[j] = (rand()%n_regras);
      av_total += r[(rand()%n_regras)].av;
    }

    if(av_total!=0){

      for (size_t w = 0; w < tour_size; w++) {
          roulette[w] = r[index_tour[w]].av / av_total;
      }

      double chance = (rand()%1001)/1000;
      for (size_t k = 0; k < tour_size; k++) {
        if(chance <= roulette[k]){
          p_index[i] = index_tour[k];
        }
      }
    }else{
      p_index[i] = index_tour[0];
    }

    for (size_t m = 0; m < i; m++) {
      if(p_index[i] == p_index[m]){
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

void crossOver(struct regra r[], struct regra r_sons[], int p_index[], int n_sons, double mutate_percent){
  int n_points =2;
  int points[n_points];
  for (size_t i = 0; i < n_sons-1; i+=2) {

    for (size_t j = 0; j < n_points; j++) {
      points[j] = rand()%33;
      for (size_t w = 0; w < j; w++) {
        if(points[j]==points[w]){
          --j;
          break;
        }
      }
    }

    r_sons[i] = r[i];
    r_sons[i+1] = r[i+1];
    for (size_t m = 0; m < n_points; m++) {
      struct regra aux;
      aux = r_sons[i];
      memcpy(r_sons[i].pesos, r_sons[i+1].pesos, sizeof(double) *points[m]);
      memcpy(r_sons[i].atributos, r_sons[i+1].atributos, sizeof(int) *points[m]);
      memcpy(r_sons[i].operadores, r_sons[i+1].operadores, sizeof(int) *points[m]);

      memcpy(r_sons[i+1].pesos, aux.pesos, sizeof(double) *points[m]);
      memcpy(r_sons[i+1].atributos, aux.atributos, sizeof(int) *points[m]);
      memcpy(r_sons[i+1].operadores, aux.operadores, sizeof(int) *points[m]);

    }
    if((rand()%1001/1000.0)<=0.3){
      r_sons[i].pesos[(rand()%34)] = (rand()%1001/1000.0);
    }
    if((rand()%1001/1000.0)<=0.3){
      int index = (rand()%34);
      if(index!=10 && 33){
        r_sons[i].atributos[index] = (rand()%4);
      }else if(index == 10){
        r_sons[i].atributos[index] = (rand()%2);
      }else if(index == 33){
        r_sons[i].atributos[index] = (rand()%71);
      }
    }
    if((rand()%1001/1000.0)<=0.3){
      r_sons[i].operadores[(rand()%34)] = (rand()%4);
    }
    if((rand()%1001/1000.0)<=0.3){
      r_sons[i+1].pesos[(rand()%34)] = (rand()%1001/1000.0);
    }
    if((rand()%1001/1000.0)<=0.3){
      int index = (rand()%34);
      if(index!=10 && 33){
        r_sons[i+1].atributos[index] = (rand()%4);
      }else if(index == 10){
        r_sons[i+1].atributos[index] = (rand()%2);
      }else if(index == 33){
        r_sons[i+1].atributos[index] = (rand()%71);
      }
    }
    if((rand()%1001/1000.0)<=0.3){
      r_sons[i+1].operadores[(rand()%34)] = (rand()%4);
    }

  }
  // printf("\npai1: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r[0].atributos[i]);
  // }
  // printf("\npai2: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r[1].atributos[i]);
  // }
  // printf("\nfil1: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r_sons[0].atributos[i]);
  // }
  // printf("\nfil2: ");
  // for (size_t i = 0; i < 34; i++) {
  //   printf("%d ",r_sons[1].atributos[i]);
  // }
  // printf("\n");
}

void updatePop(struct regra r[], struct regra r_sons[], int n_regras, int n_sons){
  struct regra aux[n_regras + n_sons];
  memcpy(aux,r,sizeof(struct regra)*n_regras);
  memcpy(&aux[50],r_sons,sizeof(struct regra)*n_sons);

  quickSort(aux,0, n_regras+n_sons-1);
  memcpy(r, aux, sizeof(struct regra)*n_regras);
}

void printRegras(struct regra r[], int n_regras){
  for (size_t i = 0; i < n_regras; i++) {
    // for (size_t j = 0; j < 34; j++) {
      // printf("%d ",r[i].operadores[j]);
      // printf("%f ",r[i].pesos[j]);
    // }
    printf("%f ",r[i].av);
    printf("\n");
  }
}

void readFile(int p_tre[][35], int tre_size, int p_tst[][35], int tst_size){
  FILE *fp = fopen("data_base", "r");

  for (size_t i = 0; i < tre_size; i++) {
    for (size_t j = 0; j < 35; j++) {
      fscanf(fp,"%d,",&p_tre[i][j]);
    }
  }
  for (size_t i = 0; i < tst_size; i++) {
    for (size_t j = 0; j < 35; j++) {
      fscanf(fp,"%d,",&p_tst[i][j]);
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

void quickSort(struct regra r[], int low, int high){
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(r, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(r, low, pi - 1);
        quickSort(r, pi + 1, high);
    }
}

int partition (struct regra r[], int low, int high){
  // int pivot = arr[high];    // pivot
  double pivot = r[high].av;    // pivot

  int i = (low - 1);  // Index of smaller element

  for (int j = low; j <= high- 1; j++)
  {
    // If current element is smaller than or
    // equal to pivot
    // printf("Pivot: %f av:%f \n", pivot, r[j].av);
    if (r[j].av >= pivot)
    {
      i++;    // increment index of smaller element
      swap(&r[i], &r[j]);
    }
  }
  swap(&r[i + 1], &r[high]);
  return (i + 1);
}

void swap(struct regra *a, struct regra *b){
  struct regra t = *a;
  *a = *b;
  *b = t;
}
