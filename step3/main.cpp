#include <iostream>
#include <vector>

#include <stdint.h> // uint32_t


#include <job.h>
#include <programmer.h>
#include <pilot.h>

// h. ---------------------------------------------------------

template <typename T>
bool IsInBounds(T httpResponseCode, T min, T max) {
    std::cout << "min: " << min << std::endl;
    std::cout << "httpResponseCode: " << httpResponseCode << std::endl;
    std::cout << "max: " << max << std::endl;

    bool result = (min <= httpResponseCode) && (httpResponseCode <= max);

    std::cout << "Result: " << result << std::endl;

    return result;
}

// -------------------------------------------------------------

// i. ----------------------------------------------------------

int ContainsTheString( bool (*func)(const std::string &) , std::vector<std::string> &v ) {
    int c = 0;

    std::vector<std::string>::iterator it;
    for (it = v.begin(); it!= v.end(); it++) {
        if ( func(*it) ) {
            c++;
        }
    }

    return c;
}

// -------------------------------------------------------------

int main() {
    // a. ~ g. -------------------------------------------------
    std::vector<Job*> workers;

    workers.push_back(new Programmer());
    workers.push_back(new Pilot());

    // Iterate
    std::vector<Job*>::iterator it;
    for (it = workers.begin(); it!= workers.end(); it++) {
        std::cout << "Name is: " << ((*it)->Name()) << std::endl;
        std::cout << "Description is: " << ((*it)->Description()) << std::endl;
        std::cout << "Hours is: " << ((*it)->Hours()) << std::endl;
        (*it)->DoWork();
    }

    for (it = workers.begin(); it!= workers.end(); it++) {
        delete *it;
    }
    workers.clear();

    // h. --------------------------
    bool result = IsInBounds<uint32_t>(530, 500, 599);

    std::cout << "IsInbound: ";
    std::cout << (result ? "true" : "false");
    std::cout << std::endl;

    // i. --------------------------
    auto theStrings = std::vector<std::string> { "one", "two", "test" };
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
    std::cout << "count: " << count << std::endl;

    return 0;
}
