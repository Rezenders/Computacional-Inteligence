#!/usr/bin/env python3
import random

from deap import base
from deap import creator
from deap import tools


def evaluateSend(i):
    result = (i[4] * 10000 + i[5] * 1000 + i[2] * 100 + i[1] * 10 + i[7]) \
            - (i[0] * 1000 + i[1] * 100 + i[2] * 10 + i[3]) \
            - (i[4] * 1000 + i[5] * 100 + i[6] * 10 + i[1])

    return abs(result),

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

def mutShuffle(ind):
    length = len(ind)
    index1 = random.randint(0, length-1)
    index2 = random.randint(0, length-1)
    temp = ind[index1]
    ind[index1] = ind[index2];
    ind[index2] = temp;
    return ind,


def main():
    creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
    creator.create("InvFitness", base.Fitness, weights=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMin, invfitness=creator.InvFitness)

    IND_SIZE = 10

    toolbox = base.Toolbox()
    toolbox.register("indices", random.sample, range(IND_SIZE), IND_SIZE)
    toolbox.register("individual", tools.initIterate, creator.Individual,
                     toolbox.indices)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", evaluateSend)
    toolbox.register("mate", cxCycle)
    toolbox.register("mutate", mutShuffle)
    toolbox.register("select", tools.selRoulette, fit_attr='invfitness')
    # toolbox.register("select", tools.selTournament, tournsize=3, fit_attr='invfitness')
    #toolbox.register("select", tools.selBest)

    pop_size = 200;
    cros_over = 0.8;
    n_sons = int(pop_size*cros_over);
    n_ger = 200;
    mutation_rate = 0.2;

    n_execucao = 0;
    n_execucao_max = 1000;
    n_zeros = 0;

    while n_execucao < n_execucao_max:
        pop = toolbox.population(n=pop_size)

        # Evaluate the entire population
        fitnesses = list(map(toolbox.evaluate, pop))
        for ind, fit in zip(pop, fitnesses):
            ind.fitness.values = fit
            ind.invfitness.values = (100000-fit[0],)

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
                ind.invfitness.values = (100000-fit[0],)


            #Reinserção Elitismo
            sorted_pop = sorted(pop, key=lambda ind: ind.fitness.values[0])
            pop[:(pop_size-n_sons)] = sorted_pop[:(pop_size-n_sons)]
            pop[(pop_size-n_sons):] = offspring

            fits = [ind.fitness.values[0] for ind in pop]
            av_min = min(fits)
            if av_min==0:
                n_zeros = n_zeros +1
                break

        print("Melhor avaliacao na execucao %i foi: %i" % (n_execucao, av_min))
        n_execucao = n_execucao +1

    print("Porcentagem de sucesso %f" % (n_zeros/n_execucao_max))

if __name__ == '__main__':
    main()
