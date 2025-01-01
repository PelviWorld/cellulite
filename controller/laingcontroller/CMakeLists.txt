project(laingcontroller
        LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
        src/laingcontroller.cpp
        include/laingcontroller.h
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        movetecmodbus::movetecmodbus
        lainginterface::lainginterface
        lmccontroller::lmccontroller
        ltccontroller::ltccontroller
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../lainginterface/>
        $<INSTALL_INTERFACE:include>
)
