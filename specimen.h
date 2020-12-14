#include "solution.h"
#include <vector>
#include <random>
#pragma once

class Specimen {
public:
    Solution solution;

    //----------------------------------------------------------------------------------------------------- Constructors
    //Takes solution and make it specimen
    Specimen(Solution solution);

    Specimen();

    //---------------------------------------------------------------------------------------------------------- Methods

    /*
     *
     * Calculates fitness of this specimen by following formula:
     * 1/(1 + score)  <- where score is method of Solution class
     */
    float fitness();

    /*
     *
     * Mutates this specimen by setting value at random position in Mask of this Solution object to opposite
     *
     * ARGUMENTS:
     * - mutationAmount = 1, this is how many mutation you want to make
     * - chance = 0.1, this is the chance of each mutation to occur (default is 10%)
     */
    void mutate(int mutationAmount = 1, float chance = 0.1);
};