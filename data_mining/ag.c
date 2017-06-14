#include "stdio.h"
#include "stdlib.h"

struct regra{
  double pesos[34];
  int atributos[34];
  int operadores[34]; // =, !=, <, >=
};

void readFile(int p_tre[][34], int tre_size, int p_tst[][34], int tst_size);

void geraPop(struct regra r[], int n_regras);
void geraAv(struct regra r[], int n_regras, char pacientes[][34], int n_pacientes );
void roletaEst();
void crossOver();
void atualizaPop();
void printRegras(struct regra r[], int n_regras);

int main(){
  int p_tre[239][34];
  int p_tst[119][34];
  readFile(p_tre,239,p_tst,119);

  srand(666);

  int n_regras = 30;
  struct regra r[n_regras];
  geraPop(r, n_regras);

  // printRegras(r, n_regras);
}

void geraPop(struct regra r[], int n_regras){
  for(int i=0; i < n_regras; i++){
    for(int j=0; j<34; j++){
        r[i].pesos[j] = rand()%1001 / 1000;
        r[i].atributos[j] = rand()%4;
        r[i].operadores[j] = rand()%4;
    }
    r[i].atributos[10] = rand()%2;
    r[i].atributos[33] = rand()%71;
  }
}

void geraAv(struct regra r[], int n_regras, char pacientes[][34], int n_pacientes ){
  printf("Avaliando o conjunto de regras\n");
}

void printRegras(struct regra r[], int n_regras){
  for (size_t i = 0; i < n_regras; i++) {
    for (size_t j = 0; j < 34; j++) {
      printf("%d ",r[i].atributos[j]);
    }
    printf("\n");
  }
}

void readFile(int p_tre[][34], int tre_size, int p_tst[][34], int tst_size){
  FILE *fp = fopen("data_base", "r");

  for (size_t j = 0; j < tre_size; j++) {
    for (size_t j = 0; j < 34; j++) {
      fscanf(fp,"%d,",&p_tre[j][j]);
    }
  }
  for (size_t j = 0; j < tst_size; j++) {
    for (size_t j = 0; j < 34; j++) {
      fscanf(fp,"%d,",&p_tst[j][j]);
    }
  }
  fclose(fp);
}
