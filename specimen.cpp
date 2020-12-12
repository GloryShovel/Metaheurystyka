#include "specimen.h"

//--------------------------------------------------------------------------------------------------------- Constructors
//Takes solution and make it specimen
Specimen::Specimen(Solution solution){
    this->solution = solution;
}

//Might be useful
Specimen::Specimen(){}

//-------------------------------------------------------------------------------------------------------------- Methods

//Calculates fitness for this specimen
float Specimen::fitness(){
    return  float(1/(1 + this->solution.score()));
}

//Mutates this specimen
void Specimen::mutate(){

    //In loop generate mutationsAmount, positions where to mutate this specimen
    //if you want, make mutations randomly (some might not mutate)

}


