cmake_minimum_required(VERSION 3.25)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES
        "${CMAKE_SOURCE_DIR}/cmake/conan_provider.cmake"
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VERSION_SRC cg_version.h.in)
set(VERSION_DST cg_version.h)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include(${CMAKE_SOURCE_DIR}/cmake/GenerateVersion.cmake)
project(cellulitegui
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
        DESCRIPTION "Cellulite GUI"
        LANGUAGES CXX C
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} WIN32
        src/main.cpp
)

add_subdirectory(controller)
add_subdirectory(gui)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        laingcontroller::laingcontroller
        gui::gui
)

install(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(CPACK_PACKAGE_NAME "cellulitegui")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cellulite GUI to set up the axis of the cellulite seat")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")

set(CPACK_PACKAGE_VENDOR "PelviWorld")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_INSTALL_DIRECTORY "CelluliteGUI")

set(CPACK_GENERATOR NSIS)
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_PACKAGE_NAME "CelluliteGUI")
set(CPACK_PACKAGE_EXECUTABLES "CelluliteGUI" "CelluliteGUI")

#set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "cellulitegui")
set(CPACK_DEBIAN_FILE_NAME "cellutitegui-${PROJECT_VERSION}.deb")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PelviWorld")

include(CPack)
