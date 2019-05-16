import tsplib95
import random
import math
import networkx

class Ant:
    def __init__(self, graph, alfa, beta):
        self.alfa = alfa
        self.beta = beta

        if graph != None:
            self.initial_node = random.choice(list(graph.nodes))
        else:
            self.initial_node = None
        self.current_node  = self.initial_node
        self.explored = [self.initial_node]
        self.edges = []

        self.path_dist = 0

    def __str__(self):
        return "(Path: %s, Path cost: %s)"%(str(self.explored), str(self.path_dist))

    def __repr__(self):
        return "(Path: %s, Path cost: %s)"%(str(self.explored), str(self.path_dist))

    def get_next(self, graph):
        neighbors = graph.neighbors(self.current_node)
        edges_data = [(n, graph.get_edge_data(self.current_node,n)) for n in neighbors if n not in self.explored]
        sum_p = sum([(e[1]["feromonio"]**self.alfa)*((1/e[1]["weight"])**self.beta) for e in edges_data])
        probability = [((e[1]["feromonio"]**self.alfa)*((1/e[1]["weight"])**self.beta))/sum_p for e in edges_data]

        roulette = [probability[0]]
        for p in range(1,len(probability)):
            roulette.append(roulette[p-1]+probability[p])

        spin = random.random()
        next_node = None
        for r in range(len(roulette)):
            if spin <= roulette[r]:
                next_node = r
                break

        self.edges.append((self.current_node, edges_data[r][0]))
        self.current_node = edges_data[r][0]
        self.explored.append(self.current_node)
        self.path_dist += edges_data[r][1]["weight"]

    def return_init(self, graph):
        self.path_dist += graph.get_edge_data(self.current_node, self.initial_node)["weight"]
        self.edges.append((self.current_node, self.initial_node))
        self.current_node = self.initial_node
        self.explored.append(self.current_node)

    def find_path(self, graph):
        iter = 0
        while(iter < graph.number_of_nodes()-1):
            iter += 1
            self.get_next(graph)

        self.return_init(graph)



def main():

    def euclidean_float(a, b):
        x1,y1 = a
        x2,y2 = b
        dist = math.sqrt((x1-x2)**2 + (y1-y2)**2)
        return dist

    Q = 1
    rho = 0.8
    alfa = 1
    beta = 4
    n_iter_max = 50
    best_ant = Ant(None, alfa, beta)
    best_ant.path_dist = 99999999

    n_exec = 0
    n_exec_max = 1

    problem = tsplib95.load_problem('st70.tsp', special=euclidean_float)
    while(n_exec < n_exec_max):
        print("Execution number: ", n_exec)
        graph = problem.get_graph()
        n_ants = graph.number_of_nodes()

        for e in graph.edges:
            graph[e[0]][e[1]]['feromonio'] = 1


        n_iter = 0
        while(n_iter < n_iter_max):
            ants_list = [Ant(graph, alfa, beta) for x in range(n_ants)]
            for a in ants_list:
                a.find_path(graph)

            for e in graph.edges:
                graph[e[0]][e[1]]['feromonio'] *= (1-rho)

            for a in ants_list:
                for e in a.edges:
                    graph[e[0]][e[1]]['feromonio'] += Q/a.path_dist

            best_ant_aux = sorted(ants_list, key=lambda a: a.path_dist)[0]
            if(best_ant_aux.path_dist < best_ant.path_dist):
                best_ant = best_ant_aux
                print("Improved")

            n_iter = n_iter + 1
        n_exec += 1
    print("Best result: ", best_ant)

if __name__ == '__main__':
    main()
