#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <random>

#include "parser.h"

#pragma once

class Solution{
public:
    std::vector<int> Set;       //is the beginning set
    std::vector<int> Mask;	    //is mask for set to generate solutions
    int time = 0;
    int timeInSeconds = 0;

    //-------------------------------------------------------------------------------------- Constructors
    //From file ( NOTE: If you put to high lineNumber it will create object with last input from file)
    Solution(std::string name, int lineNumber);

    //For existing set of numbers
    Solution(std::vector<int> setTab);

    Solution();
    //-------------------------------------------------------------------------------------- Getters

    //Returns value of subset (0 or 1)
    int getResultOfSubset(int subSetID);

    //-------------------------------------------------------------------------------------- Setters

    void setAll(std::string name, int lineNumber);

    void setAll(std::vector<int> setTab);

    void setTime(int time);

    void setTimeInSeconds(int time);
    //-------------------------------------------------------------------------------------- Stringify

    void toStringSet();

    void toStringMask();

    void toStringScore();

    //All info about Object
    void toString();

    //---------------------------------------------------------------------------------------------------------- Methods
    //Compare two objects
    bool equals(Solution s);

    //Generates first mask
    void generateSolution();

    std::vector<int> incrementVector(std::vector<int> vector, int position);
    std::vector<int> decrementVector(std::vector<int> vector, int position);

    //if vector is unwanted set it to {0,0,...,0,1} or {1,1,...,1,0}
    void checkForUnwantedMask(Solution s);

    //Generate object with new solution (for brute forcing solution)
    Solution generateIncrementedSolution(bool incrementing);

    //Generate list of neighbouring solutions (amount is equal to neighbours on each side). On left side solutions are reversed
    std::vector<Solution> neighbourList();

    Solution generateNeighbour();

    //Goal function
    int score();
};