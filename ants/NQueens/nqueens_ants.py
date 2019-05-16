import random
import math
import networkx


class Ant:
    def __init__(self, graph, alfa, beta):
        self.alfa = alfa
        self.beta = beta

        if graph is not None:
            self.initial_node = random.choice(list(graph.nodes))
        else:
            self.initial_node = None
        self.current_node = self.initial_node
        self.explored = [self.initial_node]
        self.edges = []

        self.path_dist = 0

    def __str__(self):
        return "(Items: %s, fit: %s)" % (str(self.explored), str(self.path_dist))

    def __repr__(self):
        return "(Items: %s, fit: %s)" % (str(self.explored), str(self.path_dist))

    def calc_f(self, nodes):
        t1 = 0  # number of repetitive queens in one diagonal while seen from left corner
        t2 = 0  # number of repetitive queens in one diagonal while seen from right corner

        size = len(nodes)
        f1 = []
        f2 = []
        for i in range(size):
            f1.append(nodes[i] - i)
            f2.append((1 + size) - nodes[i] - i)

        f1.sort()
        f2.sort()

        for i in range(1, size):
            if(f1[i] == f1[i - 1]):
                t1 = t1 + 1
            if(f2[i] == f2[i - 1]):
                t2 = t2 + 1

        fitness = t1 + t2
        return fitness

    def get_next(self, graph):
        neighbors = graph.neighbors(self.current_node)
        edges_data = [(n, graph.get_edge_data(self.current_node, n))
                      for n in neighbors if n not in self.explored]

        aux = []
        for e in edges_data:
            delta = (self.calc_f(self.explored +
                                 [e[0]]) - self.calc_f(self.explored))
            if delta == 0:
                delta = 0.1

            aux.append((e[1]["feromonio"]**self.alfa) * ((1 / delta)**self.beta))
        sum_p = sum(aux)

        probability = []
        for a in aux:
            probability.append(a/sum_p)

        roulette = [probability[0]]
        for p in range(1, len(probability)):
            roulette.append(roulette[p - 1] + probability[p])

        spin = random.random()
        next_node = None
        for r in range(len(roulette)):
            if spin <= roulette[r]:
                next_node = r
                break

        self.edges.append((self.current_node, edges_data[r][0]))
        self.current_node = edges_data[r][0]
        self.explored.append(self.current_node)
        self.path_dist = self.calc_f(self.explored)

    def find_path(self, graph):
        iter = 0
        while(iter < graph.number_of_nodes() - 1):
            iter += 1
            self.get_next(graph)


def main():

    Q = 1
    rho = 0.8
    alfa = 1
    beta = 4
    n_iter_max = 20

    best_ant = Ant(None, alfa, beta)
    best_ant.path_dist = 999999

    n_exec = 0
    n_exec_max = 1

    while(n_exec < n_exec_max):
        print("Execution number: ", n_exec)

        BOARD_SIZE = 32
        graph = networkx.complete_graph(BOARD_SIZE)
        n_ants = graph.number_of_nodes()

        for e in graph.edges:
            graph[e[0]][e[1]]['feromonio'] = 1

        n_iter = 0
        solution = False
        while(n_iter < n_iter_max and not solution):
            ants_list = [Ant(graph, alfa, beta) for x in range(n_ants)]
            for a in ants_list:
                a.find_path(graph)

            for e in graph.edges:
                graph[e[0]][e[1]]['feromonio'] *= (1 - rho)

            for a in ants_list:
                if a.path_dist ==0:
                    solution = True
                    break

                for e in a.edges:
                    graph[e[0]][e[1]]['feromonio'] += 1 / a.path_dist

            best_ant_aux = sorted(ants_list, key=lambda a: a.path_dist)[0]
            if(best_ant_aux.path_dist < best_ant.path_dist):
                best_ant = best_ant_aux
                print("Improved")

            n_iter = n_iter + 1
        n_exec += 1
    print("Best result: ", best_ant)


if __name__ == '__main__':
    main()
