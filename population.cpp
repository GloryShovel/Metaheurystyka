#include "population.h"
#include <vector>
#pragma once

//--------------------------------------------------------------------------------------------------------- Constructors
//Take population as my population
Population::Population(std::vector<Specimen> population){
    this->population = population;
}

Population::Population(Solution solution, int populationSize){
    population.push_back(solution);
    for (int i = 0; i < populationSize; ++i) {
        Solution nextSolution = solution;
        nextSolution.generateSolution();
        population.push_back(nextSolution);
    }
}

//-------------------------------------------------------------------------------------------------------------- Methods

std::vector<Specimen> Population::selection(){
    std::vector<Specimen> partners;

//    Here make proper selection method like for example tournment (works way better than roulett)

    return partners;
}

void Population::cross(){
//    Use selection to make partners list
//    than cross each two partners in partner list to make two new Specimens
//    mutate each specimen
}
