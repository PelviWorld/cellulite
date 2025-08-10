cmake_minimum_required(VERSION 3.25)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES
        "${CMAKE_SOURCE_DIR}/cmake/conan_provider.cmake"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VERSION_SRC cg_version.h.in)
set(VERSION_DST cg_version.h)
include(${CMAKE_SOURCE_DIR}/cmake/GenerateVersion.cmake)

project(cellulitegui
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
        DESCRIPTION "Cellulite GUI"
        LANGUAGES CXX C
)

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross compiling for ${CMAKE_SYSTEM_PROCESSOR}")
endif()

if(NOT CMAKE_SYSTEM_PROCESSOR)
    message(WARNING "No target processor set. Use -DCMAKE_SYSTEM_PROCESSOR=aarch64 for ARM64")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME}
        src/main.cpp
)

if(CMAKE_BUILD_TYPE MATCHES "Debug" AND UNIX)
  target_link_options(${PROJECT_NAME}
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
endif()

add_subdirectory(controller)
add_subdirectory(gui)
add_subdirectory(utility)
add_subdirectory(sensors)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        laingcontroller::laingcontroller
        gui::gui
        ${wxWidgets_LIBRARIES}
        ${GTK3_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PRIVATE ${wxWidgets_DEFINITIONS})

install(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(CPACK_PACKAGE_NAME "cellulitegui")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cellulite GUI to set up the axis of the cellulite seat")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")

set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${PROJECT_NAME}")

set(CPACK_PACKAGE_VENDOR "PelviWorld")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_INSTALL_DIRECTORY "CelluliteGUI")

# set(CPACK_GENERATOR NSIS)
# set(CPACK_NSIS_MODIFY_PATH ON)
# set(CPACK_NSIS_PACKAGE_NAME "CelluliteGUI")
set(CPACK_PACKAGE_EXECUTABLES "CelluliteGUI" "CelluliteGUI")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "cellulitegui")
set(CPACK_DEBIAN_FILE_NAME "cellulitegui-${PROJECT_VERSION}.deb")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PelviWorld")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxgtk3.2-1, libgtk-3-0")

include(CPack)
