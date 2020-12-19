#include "population.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
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
    //Setup
    std::random_device rd;
    std::vector<Specimen> partners;

    //Calculate overall fitness
    float fitnessOverall = 0.0;
    for(auto e: this->population){
        fitnessOverall += e.fitness();
    }

    //Population sorted by fitness (for roulette checker)
    std::vector<Specimen> populationSorted = this->population;
    std::sort(populationSorted.begin(), populationSorted.end(), [](Specimen a, Specimen b){return a.fitness() > b.fitness();});

    //Main loop (that handles partner.size() beside whole algorithm)
    for (int i = 0; i < this->population.size(); ++i) {
        //Generate random fitness of Specimen that will become partner
        std::uniform_real_distribution<float> fitDistribution(0, fitnessOverall);
        float fitnessGenerated = fitDistribution(rd);

        //Checking which Specimen is to become partner based on fitness
        float fitnessAccumulator = 0.0;
        for (int j = 0; j < this->population.size(); ++j) {
            //Increase fitnessAccumulator
            fitnessAccumulator += populationSorted.at(j).fitness();

            //Check did we find right Specimen
            if(fitnessAccumulator > fitnessGenerated){
                partners.push_back(populationSorted.at(j));
                break;
            }
        }
    }

    return partners;
}

std::vector<Specimen> Population::selectionTournament(int participantsAmount){
    std::uniform_int_distribution<int> populationDist(0, this->population.size()-1);
    std::random_device rd;
    std::vector<Specimen> partners;

    for (int i = 0; i < this->population.size(); i++) {
        //Tournament participants vector
        std::vector<Specimen> participants;

        //Filling vector with participants
        for (int j = 0; j < participantsAmount; j++) {
            participants.push_back(this->population.at(populationDist(rd)));
        }

        //Sorting
        std::sort(participants.begin(), participants.end(), [](Specimen a, Specimen b){return a.fitness() > b.fitness();});

        //Push back best from participants
        partners.push_back(participants.at(0));
    }

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
