#include "solution.h"
#include <vector>
#pragma once

class Specimen {
public:
    Solution solution;

    //----------------------------------------------------------------------------------------------------- Constructors
    //Takes solution and make it specimen
    Specimen(Solution solution);

    Specimen();

    //---------------------------------------------------------------------------------------------------------- Methods

    //Calculates fitness for this specimen
    float fitness();

    //Mutates this specimen
    void mutate();
};