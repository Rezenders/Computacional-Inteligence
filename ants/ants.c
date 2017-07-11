#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

struct Distances{
  int id;
  int dist;
};
struct Node{
  int id;
  int x;
  int y;
  struct Distances *d;
};

int main(int argc, char *argv[]) {
  char *file_path = NULL;
  size_t len = 0;
  ssize_t read;

  if (argc <= 1) {
    printf("Digite o arquivo que contém os nós do problema do caixeiro "
           "viajante\n");
    while (!(read = getline(&file_path, &len, stdin))) {
    };
  } else {
    file_path = argv[1];
  }
  printf("Arquivo a ser lido: %s \n", file_path);

  FILE *fp = fopen(file_path, "r");
  if (fp == NULL)
    fputs("Failed to open file!\n", stderr);

  char dist_type;
  int n_nodes;
  fscanf(fp,"%s", &dist_type);
  fscanf(fp,"%d", &n_nodes);

  struct Node nodes[n_nodes];
  // nodes->d = (struct Distances*) malloc(n_nodes*sizeof(struct Distances));

  if(dist_type =='c'){
    for (size_t i = 0; i < n_nodes; i++) {
      nodes[i].d = (struct Distances*) malloc(n_nodes*sizeof(struct Distances));
      fscanf( fp, "%d %d %d", &nodes[i].id, &nodes[i].x, &nodes[i].y);
    }

    for (size_t j = 0; j < n_nodes; j++) {
      for (size_t k = 0; k < n_nodes; k++) {
        long int d_x = nodes[j].x - nodes[k].x;
        // printf("d_x %d \n", d_x);
        long int d_y = nodes[j].y - nodes[k].y;

        nodes[j].d[k].dist = sqrt(pow(d_x, 2) + pow(d_y, 2));
        // printf("dist calculada %d \n",sqrt(pow(d_x, d_x) + pow(d_y, d_y)));
        printf("dist calculada %d \n",nodes[j].d[k].dist);
      }
      printf("\n");
    }
  }else if(dist_type =='d'){

  }else{
    printf("Primeiro caracter do arquivo de nós está errado. Deve ser c ou d\n");
  }
  // getline(&teste, &len, fp);

  // free(file_path);
  // free(teste);
  // free(nodes->d);
  for (size_t i = 0; i < n_nodes; i++) {
    free(nodes[i].d);
  }
  fclose(fp);
}
