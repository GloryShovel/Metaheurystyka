#include "specimen.h"

//--------------------------------------------------------------------------------------------------------- Constructors

Specimen::Specimen(Solution solution){
    this->solution = solution;
}

//Might be useful
Specimen::Specimen(){}

//-------------------------------------------------------------------------------------------------------------- Methods

float Specimen::fitness(){
    return (float)1/(1 + this->solution.score());
}

void Specimen::mutate(int mutationAmount, float chance){
    std::uniform_real_distribution<float> realDistribution(0,1);
    std::uniform_int_distribution<int> intDistribution(0, this->solution.Mask.size()-1);
    std::random_device rd;

    //Mutation loop
    for(int i=0; i < mutationAmount; i++){
        //Chance of mutating
        float randomReal = realDistribution(rd);
        if(chance > randomReal){
            //Generate position to mutate
            int position = intDistribution(rd);
            //Mutate at position
            this->solution.Mask.at(position) = (this->solution.Mask.at(position)+1) % 2;
        }
    }

}


