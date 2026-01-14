cmake_minimum_required(VERSION 3.16)
project(OFPD3DXWrapper VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(WIN32)
    add_definitions(-DWIN32 -D_WINDOWS -DNOMINMAX)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MinHook
)

file(GLOB SOURCES "src/*.cpp")

add_library(OFPD3DXWrapper SHARED ${SOURCES})

target_link_libraries(OFPD3DXWrapper PRIVATE
    d3d9.lib
    d3dx9.lib
)

if(MSVC)
    target_compile_options(OFPD3DXWrapper PRIVATE
        /W4
        /EHsc
        /WX-
    )
    
    set_target_properties(OFPD3DXWrapper PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()

install(TARGETS OFPD3DXWrapper
    RUNTIME DESTINATION bin
)

install(FILES d3dx.ini
    DESTINATION bin
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "")
    message(STATUS "==========================================")
    message(STATUS "OFP D3DX Wrapper Build Configuration")
    message(STATUS "==========================================")
    message(STATUS "")
    message(STATUS "Output: ${CMAKE_CURRENT_BINARY_DIR}/")
    message(STATUS "DLL files will be generated after build")
    message(STATUS "")
    message(STATUS "To build, run:")
    message(STATUS "  cmake --build . --config Release")
    message(STATUS "")
    message(STATUS "==========================================")
endif()
