#include "stdio.h"

struct regra{
  double pesos[34];
  char atributos[34];
  char operadores[34];
};

void geraPop(struct regra r[], int n_regras);
void geraAv(struct regra r[], int n_regras, char pacientes[][34], int n_pacientes );
void roletaEst();
void crossOver();
void atualizaPop();

int main(){
  FILE *fp = fopen("data_base", "r");

  int n_regras = 30;
  struct regra r[n_regras];
  geraPop(r, n_regras);
}

void geraPop(struct regra r[], int n_regras){
  
  printf("Regras geradas aleatoriamente\n");
}

void geraAv(struct regra r[], int n_regras, char pacientes[][34], int n_pacientes ){
  printf("Avaliando o conjunto de regras\n");
}
