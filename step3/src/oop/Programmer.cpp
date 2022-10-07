#include "Programmer.hpp"

namespace job {

// Member functions
void Programmer::DoWork() {
    std::cout << "My work involves - " + Description() << std::endl;
}

}// End namespace job
