#include "component.h"
#include <cstdio>
#include <cinttypes>

int
main() {
    uint64_t floatId = RegisterComponent<float>();
    uint64_t intId   = RegisterComponent<int>();

    printf("int: %" PRIu64 " (must be 1), float: %" PRIu64 " (must be 0)\n", intId, floatId);

    return 0;
}
