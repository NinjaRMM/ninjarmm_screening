set(_STEP1 "${PROJECT_NAME}Step1")

add_executable(${_STEP1} main.cpp)
install(
  TARGETS
    ${_STEP1}
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1
)
