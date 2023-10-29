set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/step1/job.hpp"
    "${PROJECT_SOURCE_DIR}/include/step1/pilot.hpp"
    "${PROJECT_SOURCE_DIR}/include/step1/programmer.hpp"
    "${PROJECT_SOURCE_DIR}/include/step1/utils.hpp")

set(SOURCE_LIST
    "job.cpp"
    "pilot.cpp"
    "programmer.cpp"
    "utils.cpp"
)

add_library(step1 ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(step1 PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_compile_features(step1 PUBLIC cxx_std_20)
