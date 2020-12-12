#include "specimen.h"
#include <vector>
#pragma once

class Population {
public:
    std::vector<Specimen> population;

    //----------------------------------------------------------------------------------------------------- Constructors
    //Take population as my population
    Population(std::vector<Specimen> population);

    //Generates population with solution generation
    Population(Solution solution, int populationSize);

    //---------------------------------------------------------------------------------------------------------- Methods
    //Finds two specimens to reproduce from this population
    std::vector<Specimen> selection();

    //Crosses specimens from selection to make whole new population
    void cross();
};