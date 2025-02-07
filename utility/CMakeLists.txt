project(utility
        LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
list(APPEND SOURCES 
        src/windowsutility.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
list(APPEND SOURCES 
        src/linuxutility.cpp)
endif ()

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(inih REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        inih::inih
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        INTERFACE
        $<INSTALL_INTERFACE:include>
)
