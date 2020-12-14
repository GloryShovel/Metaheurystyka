#include <math.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <string.h>
#include <algorithm>
#include <functional>

#include "parser.h"
#include "solution.h"
#include "specimen.h"
#include "population.h"

//------------------------------------------------------------------------------------------------------------ Functions
std::vector<int> generateVector(){
    std::random_device rd;
    std::uniform_int_distribution<int> distribute(0,1000);
    int amount = 0;
    std::cout << "How many elements in vector?" << std::endl;
    std::cin >> amount;
    std::cout << std::endl;

    std::vector<int> generatedExample;
    for(int i=0; i < amount; i++){
        generatedExample.push_back(distribute(rd));
    }
    return generatedExample;
}

//--------------------------------------------------------------------------------------------Metaheuristics
Solution bruteForce(Solution solution){
    //If problem is too big don't bother calculating
    if(solution.Set.size() > 20){
        Solution s = solution;
        s.Set = {0};
        s.Mask = {0};
        return s;
    }

    clock_t time = clock();
    Solution next = solution, best = solution;
    do{
       next = next.generateIncrementedSolution(true);
       if(next.score() < best.score()){
           best = next;
       }
       //TODO:  save this to make plot out of it
//       std::cout << next.score() << std::endl;
    } while (next.Mask != solution.Mask);

    //Write time of execution in console
    time = clock() - time;
    best.setTime(time);
    best.setTimeInSeconds(time/CLOCKS_PER_SEC);
    return best;
}

Solution hillClimb(Solution solution){
    clock_t time = clock();
    Solution best, nextBest = solution;
    do{
        best = nextBest;
        std::vector<Solution> neighbours = nextBest.neighbourList();
        for (int i = 0; i < neighbours.size(); i++) {
            if(neighbours.at(i).score() < nextBest.score()){
                nextBest = neighbours.at(i);
            }
        }
    } while(best.score() != nextBest.score());

    //Write time of execution in console
    time = clock() - time;
    best.setTime(time);
    best.setTimeInSeconds(time/CLOCKS_PER_SEC);
    return best;
}

//tabu always needs iterations
Solution tabu(Solution solution, int iterations){

    clock_t time = clock();
    Solution best = solution, nextBest = solution;
    std::vector<Solution> tabuList;
    tabuList.push_back(nextBest);

    for(int i = 0; i<iterations; i++){
        if(best.score() > nextBest.score()){
            best = nextBest;
        }

        //if neighbour is better then nextBest then search tabuList (did we already saw this Solution)
        std::vector<Solution> neighbours = nextBest.neighbourList();

//        //Dev Stuff - writes neighbours list
//        for (int i = 0; i < neighbours.size(); ++i) {
//            neighbours.at(i).toStringMask();
//        }
//        std::cout << "After" << std::endl;

        //Remove tabu elements form neighbours
        for(int i = 0; i < tabuList.size(); i++){
            int j = 0;
            while (j < neighbours.size()){
                if(neighbours.at(j).equals(tabuList.at(i))){
                    neighbours.erase(neighbours.begin() + j);
                    break;
                }else{
                    j++;
                }
            }
        }
//        //Dev Stuff - writes neighbours list
//        for (int i = 0; i < neighbours.size(); ++i) {
//            neighbours.at(i).toStringMask();
//        }
//        std::cout << std::endl;

        //Exit loop because of tabu or find best in neighbours
        if(neighbours.size() <= 0){
            if(best.score() > nextBest.score()){
                best = nextBest;
            }
            std::cout << "Stuck because of tabu." << std::endl;
            break;
        }else{
            //Find best element from new neighbour list
            nextBest = neighbours.at(0);
            for (int i = 0; i < neighbours.size(); i++) {
                if (neighbours.at(i).score() < nextBest.score()){
                    nextBest = neighbours.at(i);
                }
            }
            //Push back last visited element
            tabuList.push_back(nextBest);
        }
    }

//    //Dev Stuff - writes tabu list to console
//    for (int i = 0; i < tabuList.size(); ++i) {
//        tabuList.at(i).toStringMask();
//    }
//    std::cout << std::endl;

    //Write time of execution in console
    time = clock() - time;
    best.setTime(time);
    best.setTimeInSeconds(time/CLOCKS_PER_SEC);
    return best;
}

Solution simulatedAnnealing(Solution problem, int iterations = 100, auto f = [](){return 0.1;}){
    clock_t time = clock();
    std::random_device rd;
    std::mt19937 random_mt19973(rd());
    Solution best = problem, nextSolution = problem;

    for (int i = 0; i < iterations; i++) {
        Solution newSolution = nextSolution.generateNeighbour();
        if(nextSolution.score() > newSolution.score()){
            nextSolution = newSolution;
        }else{
            std::uniform_real_distribution<double> distribution(0,1);
            double anneal = exp(-abs(nextSolution.score() - best.score()) / (f()/i));
//            std::cout << "anneal: " << anneal << std::endl;

            if(distribution(random_mt19973) < anneal){
                nextSolution = newSolution;
            }
        }
        if(nextSolution.score() < best.score()){
            best = nextSolution;
        }
    }

    //Write time of execution in console
    time = clock() - time;
    best.setTime(time);
    best.setTimeInSeconds(time/CLOCKS_PER_SEC);
    return best;
}

//--------------------------------------------------------------------------------------------Genetic Algorithms

Specimen geneticAlgorithm(Population population, int iterations){
    Specimen best = population.population.at(0);

    //UI - selecting selection
    std::cout << "What selection?" << std::endl;
    std::cout << "1.Tournament" << std::endl;
    std::cout << "2.Roulette" << std::endl;
    int selectionType = 0;
    std::cin >> selectionType;
    std::cout << std::endl;

    for (int i = 0; i < iterations; i++) {

        //Searching for best in current population
        for (int j = 0; j < population.population.size(); j++) {
            if(best.fitness() < population.population.at(j).fitness()){
                best = population.population.at(j);
            }
        }

        //Selection select
        std::vector<Specimen> partners;
        if(selectionType == 1){
            partners = population.selectionTournament();
        }
        else if(selectionType == 2){
            partners = population.selectionRoulette();
        }else{
            std::cout << "ABORTING" << std::endl;
            break;
        }

        //Cross specimens in population to get new generation
        std::vector<Specimen> newGeneration = population.cross(partners);

        //Mutate each specimen in new generation
        for(auto e: newGeneration){
            e.mutate();
        }

        //Save new generation
        population.population = newGeneration;
    }

    return best;
}

Specimen geneticAlgorithm(Population population, float averageFitness){
    Specimen best = population.population.at(0);
    float accumulateFitness;

    //UI - selecting selection
    std::cout << "What selection?" << std::endl;
    std::cout << "1.Tournament" << std::endl;
    std::cout << "2.Roulette" << std::endl;
    int selectionType = 0;
    std::cin >> selectionType;
    std::cout << std::endl;

    do{
        //Selection select
        std::vector<Specimen> partners;
        if(selectionType == 1){
            partners = population.selectionTournament();
        }
        else if(selectionType == 2){
            partners = population.selectionRoulette();
        }else{
            std::cout << "ABORTING" << std::endl;
            break;
        }

        //Cross specimens in population to get new generation
        std::vector<Specimen> newGeneration = population.cross(partners);

        //Mutate each specimen in new generation
        for(auto e: newGeneration){
            e.mutate();
        }

        //Save new generation
        population.population = newGeneration;

        //Searching for best in current population
        for (int j = 0; j < population.population.size(); j++) {
            if(best.fitness() < population.population.at(j).fitness()){
                best = population.population.at(j);
            }
        }

        //Reset the accumulateFitness
        accumulateFitness = 0;

        //Calculate average fitness of this population
        for(auto e: population.population){
            accumulateFitness += e.fitness();
        }
        accumulateFitness /= population.population.size();

    }while (accumulateFitness < averageFitness);

    return best;
}

int main( int argc, char** argv )
{
    int option = 0, hillAccuracy = 4;
    std::string fileName = "set_examples.txt";
    if(argc > 1){
        fileName = argv[1];
    }

    //Main loop of program
    do {
        //Scoped variables
        Solution problem;

        std::cout << "What needs to be done?" << std::endl;
        std::cout << "1. Use problem from file." << std::endl;
        std::cout << "2. Generate problem." << std::endl;
        std::cout << "3. Do research data form file." << std::endl;
        std::cout << "0. Exit." << std::endl;
        std::cin >> option;
        std::cout << std::endl;

        //From File
        if (option == 1) {
            std::cout << "What line you want to input?" << std::endl;
            std::cin >> option;
            std::cout << std::endl;
            problem.setAll(fileName, option);
        }
        //Generator
        else if (option == 2) {
            problem.setAll(generateVector());
        }
        //Experiments TODO: I changed location of experiments so update this code !!!
        else if (option == 3) {
            std::string filename;
            std::cout << "Tell me input file name." << std::endl;
            std::cin >> filename;
            std::cout << std::endl;

            std::ifstream file(filename);
            std::ofstream outBrute("brute experiment results.txt");
            std::ofstream outHill("hill experiment results.txt");
            std::ofstream outTabu("tabu experiment results.txt");
            std::ofstream outAnneal("anneal experiment results.txt");


            //to set tabu iterations
            int iterations;
            std::cout << "How many iterations?" << std::endl;
            std::cin >> iterations;
            std::cout << std::endl;

            //format: setsize time score
            std::string line;
            int lineCounter = 0;
            while(std::getline(file, line)) {
                problem.setAll(parseFileLineToVector(line));

                for(int i=0; i<25; i++){
                    Solution brute = bruteForce(problem);
                    outBrute << brute.Set.size() << " " << brute.time << " " << brute.score() << std::endl;
                    Solution hill = hillClimb(problem);
                    outHill << hill.Set.size() << " " << hill.time << " " << hill.score() << std::endl;
                    Solution tab = tabu(problem, iterations);
                    outTabu << tab.Set.size() << " " << tab.time << " " << tab.score() << std::endl;
                    Solution annealing = simulatedAnnealing(problem, iterations, [&](){return (double(iterations)/10*iterations);});
                    outAnneal << annealing.Set.size() << " " << annealing.time << " " << annealing.score() << std::endl;

                    std::cout << "I just did " << i << " iteration of " << lineCounter << " line" << std::endl;
                }
                std::cout << "I just did " << ++lineCounter << " line" << std::endl;
            }
            file.close();
            outBrute.close();
            outHill.close();
            outTabu.close();
            outAnneal.close();
            break;
        }
        //DEV TESTING
        else if(option == 666){
            problem.setAll(generateVector());
            problem.toString();
            Specimen specimen(problem);

            Population population(problem, 2);

            for(int i=0; i<population.population.size(); i++){
                population.population.at(i).solution.toStringMask();
            }

            std::vector<Specimen> newGen = population.cross(population.population, 6);

            for(int i=0; i<newGen.size(); i++){
                newGen.at(i).solution.toStringMask();
            }

            break;
        }
        else{
            break;
        }

        //Algorithm part
        do {
            std::cout << "What type of algorithm to use?" << std::endl;
            std::cout << "1.Brute force." << std::endl;
            std::cout << "2.Hill climb" << std::endl;
            std::cout << "3.Tabu" << std::endl;
            std::cout << "4.Simulated Annealing" << std::endl;
            std::cout << "5.Genetic Algorithms" << std::endl;
            std::cout << "6.All" << std::endl;
            std::cout << "7.All without brute force" << std::endl;
            std::cin >> option;
            std::cout << std::endl;

            problem.toString();

                //FORCE
            if (option == 1) {
                Solution brute = bruteForce(problem);
                brute.toString();
                break;
            }
                //HIll
            else if (option == 2) {
                Solution hill = hillClimb(problem);
                hill.toString();
                break;
            }
                //TABU
            else if (option == 3) {
                int iterations;
                std::cout << "How many iterations?" << std::endl;
                std::cin >> iterations;
                std::cout << std::endl;
                Solution tab = tabu(problem, iterations);
                tab.toString();
                break;
            }
                //ANNEALING
            else if(option == 4){
                int iterations;
                std::cout << "How many iterations?" << std::endl;
                std::cin >> iterations;
                Solution annealing = simulatedAnnealing(problem, iterations, [&](){return (double(iterations)/10)*iterations;});
                annealing.toString();
                break;
            }
                //GA TODO: make it usable in ALL and without brute ;)
            else if(option == 5){
                Population population(problem, 10);
                //UI
                std::cout << "Do you want iterations or average fitness method?" << std::endl;
                std::cout << "1.Iterations" << std::endl;
                std::cout << "2.Average fitness" << std::endl;
                std::cin >> option;
                std::cout << std::endl;

                //Selections select
                Specimen best;
                if(option == 1){
                    //Ask for iterations
                    std::cout << "How many iterations?" << std::endl;
                    int iterations=0;
                    std::cin >> iterations;
                    std::cout << std::endl;

                    best = geneticAlgorithm(population, iterations);
                }
                else if(option == 2){
                    best = geneticAlgorithm(population, 0.9f);
                }else{
                    std::cout << "ABORTING" << std::endl;
                    break;
                }

                best.solution.toString();
                break;
            }
                //ALL
            else if (option == 6) {
                std::cout << "BRUTE FORCE:" << std::endl;
                Solution brute = bruteForce(problem);
                brute.toString();

                std::cout << "HILL CLIMB:" << std::endl;
                Solution hill = hillClimb(problem);
                hill.toString();

                int iterations;
                std::cout << "How many iterations?" << std::endl;
                std::cin >> iterations;
                std::cout << std::endl;

                std::cout << "TABU:" << std::endl;
                Solution tab = tabu(problem, iterations);
                tab.toString();

                std::cout << "SIMULATED ANNEAL:" << std::endl;
                Solution annealing = simulatedAnnealing(problem, iterations, [&](){return (double(iterations)/10)*iterations;});
                annealing.toString();
                break;
            }
                //ALL without Brute Force
            else if (option == 7) {
                std::cout << "HILL CLIMB:" << std::endl;
                Solution hill = hillClimb(problem);
                hill.toString();

                int iterations;
                std::cout << "How many iterations?" << std::endl;
                std::cin >> iterations;
                std::cout << std::endl;

                std::cout << "TABU:" << std::endl;
                Solution tab = tabu(problem, iterations);
                tab.toString();

                std::cout << "SIMULATED ANNEAL:" << std::endl;
                Solution annealing = simulatedAnnealing(problem, iterations, [&](){return (double(iterations)/10)*iterations;});
                annealing.toString();
                break;
            } else {
                std::cout << "That's not an option" << std::endl;
                std::cout << std::endl;
            }
        } while (true);
    } while(true);





    return 0;
}
