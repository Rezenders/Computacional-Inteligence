#include "stdio.h"
#include "stdlib.h"

struct regra{
  double pesos[34];
  int atributos[34];
  int operadores[34]; // =, !=, <, >=
};

void geraPop(struct regra r[], int n_regras);
void geraAv(struct regra r[], int n_regras, char pacientes[][34], int n_pacientes );
void roletaEst();
void crossOver();
void atualizaPop();

int main(){
  FILE *fp = fopen("data_base", "r");

  srand(666);

  int n_regras = 30;
  struct regra r[n_regras];
  geraPop(r, n_regras);
}

void geraPop(struct regra r[], int n_regras){
  for(int i=0; i < n_regras; i++){
    for(int j=0; j<34; j++){
        r[i].pesos[j] = rand()%1001 / 1000;
        r[i].atributos[j] = rand()%4;
        r[i].operadores[j] = rand()%4;
    }
    r[i].atributos[10] = rand()%2;
    r[i].atributos[33] = rand()%2;
  }
  printf("Regras geradas aleatoriamente\n");
}

void geraAv(struct regra r[], int n_regras, char pacientes[][34], int n_pacientes ){
  printf("Avaliando o conjunto de regras\n");
}
