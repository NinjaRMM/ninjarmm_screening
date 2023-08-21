set(_STEP3 "${PROJECT_NAME}Step3")

add_executable(${_STEP3} CodeChallenge.cpp)

install(
  TARGETS
    ${_STEP3}
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step3
)
