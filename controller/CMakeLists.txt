add_subdirectory(lainginterface)
add_subdirectory(movetecmodbus)
add_subdirectory(laingcontroller)
add_subdirectory(lmccontroller)
add_subdirectory(ltccontroller)

add_custom_target(build_all_libraries
        DEPENDS
        lainginterface
        movetecmodbus
        laingcontroller
        lmccontroller
        ltccontroller
)

add_dependencies(cellulitegui build_all_libraries)
