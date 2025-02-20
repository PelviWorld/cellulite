project(gui
        LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        src/celluliteapp.cpp
        src/celluliteframe.cpp
        src/controllerframe.cpp
        src/configreader.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(inih REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${wxWidgets_LIBRARIES}
        gyrosensor::gyrosensor
        PRIVATE
        inih::inih
        laingcontroller::laingcontroller
        utility::utility
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