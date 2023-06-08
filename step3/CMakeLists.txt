set(BINARY ${CMAKE_PROJECT_NAME}_step3)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.hpp *.cpp)

set(SOURCES ${SOURCES})


add_executable(${BINARY} ${SOURCES})


