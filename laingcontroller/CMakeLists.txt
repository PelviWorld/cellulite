project(laingcontroller
        LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
        src/movetecmodbus.cpp
        include/movetecmodbus.h
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(libmodbus REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        libmodbus::libmodbus
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${libmodbus_INCLUDE_DIRS}
)
