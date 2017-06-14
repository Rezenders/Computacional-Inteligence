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
void roletaEst();
void crossOver();
void atualizaPop();
void printRegras(struct regra r[], int n_regras);

int main(){
  int p_tre[239][35];
  int p_tst[119][35];
  readFile(p_tre,239,p_tst,119);

  srand(999);

  int n_regras = 50;
  for(int classe=1; classe<=1; classe++){
    struct regra r[n_regras];
    geraPop(r, n_regras);
    // printRegras(r, n_regras);
    geraAv(r, n_regras, p_tre, 239, 1);
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

      // if(!is_class){
      //   printf("oi\n");
      // }
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
    printf("Av: %f \n",r[i].av);
    // printf("Se: %f \n",r[i].se);
    // printf("Sp: %f \n",r[i].sp);
  }
  // printf("Avaliando o conjunto de regras\n");
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
