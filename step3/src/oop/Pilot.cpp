#include "Pilot.hpp"

namespace job {

// Member functions
void Pilot::DoWork() {
    std::cout << "My work involves - " + Description() << std::endl;
}

}// End namespace job
