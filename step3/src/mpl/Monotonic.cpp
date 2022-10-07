// Application Headers
#include "Monotonic.hpp"

namespace pmr {

void Allocation()
{
    // Pre-allocate 512 bytes on the stack
    std::array<std::byte, 512> buffer{};

    // Memory is release ONLY after 'pool' goes out of scope
    std::pmr::monotonic_buffer_resource pool(buffer.data(), buffer.size());

    // Creates 'pmr::vector' of strings using the pool
    std::pmr::vector<std::pmr::string> v{&pool};

    v.push_back("Hello");
    v.push_back("World");
    v.push_back("!");

    // Iterate over the vector and display its elements
    std::for_each(v.begin(), v.end(), [](const auto& s) {std::cout << s << " ";});
}

}// End of namespace pmr
