cmake_minimum_required(VERSION 3.10)
project(step1)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

add_executable(step1 Step1.cpp)

add_executable(teststep1 Step1.cpp)


set(Boost_NO_WARN_NEW_VERSIONS ON)
set(BOOST_ROOT "C:\\local\\boost_1_81_0")
set(Boost_LIBRARY_DIR "${BOOST_ROOT}\\stage\\lib")
set(BOOST_MIN_VERSION "1.81.0")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS program_options)
if (Boost_FOUND)
    message(STATUS "Boost found")
    include_directories(${BOOST_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
    link_directories(${BOOST_LIBRARYDIR} ${Boost_LIBRARY_DIRS})
    target_link_libraries(step1 ${Boost_LIBRARIES})
    target_link_libraries(teststep1 ${Boost_LIBRARIES})
    target_compile_definitions(step1 PRIVATE -DHAS_BOOST)
    target_compile_definitions(teststep1 PRIVATE -DHAS_BOOST)
else ()
    message(WARNING "Boost not found")

endif ()

target_compile_definitions(teststep1 PRIVATE -DUNIT_TESTS)
enable_testing()
add_test(NAME teststep1 COMMAND teststep1)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    # Setting variables for Doxygen
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN NO)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif ()

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME step1)
set(CPACK_PACKAGE_VENDOR "Alexandre Horst")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is a small project to screening for a job")
set(CPACK_PACKAGE_VERSION "1.0.0")
include(CPack)
set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
install(TARGETS step1
        RUNTIME DESTINATION bin)