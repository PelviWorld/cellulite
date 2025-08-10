project(gui
        LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        src/celluliteapp.cpp
        src/celluliteframe.cpp
        src/controllerframe.cpp
        src/configreader.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

find_package(inih REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${GTK3_LIBRARIES}
        ${wxWidgets_LIBRARIES}
        gyrosensor::gyrosensor
        PRIVATE
        inih::inih
        laingcontroller::laingcontroller
        utility::utility
)

target_include_directories(${PROJECT_NAME}
    SYSTEM PUBLIC
    ${GTK3_INCLUDE_DIRS}
    ${wxWidgets_INCLUDE_DIRS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../controller/lainginterface/>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    GTK_DISABLE_SINGLE_INCLUDES=1
    GDK_VERSION_MIN_REQUIRED=GDK_VERSION_3_0
    GDK_VERSION_MAX_ALLOWED=GDK_VERSION_3_0
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/res/cellulite.ini
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cellulite.ini
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/res/Trainer.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Trainer.png
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/res/Seat.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Seat.png
)

include(GNUInstallDirs)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/cellulite.ini DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/Trainer.png DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/Seat.png DESTINATION ${CMAKE_INSTALL_BINDIR})
