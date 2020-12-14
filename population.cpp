#include "population.h"
#include <vector>
#include <algorithm>
#include <functional>
#pragma once

//--------------------------------------------------------------------------------------------------------- Constructors
//Take population as my population
Population::Population(std::vector<Specimen> population){
    this->population = population;
}

Population::Population(Solution solution, int populationSize){
    Specimen pusher(solution);
    population.push_back(pusher);
    for (int i = 0; i < populationSize-1; i++) {
        Specimen nextOne = pusher;
        nextOne.solution.generateSolution();
        population.push_back(nextOne);
    }
}

//-------------------------------------------------------------------------------------------------------------- Methods

std::vector<Specimen> Population::selectionRoulette(){
    std::vector<Specimen> partners;

//    Here make proper selection roulet method

    return partners;
}

std::vector<Specimen> Population::selectionTournament(){
    std::vector<Specimen> partners = this-> population;

//    Here make proper selection tournament method

    return partners;
}

std::vector<Specimen> Population::cross(std::vector<Specimen> partners, int crossPointsAmount, float chance){
    //Setup
    std::vector<Specimen> newGeneration;

    //Safety
    if(crossPointsAmount > partners.at(0).solution.Mask.size()){
        std::cout << "There is to much cross points. ABORTING CROSSING" << std::endl;
        return newGeneration;
    }

    //All partners iterator
    for (int i = 0; i < partners.size(); i+=2) {
        //Offspring
        Specimen first = partners.at(i);
        Specimen second = partners.at(i+1);

        //Generating crossing points
        std::uniform_int_distribution<int> distribution(0, first.solution.Mask.size());
        std::random_device rd;
        std::vector<int> crossPoints;
        crossPoints.push_back(distribution(rd));
        int crossesLeft = crossPointsAmount-1;
        while(crossesLeft > 0) {
            int point = distribution(rd);
            //Prevent duplication
            bool dontPrevent = true;
            for(int iter=0; iter < crossPoints.size(); iter++){
                if(crossPoints.at(iter) == point){
                    dontPrevent = false;
                }
            }
            if(dontPrevent){
                crossPoints.push_back(point);
                crossesLeft--;
            }
        }

        //Sort crossPoints
        std::sort(crossPoints.begin(), crossPoints.end(), [](int a, int b){return a>b;});
        for(auto e: crossPoints){
            std::cout << e << "," ;
        }
        std::cout << std::endl;

        //Chance to cross
        std::uniform_real_distribution<float> realDistribution(0, 1);
        if(chance > realDistribution(rd)) {

            //Crossing loop
            int isCrossing = 0;
            int upcomingCrossPoint = crossPoints.back();
            for (int j = 0; j < first.solution.Mask.size(); j++) {
                //Switcher
                if(j == upcomingCrossPoint){
                    isCrossing = (isCrossing +1) %2;
                    crossPoints.pop_back();
                    upcomingCrossPoint = crossPoints.back();
                }

                //Crosser
                if(isCrossing){
                    first.solution.Mask.at(j) = partners.at(i+1).solution.Mask.at(j);
                    second.solution.Mask.at(j) = partners.at(i).solution.Mask.at(j);
                }
            }

            //Push offspring to new generation
            newGeneration.push_back(first);
            newGeneration.push_back(second);
        }
        else{
            //Push parents to new generation
            newGeneration.push_back(partners.at(i));
            newGeneration.push_back(partners.at(i+1));
        }
    }
    return newGeneration;
}
