#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>

struct regra{
  double pesos[34];
  int atributos[34];
  int operadores[34]; // =, !=, <, >=
  double sp, se;
  double av;
};

void readFile(int p_tre[][35], int tre_size, int p_tst[][35], int tst_size);

void geraPop(struct regra r[], int n_regras);
void geraAv(struct regra r[], int n_regras, int pacientes[][35], int n_pacientes, int classe);
void tourEst(struct regra r[], int n_regras,  int p_index[], int n_sons, int tour_size);
void crossOver();
void atualizaPop();
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

  for(int classe=1; classe<1; classe++){
    geraPop(r, n_regras);
    // printRegras(r, n_regras);
    geraAv(r, n_regras, p_tre, 239, classe);

    for (size_t geracao = 0; geracao < n_ger; geracao++) {
      tourEst(r, n_regras, p_index, n_sons, 3);
    }

  }
}

void geraPop(struct regra r[], int n_regras){
  for(int i=0; i < n_regras; i++){
    for(int j=0; j<34; j++){
        r[i].pesos[j] = (double)(rand()%1001)/1000;
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
  }
  printf("\n");
  for (size_t i = 0; i < n_sons; i++) {
    printf("%d\n", p_index[i]);
  }
}

void printRegras(struct regra r[], int n_regras){
  for (size_t i = 0; i < n_regras; i++) {
    for (size_t j = 0; j < 34; j++) {
      printf("%d ",r[i].operadores[j]);
      // printf("%f ",r[i].pesos[j]);
    }
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
