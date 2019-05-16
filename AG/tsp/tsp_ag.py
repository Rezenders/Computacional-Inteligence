#!/usr/bin/env python3
import random
import networkx
import tsplib95

from deap import base
from deap import creator
from deap import tools

def euclidean_float(a, b):
    x1,y1 = a
    x2,y2 = b
    dist = math.sqrt((x1-x2)**2 + (y1-y2)**2)
    return dist

problem = tsplib95.load_problem('st70.tsp', special=euclidean_float)
graph = problem.get_graph()
N_VERTICES = graph.number_of_nodes()

def cxCycle(ind1, ind2):
    length = len(ind1)
    index = random.randint(0, length-1)
    cycle = [0]*length

    while(not cycle[index]):
        cycle[index] = 1;
        for i in range(0, length):
            if ind1[index] == ind2[i]:
                index = i
                break

    for j in range(0,length):
      if (cycle[j] == 1):
        temp = ind1[j]
        ind1[j] = ind2[j];
        ind2[j] = temp;

    return ind1,ind2


def evalGraph(individual):
    size = len(individual)
    fitness = 0
    for i in range(1,size):
        edge = graph.get_edge_data(individual[i-1],individual[i])
        fitness += edge["weight"]

    fitness += graph.get_edge_data(1,size)["weight"]

    return fitness,

def main():
    creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMin)

    toolbox = base.Toolbox()
    toolbox.register("indices", random.sample, range(1,N_VERTICES+1), N_VERTICES)
    toolbox.register("individual", tools.initIterate, creator.Individual,
                     toolbox.indices)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", evalGraph)
    toolbox.register("mate", cxCycle)
    toolbox.register("mutate", tools.mutShuffleIndexes,indpb=0.05)
    toolbox.register("select", tools.selTournament, tournsize=3)

    pop_size = 200;
    cros_over = 0.8;
    n_sons = int(pop_size*cros_over);
    n_ger = 2000;
    mutation_rate = 0.2;

    pop = toolbox.population(n=pop_size)

    # Evaluate the entire population
    fitnesses = list(map(toolbox.evaluate, pop))
    for ind, fit in zip(pop, fitnesses):
        ind.fitness.values = fit

    # Variable keeping track of the number of generations
    g = 0
    while g < n_ger:
        # A new generation
        g = g + 1
        print("-- Generation %i --" % g)

        #Select parents
        parents = toolbox.select(pop, n_sons)
        # Clone the selected individuals
        offspring = list(map(toolbox.clone, parents))

        for child1, child2 in zip(offspring[::2], offspring[1::2]):
            toolbox.mate(child1, child2)

        for mutant in offspring:
            if random.random() < mutation_rate:
                toolbox.mutate(mutant)

        fitnesses = list(map(toolbox.evaluate, offspring))
        for ind, fit in zip(offspring, fitnesses):
            ind.fitness.values = fit

        sorted_pop = sorted(pop, key=lambda ind: ind.fitness.values[0])
        pop[:(pop_size-n_sons)] = sorted_pop[-(pop_size-n_sons):]
        pop[(pop_size-n_sons):] = offspring

        sorted_pop = sorted(pop, key=lambda ind: ind.fitness.values[0])
        av_min = sorted_pop[0].fitness.values[0]
        av_max = sorted_pop[-1].fitness.values[0]
        fits = [ind.fitness.values[0] for ind in pop]

        mean = sum(fits) / pop_size
        sum2 = sum(x*x for x in fits)
        std = abs(sum2 / pop_size - mean**2)**0.5

        print("  Best ", sorted_pop[0])
        print("  Min %s" % av_min)
        print("  Max %s" % av_max)
        print("  Avg %s" % mean)
        print("  Std %s" % std)

        if av_min==0:
            break

if __name__ == '__main__':
    main()
