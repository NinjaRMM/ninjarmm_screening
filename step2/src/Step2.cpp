#include <iostream>
#include <vector>
#include <functional>

using namespace std;
//<<<<<<<<<<<<<<<<<< INHERITANCE <<<<<<<<<<<<<<<<
class Job {
public:
    virtual std::string getName() = 0;
    virtual std::string getDesctiption() = 0;
    virtual int getNumOfHours() = 0;
    virtual void doWork() = 0;
};

class Programmer : public Job {
public:
    std::string getName() override { return "Programmer"; };
    std::string getDesctiption() override { return "being a good software engineer at NinjaOne"; };
    int getNumOfHours() override { return 40; };
    void doWork() override { std::cout << "My work involves " << getDesctiption() << "\n" << std::endl; };
};

class Pilot : public Job {
public:   
    std::string getName() override { return "Pilot"; };
    std::string getDesctiption() override { return "taking people to the closest island on my aircraft"; };
    int getNumOfHours() override { return 25; };
    void doWork() override { std::cout << "My work involves " << getDesctiption()<< std::endl; };
};

// <<<<<<<<<<<<<<<<<<< Template function >>>>>>>>>>>>>
template<typename T>
void IsInBouds(T httpResonse, int min, int max) {
    if (httpResonse > min && httpResonse < max) {
        std::cout << httpResonse << " is in range [" << min << ", " << max << "]" << std::endl;
    }
    else
        std::cout << httpResonse << " is NOT in range [" << min << ", " << max << "]" << std::endl;
}

// <<<<<<<<<<<<<<<<<<< Lambda Function >>>>>>>>>>>>>
void ContainsTheString(const std::function<bool(std::string)>& func, std::vector<std::string> stringVector  ) {
    int cont = 0;
    for (int i = 0; i < stringVector.size(); i++) {
        if (func(stringVector[i])) {
            cont++;
        }
    }
    std::cout << "String found " << cont << " times" << std::endl;
}


int main()
{
    //<<<<<<<<<<<< inheritance >>>>>>>>>>>>>
    Programmer* programmer1 = new Programmer();    
    Pilot* pilot1 = new Pilot();

    std::vector<Job*> collection= { programmer1, pilot1 };
    std::cout << "\n                               <<<<<<<<<<<< INHERITANCE >>>>>>>>>>>>\n" << std::endl;
    for (int i = 0; i < collection.size(); i++) {
        std::cout << "Name: "<< collection[i]->getName() << std::endl;
        std::cout << "Job Description : "<< collection[i]->getDesctiption() << std::endl;
        std::cout << "Number of hours: "<< collection[i]->getNumOfHours() << std::endl;
        collection[i]->doWork();
    }

    //<<<<<<<<<<<< template function >>>>>>>>>>>>>
    std::cout << "\n                             <<<<<<<<<<<< TEMPLATE FUNCTION >>>>>>>>>>\n" << std::endl;
    const uint32_t httpResonseCode = 528;
    IsInBouds<uint32_t>(httpResonseCode, 500, 599);    

    //<<<<<<<<<<<< lambda function >>>>>>>>>>>>>
    std::cout << "\n                            <<<<<<<<<<<< LAMBDA FUNCTION >>>>>>>>>>>>>\n" << std::endl;
    std::vector<std::string> theStrings = { "one", "two", "three", "test" };
    auto lambda = [](const std::string& tested) { return tested == "test"; };
    ContainsTheString(lambda, theStrings);

}

