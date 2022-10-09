New-Item -ItemType Directory -Force -Path build\Windows
cd build\Windows
cmake.exe ../.. -DCMAKE_BUILD_TYPE=Debug -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build .
cd ../..
