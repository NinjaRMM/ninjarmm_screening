set(_STEP1 "${PROJECT_NAME}Step1")

add_executable(${_STEP1} main.cpp)
install(
  TARGETS
    ${_STEP1}
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1
)

add_executable("VirtualTable" VirtualTable.cpp)

install(
  TARGETS
        VirtualTable
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1-free-play
)
