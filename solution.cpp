#include "solution.h"

//--------------------------------------------------------------------------------------------------------- Constructors
//From file ( NOTE: If you put to high lineNumber it will create object with last input from file)
Solution::Solution(std::string name, int lineNumber){
this->setAll(name, lineNumber);
}

//For existing set of numbers
Solution::Solution(std::vector<int> setTab){
this->setAll(setTab);
}

Solution::Solution(){

}
//-------------------------------------------------------------------------------------------------------------- Getters
//Returns value of subset (0 or 1)
int Solution::getResultOfSubset(int subSetID){
    int result = 0;
    for(int i = 0; i < this->Set.size(); i++){
        if(this->Mask[i] == subSetID){
            result += this->Set[i];
        }
    }
    return result;
}
//-------------------------------------------------------------------------------------------------------------- Setters

void Solution::setAll(std::string name, int lineNumber){
    std::ifstream file(name);
    if(file.is_open()){
        //Reads proper line
        std::string line;
        for(int i = 0; i <=lineNumber; i++){
            std::getline(file, line);
        }

        this->Set = parseFileLineToVector(line);
        this->generateSolution();
    }else{
        std::cout << "Error when tried to open the file" << std::endl;
    }
    file.close();
}

void Solution::setAll(std::vector<int> setTab){
    Set = setTab;
    this->generateSolution();
}

void Solution::setTime(int time){
    this->time = time;
}

void Solution::setTimeInSeconds(int time){
    this->timeInSeconds = time;
}
//------------------------------------------------------------------------------------------------------------ Stringify

void Solution::toStringSet(){
    std::cout << "Set:  {";
    for(int i=0; i < this->Set.size()-1; i++){
        std::cout << std::to_string(this->Set.at(i)) + ",";
    }
    std::cout << std::to_string(this->Set.at(this->Set.size()-1)) +"}" << std::endl;

}

void Solution::toStringMask(){
    std::cout << "Mask: {";
    for(int i=0; i < this->Mask.size()-1; i++){
        std::cout << std::to_string(this->Mask.at(i)) + ",";
    }
    std::cout << std::to_string(this->Mask.at(this->Mask.size()-1)) +"}" << std::endl;
}

void Solution::toStringScore(){
    std::cout << "Score: " + std::to_string(this->score()) << std::endl;
}

//All info about Object
void Solution::toString(){

    this->toStringSet();
    this->toStringMask();

    std::cout << "Time of calculating the solution: " << this->time << std::endl;
    std::cout << "Time of calculating the solution: " << this->timeInSeconds << "sec" << std::endl;

    std::cout << "Result of first subset: " + std::to_string(this->getResultOfSubset(0)) << std::endl;
    std::cout << "Result of second subset: " + std::to_string(this->getResultOfSubset(1)) << std::endl;
    std::cout << "Score: " + std::to_string(this->score()) << std::endl << std::endl;
}

//-------------------------------------------------------------------------------------------------------------- Methods
bool Solution::equals(Solution s){
    if(this->Set == s.Set){
        if(this->Mask == s.Mask){
            return true;
        } else{
            return false;
        }
    }else{
        return false;
    }
}

//Generates first mask
void Solution::generateSolution(){
    std::random_device rd;
    std::uniform_int_distribution<int> dis(0,1);

    std::vector<int> generatedMask;

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
        generatedMask.push_back(temp);
    }
    if(anyZero && anyOne){
        generatedMask.push_back(dis(rd));
    }else{
        if(anyZero){
            generatedMask.push_back(1);
        }else{
            generatedMask.push_back(0);
        }
    }

    this->Mask = generatedMask;
}

std::vector<int> Solution::incrementVector(std::vector<int> vector, int position){
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
std::vector<int> Solution::decrementVector(std::vector<int> vector, int position){
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
void Solution::checkForUnwantedMask(Solution s){
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
    //TODO: handle unwanted vector in different place
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

//Generate object with new solution (for brute forcing solution)
Solution Solution::generateIncrementedSolution(bool incrementing){
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
std::vector<Solution> Solution::neighbourList(){
    std::vector<Solution> result;
    for (int i = 0; i < this->Mask.size(); i++) {
        Solution s = *this;
        if(s.Mask.at(i) == 0){
            s.Mask.at(i) = 1;
        }else {
            s.Mask.at(i) = 0;
        }
        //DEV STUFF
//            s.toStringMask();
        result.push_back(s);
    }
    return result;
}

Solution Solution::generateNeighbour(){
    std::random_device rd;
    std::uniform_int_distribution<int> distribute(0, this->Mask.size()-1);
    int randNeighbour = distribute(rd);
    Solution result = this->neighbourList().at(randNeighbour);
    return result;
}

//Goal function
int Solution::score(){
    return std::abs(getResultOfSubset(0) - getResultOfSubset(1));
}