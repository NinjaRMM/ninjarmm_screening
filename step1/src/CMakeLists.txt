

if(WIN32)
  add_executable(win32_msi_enum win32_msi_enum.cpp)
  target_link_libraries(win32_msi_enum PRIVATE msi)
  add_executable(win32_sysinfo win32_sysinfo.cpp)
elseif(UNIX)
  add_executable(linux_apt_enum linux_apt_enum.cpp)
  target_link_libraries(linux_apt_enum PRIVATE apt-pkg)
  add_executable(linux_sysinfo linux_sysinfo.cpp)
endif()