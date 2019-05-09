#!/usr/bin/env python3
import random
import igraph

from deap import base
from deap import creator
from deap import tools

N_COLORS = 3
N_VERTICES = 7

graph = igraph.Graph()
graph.add_vertices(N_VERTICES)
graph.add_edges([(0,1),(0,2),(1,2),(1,3),(2,3),(2,4),(2,5),(3,4),(4,5)])

def evalGraph(individual):
    edges = graph.get_edgelist()
    fitness = 0
    for e in edges:
        if individual[e[0]] == individual[e[1]]:
            fitness = fitness + 1

    return fitness,

def main():
    creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMin)

    toolbox = base.Toolbox()
    toolbox.register("attr_color", random.randint, 0, N_COLORS-1)


    toolbox.register("individual", tools.initRepeat, creator.Individual,
        toolbox.attr_color, N_VERTICES)

    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", evalGraph)
    toolbox.register("mate", tools.cxTwoPoint)
    toolbox.register("mutate", tools.mutUniformInt,low=0, up=N_COLORS-1, indpb=0.05)
    toolbox.register("select", tools.selTournament, tournsize=3)

    pop_size = 20;
    cros_over = 0.5;
    n_sons = int(pop_size*cros_over);
    n_ger = 200;
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
