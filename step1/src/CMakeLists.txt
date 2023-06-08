set(BINARY ${SUBPROJECT})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.hpp *.cpp)

set(SOURCES ${SOURCES})

# Create library for unit testing 
add_library(${SUBPROJECT}_lib STATIC ${SOURCES})
# Create executable
add_executable(${SUBPROJECT} ${SOURCES})


