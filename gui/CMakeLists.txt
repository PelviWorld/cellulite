project(gui
        LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        src/celluliteapp.cpp
        src/celluliteframe.cpp
        src/startframe.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND SOURCES src/windowsutility.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND SOURCES src/linuxutility.cpp)
endif ()

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(inih REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${wxWidgets_LIBRARIES}
        PRIVATE
        inih::inih
        laingcontroller::laingcontroller
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../controller/lainginterface/>
        $<INSTALL_INTERFACE:include>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/res/cellulite.ini
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cellulite.ini
)

include(GNUInstallDirs)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/cellulite.ini DESTINATION ${CMAKE_INSTALL_BINDIR})