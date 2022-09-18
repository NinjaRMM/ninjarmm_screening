#include "component.h"
#include <cstdio>
#include <cinttypes>

int
main() {
    uint64_t intId   = RegisterComponent<int>();
    uint64_t floatId = RegisterComponent<float>();

    printf("int: %" PRIu64 " (must be 0), float: %" PRIu64 " (must be 1)\n", intId, floatId);

    return 0;
}
