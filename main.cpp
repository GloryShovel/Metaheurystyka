#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <string.h>
#include <algorithm>
#include <fstream>

class Solution{
public:
    std::vector<int> Set;       //is the beginning set
    std::vector<int> Mask;	    //is mask for set to generate solutions

    //-------------------------------------------------------------------------------------- Constructors
    //From file ( NOTE: If you put to high lineNumber it will create object with last input from file)
    Solution(std::string name, int lineNumber){
        this->setAll(name, lineNumber);
    }

    //For existing set of numbers
    Solution(std::vector<int> setTab){
        this->setAll(setTab);
    }

    Solution(){

    }
    //-------------------------------------------------------------------------------------- Getters
    //Returns value of subset (0 or 1)
    int getResultOfSubset(int subSetID){
        int result = 0;
        for(int i = 0; i < this->Set.size(); i++){
            if(this->Mask[i] == subSetID){
                result += this->Set[i];
            }
        }
        return result;
    }
    //-------------------------------------------------------------------------------------- Setters
    void setAll(std::string name, int lineNumber){
        std::ifstream file(name);
        if(file.is_open()){
            //Reads proper line
            std::string line;
            for(int i = 0; i <=lineNumber; i++){
                std::getline(file, line);
            }

            //Converts line to vector and assign it to Set
            std::string buffer;
            for (int i = 0; i < line.size(); i++) {
                if(line.at(i) == ','){
                    //Pushes buffer to Set as int and cleans buffer
                    Set.push_back(std::stoi(buffer));
                    buffer.clear();
                }else{
                    buffer.push_back(line.at(i));
                }
            }
            //Almost forgot to push back buffer with last value ;)
            Set.push_back(std::stoi(buffer));
        }else{
            std::cout << "Error when tried to open the file" << std::endl;
        }
        file.close();
        this->generateSolution();
    }

    void setAll(std::vector<int> setTab){
        Set = setTab;
        this->generateSolution();
    }

    //-------------------------------------------------------------------------------------- Stringify
    void toStringSet(){
        std::cout << "Set:  {";
        for(int i=0; i < this->Set.size()-1; i++){
            std::cout << std::to_string(this->Set.at(i)) + ",";
        }
        std::cout << std::to_string(this->Set.at(this->Set.size()-1)) +"}" << std::endl;

    }

    void toStringMask(){
        std::cout << "Mask: {";
        for(int i=0; i < this->Mask.size()-1; i++){
            std::cout << std::to_string(this->Mask.at(i)) + ",";
        }
        std::cout << std::to_string(this->Mask.at(this->Mask.size()-1)) +"}" << std::endl;
    }

    void toStringScore(){
        std::cout << "Score: " + std::to_string(this->score()) << std::endl;
    }

    //All info about Object
    void toString(){

        this->toStringSet();
        this->toStringMask();

        std::cout << "Result of first subset: " + std::to_string(this->getResultOfSubset(0)) << std::endl;
        std::cout << "Result of second subset: " + std::to_string(this->getResultOfSubset(1)) << std::endl;
        std::cout << "Score: " + std::to_string(this->score()) << std::endl << std::endl;
    }

    //-------------------------------------------------------------------------------------- Methods
    //Generates first mask
    void generateSolution(){
        std::random_device rd;
        std::uniform_int_distribution<int> dis(0,1);

        bool anyZero = false;
        bool anyOne = false;

        //Generate new mask (have to prevent form setting everything to 0 or 1, because it means only one set)
        for (int i = 0; i < Set.size()-1; i++) {
            int temp = dis(rd);
            if(temp){
                anyOne = true;
            }else{
                anyZero = true;
            }
            this->Mask.push_back(temp);
        }
        if(anyZero && anyOne){
            this->Mask.push_back(dis(rd));
        }else{
            if(anyZero){
                this->Mask.push_back(1);
            }else{
                this->Mask.push_back(0);
            }
        }
    }

    std::vector<int> incrementVector(std::vector<int> vector, int position){
        if(vector.at(position) == 0){
            vector.at(position) = 1;
            return vector;
        }else{
            vector.at(position) = 0;
            if(position != 0){
                vector = incrementVector(vector, position-1);
            }
            return vector;
        }
    }
    std::vector<int> decrementVector(std::vector<int> vector, int position){
        if(vector.at(position) == 1){
            vector.at(position) = 0;
            return vector;
        }else{
            vector.at(position) = 1;
            if(position != 0){
                vector = decrementVector(vector, position-1);
            }
            return vector;
        }
    }
    //if vector is unwanted set it to {0,0,...,0,1} or {1,1,...,1,0}
    void checkForUnwantedMask(Solution s){
        int atFirst = s.Mask.at(0);
        bool isUnwantedVector = false;
        for (int i = 1; i < s.Mask.size(); i++) {
            if(s.Mask.at(i) != atFirst){
                isUnwantedVector = false;
                break;
            }else{
                isUnwantedVector = true;
            }
        }
        //Handle unwanted vector
        if (isUnwantedVector) {
            std::vector<int> temp;
            if (s.Mask.at(0) == 1) {
                for (int i = 0; i < s.Mask.size() - 1; i++) {
                    temp.push_back(0);
                }
                temp.push_back(1);
            } else {
                for (int i = 0; i < s.Mask.size() - 1; i++) {
                    temp.push_back(1);
                }
                temp.push_back(0);
            }
            s.Mask = temp;
            //Dev stuff
//            std::cout << temp.at(0) << "..." << temp.at(temp.size()-1) << std::endl;
        }
    }
    //Generate object with new solution
    Solution generateNextSolution(bool incrementing){
        Solution s = *this;
        if(incrementing){
            s.Mask = incrementVector(s.Mask, s.Mask.size()-1);
        }else{
            s.Mask = decrementVector(s.Mask, s.Mask.size()-1);
        }
        checkForUnwantedMask(s);
        return s;
    }

    //Generate list of neighbouring solutions (amount is equal to neighbours on each side). On left side solutions are reversed
    std::vector<Solution> neighbourList(int amount){
        std::vector<Solution> result;
        for (int i = 0; i < amount; i++) {
            result.push_back(generateNextSolution(false));
        }
        for (int i = 0; i < amount; i++) {
            result.push_back(generateNextSolution(true));
        }
        return result;
    }

    //Goal function
    int score(){
        return std::abs(getResultOfSubset(0) - getResultOfSubset(1));
    }
};

//------------------------------------------------------------------------------------------------- Functions
std::vector<int> generateVector(int difficulty){
    std::random_device rd;
    std::uniform_int_distribution<int> distribute(0,1000);
    int mode = 0;
    switch (difficulty) {
        case 0: mode = 10;
            break;
        case 1: mode = 20;
            break;
        case 2: mode = 23;
            break;
        default: std::cout << "Yo its out of range difficulty. Don't try to burn your machine XD" << std::endl;
            break;
    }
    std::vector<int> generatedExample;
    for(int i=0; i < mode; i++){
        generatedExample.push_back(distribute(rd));
    }
    return generatedExample;
}

Solution bruteForce(Solution solution){
    Solution next = solution, best = solution;
    do{
       next = next.generateNextSolution(true);
       if(next.score() < best.score()){
           best = next;
       }
       //TODO:  save this to make plot out of it
//       std::cout << next.score() << std::endl;
    } while (next.Mask != solution.Mask);
    return best;
}

Solution hillClimb(Solution solution, int accuracy){
    Solution best, nextBest = solution;
    do{
        best = nextBest;
        std::vector<Solution> neighbours = nextBest.neighbourList(accuracy);
        for (int i = 0; i < neighbours.size(); i++) {
            if(neighbours.at(i).score() < nextBest.score()){
                nextBest = neighbours.at(i);
            }
        }
    } while(best.score() != nextBest.score());

    return best;
}

int main( int argc, char** argv )
{
    int option = 0, hillAccuracy = 4;
    std::string fileName = "set_examples.txt";
    if(argc > 1){
        fileName = argv[1];
    }

    do {
        //Scoped variables
        Solution problem;

        std::cout << "What needs to be done?" << std::endl;
        std::cout << "1. Use problem from file." << std::endl;
        std::cout << "2. Generate problem." << std::endl;
        std::cout << "0. Exit." << std::endl;
        std::cin >> option;
        std::cout << std::endl;

        if (option == 1) {
            std::cout << "What line you want to input?" << std::endl;
            std::cin >> option;
            std::cout << std::endl;
            problem.setAll(fileName, option);
        } else if (option == 2) {
            std::cout << "What difficulty? (0,1,2)" << std::endl;
            std::cin >> option;
            std::cout << std::endl;
            problem.setAll(generateVector(option));
        } else {
            break;
        }

        do {
            std::cout << "What type of algorithm to use?" << std::endl;
            std::cout << "1.Brute force." << std::endl;
            std::cout << "2.Hill climb" << std::endl;
            std::cout << "3.All" << std::endl;
            std::cout << "4.All without brute force" << std::endl;
            std::cin >> option;
            std::cout << std::endl;

            problem.toString();

            if (option == 1) {
                Solution brute = bruteForce(problem);
                brute.toString();
                break;
            } else if (option == 2) {
                Solution hill = hillClimb(problem, hillAccuracy);
                hill.toString();
                break;
            } else if (option == 3) {
                std::cout << "BRUTE FORCE:" << std::endl;
                Solution brute = bruteForce(problem);
                brute.toString();
                std::cout << "HILL CLIMB:" << std::endl;
                Solution hill = hillClimb(problem, hillAccuracy);
                hill.toString();
                break;
            } else if (option == 4) {
                std::cout << "HILL CLIMB:" << std::endl;
                Solution hill = hillClimb(problem, hillAccuracy);
                hill.toString();
                break;
            } else {
                std::cout << "That's not an option" << std::endl;
                std::cout << std::endl;
            }
        } while (true);

    } while(true);





    return 0;
}
