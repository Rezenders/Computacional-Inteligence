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
        self.path_weight = 0
        self.path_value = 0

    def __str__(self):
        return "(Items: %s, value: %s, weight: %s)"%(str(self.explored), str(self.path_value),str(self.path_weight))

    def __repr__(self):
        return "(Items: %s, value: %s, weight: %s)"%(str(self.explored), str(self.path_value),str(self.path_weight))

    def calc_f(self, graph, nodes):
        value = self.calc_value(graph, nodes)
        weight = self.calc_weight(graph, nodes)
        return (value - 1*min(0,graph.graph["capacity"] - weight))

    def calc_value(self, graph, nodes):
        return sum([graph.node[e]['value'] for e in nodes])

    def calc_weight(self, graph, nodes):
        return sum([graph.node[e]['weight'] for e in nodes])

    def get_next(self, graph):
        neighbors = graph.neighbors(self.current_node)
        edges_data = [(n, graph.get_edge_data(self.current_node,n)) for n in neighbors if n not in self.explored]

        sum_p = sum([(e[1]["feromonio"]**self.alfa)*((self.calc_f(graph, self.explored + [e[0]]) - self.calc_f(graph, self.explored))**self.beta) for e in edges_data])
        probability = [((e[1]["feromonio"]**self.alfa)*((self.calc_f(graph, self.explored + [e[0]]) - self.calc_f(graph, self.explored))**self.beta))/sum_p for e in edges_data]

        roulette = [probability[0]]
        for p in range(1,len(probability)):
            roulette.append(roulette[p-1]+probability[p])

        spin = random.random()
        next_node = None
        for r in range(len(roulette)):
            if spin <= roulette[r]:
                next_node = r
                break

        end_condition = False
        if self.calc_weight(graph, self.explored +[edges_data[r][0]]) < graph.graph["capacity"]:
            self.edges.append((self.current_node, edges_data[r][0]))
            self.current_node = edges_data[r][0]
            self.explored.append(self.current_node)
            self.path_dist = self.calc_f(graph, self.explored)
            self.path_weight = self.calc_weight(graph, self.explored)
            self.path_value = self.calc_value(graph, self.explored)
            print("path dist ", self.path_dist, "path_weight ", self.path_weight)
        else:
            print("terminou")
            end_condition = True

        return end_condition


    def find_path(self, graph):
        iter = 0
        end_condition = False
        while(iter < graph.number_of_nodes()-1 and not end_condition):
            iter += 1
            end_condition = self.get_next(graph)


        # self.return_init(graph)

def main():

    Q = 1
    rho = 0.8
    alfa = 1
    beta = 4
    n_iter_max = 1

    best_ant = Ant(None, alfa, beta)
    best_ant.path_dist = 0
    best_ant.path_value = 0

    n_exec = 0
    n_exec_max = 1

    while(n_exec < n_exec_max):
        print("Execution number: ", n_exec)

        # weight = [95, 4,60,32,23,72,80,62,65,46 ]
        # value = [55,10,47,5, 4,50,8,61,85,87]
        # c = 269

        weight = [92, 4,43,83,84,68,92,82,6,44,32,18,56,83, 25,96,70,48,14,58]
        value = [44,46,90,72,91, 40,75,35,8,54, 78, 40,77,15,61,17,75,29,75,63]
        c=878

        graph = networkx.complete_graph(len(weight))
        n_ants = graph.number_of_nodes()

        graph.graph["capacity"] = c
        for n in graph:
            graph.node[n]['weight'] = weight[n]
            graph.node[n]['value'] = value[n]

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
                    graph[e[0]][e[1]]['feromonio'] += a.path_dist

            best_ant_aux = sorted(ants_list, key=lambda a: a.path_value)[-1]
            if(best_ant_aux.path_value > best_ant.path_value):
                best_ant = best_ant_aux
                print("Improved")

            n_iter = n_iter + 1
        n_exec += 1
    print("Best result: ", best_ant)



if __name__ == '__main__':
    main()
