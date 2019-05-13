#!/usr/bin/env python3
import random

from deap import base
from deap import creator
from deap import tools

class Item():
    def __init__(self, n):
        self.weight = random.randint(0,n)
        self.value = random.randint(0,n)

    def __str__(self):
        return "(v: %i, w: %i)"%(self.value,self.weight)

    def __repr__(self):
        return "(v: %i, w: %i)"%(self.value,self.weight)

N_ITEMS = 40
MAX_WEIGHT = 5*N_ITEMS
items = [Item(N_ITEMS) for x in range(N_ITEMS)]

def evalKnapsack(individual):
    fitness = sum([ind*item.value for ind,item in zip(individual, items)])
    weight = sum([ind*item.weight for ind,item in zip(individual, items)])

    if weight > MAX_WEIGHT:
        fitness = 0

    return fitness, weight


def main():
    creator.create("FitnessMax", base.Fitness, weights=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMax)

    toolbox = base.Toolbox()
    toolbox.register("attr_bool", random.randint, 0, 1)


    toolbox.register("individual", tools.initRepeat, creator.Individual,
        toolbox.attr_bool, N_ITEMS)

    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", evalKnapsack)
    toolbox.register("mate", tools.cxTwoPoint)
    toolbox.register("mutate", tools.mutFlipBit, indpb=0.05)
    toolbox.register("select", tools.selTournament, tournsize=3)

    pop_size = 20;
    cros_over = 0.8;
    n_sons = int(pop_size*cros_over);
    n_ger = 50;
    mutation_rate = 0.2;

    pop = toolbox.population(n=pop_size)

    # Evaluate the entire population
    fitnesses = list(map(toolbox.evaluate, pop))
    for ind, fit in zip(pop, fitnesses):
        ind.fitness.values = fit
        setattr(ind,'weight', fit[1])

    # Variable keeping track of the number of generations
    g = 0
    while g < n_ger:
        # A new generation
        g = g + 1

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
            setattr(ind,'weight', fit[1])

        sorted_pop = sorted(pop, key=lambda ind: ind.fitness.values[0])
        pop[:(pop_size-n_sons)] = sorted_pop[-(pop_size-n_sons):]
        pop[(pop_size-n_sons):] = offspring

    best = sorted(pop, key=lambda ind: ind.fitness.values[0])[-1]

    print("Os items disponíveis são: ", items)
    print("O melhor resultado encontrado é: ", best)
    print("Seu valor é: ", best.fitness.values[0], " e o peso total é: ",best.weight)


if __name__ == '__main__':
    main()
