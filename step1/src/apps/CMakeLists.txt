add_executable(app app.cpp)

target_compile_features(app PRIVATE cxx_std_20)

target_link_libraries(app PRIVATE step1)
