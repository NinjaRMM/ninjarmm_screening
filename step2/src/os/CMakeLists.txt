set(os_head os.hpp)

if(WIN32)
    set(os_src os.windows.cpp)
endif()

if(UNIX)
    if(APPLE)
        set(os_src os.mac.cpp)
    else()
        set(os_src os.linux.cpp)
    endif()
endif()

MESSAGE(STATUS ${os_src})
add_library(os ${os_head} ${os_src})
