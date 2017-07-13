#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
  int id;
  double x;
  double y;
  double *d;
  double *f;
  double *prob;
};

struct Ants {
  int ant_id;
  struct Node initial_node;
  struct Node current_node;
  int *explored_nodes;
  struct Node *node_order;
  int path_dist;
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
    // scanf("%s",file_path);
  } else {
    file_path = argv[1];
  }
  printf("Arquivo a ser lido: %s\n", file_path);

  FILE *fp = fopen(file_path, "r");
  if (fp == NULL)
    fputs("Failed to open file!\n", stderr);

  char dist_type;
  int n_nodes;
  fscanf(fp, "%s", &dist_type);
  fscanf(fp, "%d", &n_nodes);

  struct Node nodes[n_nodes];

  if (dist_type == 'c') {
    for (size_t i = 0; i < n_nodes; i++) {
      nodes[i].d = (double *)malloc(n_nodes * sizeof(double));
      nodes[i].f = (double *)malloc(n_nodes * sizeof(double));
      nodes[i].prob = (double *)malloc(n_nodes * sizeof(double));
      fscanf(fp, "%d %lf %lf", &nodes[i].id, &nodes[i].x, &nodes[i].y);
    }

    for (size_t j = 0; j < n_nodes; j++) {
      for (size_t k = 0; k < n_nodes; k++) {
        double d_x = nodes[j].x - nodes[k].x;
        double d_y = nodes[j].y - nodes[k].y;

        nodes[j].d[k] = sqrt(pow(d_x, 2) + pow(d_y, 2));
        nodes[j].f[k] = 1;
        nodes[j].prob[k] = 0;
        // printf("dist calculada %d \n",nodes[j].d[k]);
      }
      // printf("\n");
    }
  } else if (dist_type == 'd') {
    for (size_t i = 0; i < n_nodes; i++) {
      nodes[i].d = (double *)malloc(n_nodes * sizeof(double));
      nodes[i].f = (double *)malloc(n_nodes * sizeof(double));
      nodes[i].prob = (double *)malloc(n_nodes * sizeof(double));

      fscanf(fp, "%d", &nodes[i].id);
      nodes[i].x = 0;
      nodes[i].y = 0;
      for (size_t j = 0; j < n_nodes; j++) {
        fscanf(fp, "%lf", &nodes[i].d[j]);
        nodes[i].f[j] = 1;
        nodes[i].prob[j] = 0;
      }
    }
  } else {
    printf(
        "Primeiro caracter do arquivo de nós está errado. Deve ser c ou d\n");
  }
  double evap =0.5;
  int alpha = 1, beta = 2;
  int n_ants = n_nodes;
  struct Ants ants[n_ants];
  int n_exec = 50;
  int best_dist = 99999999;
  while (n_exec > 0) {
    int n_ger = 50;
    for (size_t i = 0; i < n_ants; i++) {
      ants[i].explored_nodes = (int *)malloc(n_nodes * sizeof(int));
      ants[i].node_order =
          (struct Node *)malloc((1 + n_nodes) * sizeof(struct Node));
      for (size_t j = 0; j < n_nodes; j++) {
        ants[i].explored_nodes[j] = 0;
      }
    }

    while (n_ger > 0) {
      for (size_t i = 0; i < n_ants; i++) {
        ants[i].ant_id = i;
        ants[i].initial_node = nodes[rand() % n_nodes];
        ants[i].current_node = ants[i].initial_node;
        ants[i].path_dist = 0;
        for (size_t j = 0; j < n_nodes; j++) {
          ants[i].explored_nodes[j] = 0;
        }
      }
      // for (size_t iterations = 0; iterations < 1; iterations++) {
      for (size_t iterations = 0; iterations < n_nodes; iterations++) {
        for (size_t a = 0; a < n_ants; a++) {
          ants[a].node_order[iterations] = ants[a].current_node;
          ants[a].explored_nodes[ants[a].current_node.id] = 1;

          int next_node = 0;
          double sum_fn = 0;
          for (size_t n1 = 0; n1 < n_nodes; n1++) {
            if ((!ants[a].explored_nodes[n1]) &&
                (ants[a].current_node.d[n1] != 0)) {
              sum_fn += (pow(ants[a].current_node.f[n1], alpha) *
                         pow(1.0 / ants[a].current_node.d[n1], beta));
              //  printf("visibilidade %f\n",   pow(ants[a].current_node.d[n1],
              //  beta));
            }
          }
          // printf("sum_fn %f\n",sum_fn);
          // Calcula probabilidade para os proximos nós possíveis e monta uma
          // especie de roleta na estrutura do nó
          // printf("\nProbabilidade dos nós formiga %d \n", a);
          for (size_t n2 = 0; n2 < n_nodes; n2++) {
            if ((!ants[a].explored_nodes[n2]) &&
                (ants[a].current_node.d[n2] != 0)) {
              ants[a].current_node.prob[n2] =
                  (pow(ants[a].current_node.f[n2], alpha) *
                   pow(1.0 / ants[a].current_node.d[n2], beta)) /
                  sum_fn;
            } else {
              ants[a].current_node.prob[n2] = 0;
            }
            if (n2 != 0) {
              ants[a].current_node.prob[n2] +=
                  ants[a].current_node.prob[n2 - 1];
            }
            // printf("%f\n", ants[a].current_node.prob[n2]);
          }

          double chance = (rand() % 1001) / 1000.0;
          for (size_t n3 = 0; n3 < n_nodes; n3++) {
            if ((ants[a].explored_nodes[n3] != 1) &&
                (chance < ants[a].current_node.prob[n3])) {
              next_node = n3;
              ants[a].explored_nodes[n3] = 1;
              ants[a].path_dist += ants[a].current_node.d[n3];
              ants[a].current_node = nodes[n3];
              break;
            }
          }
        }
      }

      for (size_t a = 0; a < n_ants; a++) {
        ants[a].node_order[n_nodes] = ants[a].initial_node;
        ants[a].path_dist += ants[a].current_node.d[ants[a].initial_node.id];
      }

      for (size_t n = 0; n < n_nodes; n++) {
        // printf("f ");
        for (size_t n2 = 0; n2 < n_nodes; n2++) {
          nodes[n].f[n2] *= (1 - evap);
          // printf(" %f ",nodes[n].f[n2]);
        }
        // printf("\n");
      }

      // NAO TESTADO
      for (size_t a = 0; a < n_ants; a++) {
        for (size_t n = 0; n < n_nodes; n++) {
          nodes[ants[a].node_order[n].id].f[ants[a].node_order[n + 1].id] +=
              1.0 / ants[a].path_dist;
          nodes[ants[a].node_order[n + 1].id].f[ants[a].node_order[n].id] +=
              1.0 / ants[a].path_dist;
        }
      }

      // printf("FEROMONIOS ");
      // for (size_t n = 0; n < n_nodes; n++) {
      //   printf("f ");
      //   for (size_t n2 = 0; n2 < n_nodes; n2++) {
      //     printf(" %f ",nodes[n].f[n2]);
      //   }
      //   printf("\n");
      // }
      n_ger--;
      // printf("ger:%d\n",n_ger);
    }

    // printf("Ant 4 initial_node %d \n", ants[4].initial_node.id);
    // for (size_t i = 0; i < n_nodes; i++) {
    //   printf("ants explored_nodes %d\n", ants[1].explored_nodes[i]);
    // }
    // for (size_t i = 0; i < n_nodes+1; i++) {
    //   printf("Node order %d\n", ants[4].node_order[i].id);
    // }

    for (size_t i = 0; i < n_ants; i++) {
      if (ants[i].path_dist < best_dist) {
        best_dist = ants[i].path_dist;
      }
      // printf("Total dist %d \n",ants[i].path_dist);
    }
    n_exec--;
    // printf("exec:%d\n",n_exec);
  }
  printf("Best dist %d \n", best_dist);
  // free(file_path);
  // free(teste);
  // free(nodes->d);
  for (size_t i = 0; i < n_nodes; i++) {
    free(nodes[i].d);
  }
  fclose(fp);
}
