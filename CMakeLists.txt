cmake_minimum_required(VERSION 3.25)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES
        "${CMAKE_SOURCE_DIR}/cmake/conan_provider.cmake"
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(VERSION_SRC cg_version.h.in)
set(VERSION_DST cg_version.h)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include(${CMAKE_SOURCE_DIR}/cmake/GenerateVersion.cmake)
project(cellulitegui
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
        DESCRIPTION "Cellulite GUI"
        LANGUAGES CXX
)

find_package(libmodbus REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        libmodbus::libmodbus)

install(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

