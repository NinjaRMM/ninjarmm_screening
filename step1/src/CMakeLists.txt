set(_STEP1 "${PROJECT_NAME}Step1")

add_executable(${_STEP1} main.cpp)
install(
  TARGETS
    ${_STEP1}
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1
)

add_executable("VirtualTable" VirtualTable.cpp)
add_executable("VariadicTemplateA" VariadicTemplateA.cpp)
add_executable("VariadicTemplateB" VariadicTemplateB.cpp)

install(
  TARGETS
        VirtualTable
        VariadicTemplateA
        VariadicTemplateB
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/step1-free-play
)

install(
  FILES
    run-me.sh
  PERMISSIONS
    OWNER_EXECUTE OWNER_WRITE OWNER_READ
  DESTINATION 
    ${CMAKE_INSTALL_PREFIX}/step1-free-play
)
