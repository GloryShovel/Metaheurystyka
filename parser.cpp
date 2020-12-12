#include "parser.h"

std::vector<int> parseFileLineToVector(std::string line){
    std::vector<int> result;
    std::string buffer;

    for (int i = 0; i < line.size(); i++) {
        if(line.at(i) == ','){
            //Pushes buffer to Set as int and cleans buffer
            result.push_back(std::stoi(buffer));
            buffer.clear();
        }else{
            buffer.push_back(line.at(i));
        }
    }

    //Almost forgot to push back buffer with last value ;)
    result.push_back(std::stoi(buffer));
    return result;
}