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
    //Creates vector Specimen of partners to be passed to cross method by roulette alg
    std::vector<Specimen> selectionRoulette();

    /*
     * Selects population.size() partners from population by tournament algorithm
     *
     * ARGUMENTS:
     * - participantsAmount is number of participants of each tournament
     *
     * RETURN:
     * - std::vector<Specimen> partners, that stores partners to make new generation
     */
    std::vector<Specimen> selectionTournament(int participantsAmount);

    /*
     *
     * Takes two next partners in vector and make cross of their Mask (which is almost like genome ;) )
     *
     * ARGUMENTS:
     * - partners are stored in vector<Specimen> (NOTE: two partners should be one after another)
     * - crossPointAmount is number of crossing between partners (default = 1)
     * - chance is exactly that - chance of crossing partners (default = 1.0)
     *
     * RETURNS:
     * - std::vector<Specimen> newGeneration, that holds whole new generation
     */
    std::vector<Specimen> cross(std::vector<Specimen> partners, int crossPointsAmount = 1, float chance = 1.0);
};