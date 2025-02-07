project(gyrosensor
        LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
list(APPEND SOURCES 
        src/windowsgyrocom.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
list(APPEND SOURCES 
        src/windowsgyrocom.cpp) # TODO: Change to linuxgyrocom.cpp when implemented
endif ()

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        INTERFACE
        $<INSTALL_INTERFACE:include>
)
