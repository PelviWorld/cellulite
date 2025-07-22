set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Toolchain Setup
set(TOOLCHAIN_PREFIX aarch64-linux-gnu-)
execute_process(
  COMMAND which ${TOOLCHAIN_PREFIX}gcc
  OUTPUT_VARIABLE BINUTILS_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# PKG_CONFIG für Cross-Compilation
set(ENV{PKG_CONFIG_PATH} "/usr/lib/aarch64-linux-gnu/pkgconfig:/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_LIBDIR} "/usr/lib/aarch64-linux-gnu/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} "/")

# wxWidgets Cache-Variablen VOR find_package setzen
set(wxWidgets_FOUND TRUE CACHE BOOL "wxWidgets found" FORCE)
set(wxWidgets_DIR "/usr/lib/aarch64-linux-gnu/cmake/wxWidgets" CACHE PATH "wxWidgets dir" FORCE)
set(wxWidgets_ROOT_DIR "/usr" CACHE PATH "wxWidgets root" FORCE)
set(wxWidgets_INCLUDE_DIRS "/usr/include/wx-3.2;/usr/lib/aarch64-linux-gnu/wx/include/gtk3-unicode-3.2" CACHE STRING "wxWidgets include dirs" FORCE)
set(wxWidgets_LIBRARY_DIRS "/usr/lib/aarch64-linux-gnu" CACHE STRING "wxWidgets library dirs" FORCE)
set(wxWidgets_LIBRARIES "/usr/lib/aarch64-linux-gnu/libwx_gtk3u_core-3.2.so;/usr/lib/aarch64-linux-gnu/libwx_baseu-3.2.so" CACHE STRING "wxWidgets libraries" FORCE)
set(wxWidgets_USE_UNICODE TRUE CACHE BOOL "wxWidgets unicode" FORCE)
set(wxWidgets_USE_STATIC FALSE CACHE BOOL "wxWidgets static" FORCE)

# Zusätzliche FindwxWidgets-spezifische Variablen
set(wxWidgets_wxrc_EXECUTABLE "/usr/bin/wxrc-3.2" CACHE FILEPATH "wxrc executable" FORCE)
set(wxWidgets_CONFIG_EXECUTABLE "/usr/bin/wx-config" CACHE FILEPATH "wx-config executable" FORCE)

# Library-Suchpfade für Cross-Compilation
set(CMAKE_LIBRARY_PATH "/usr/lib/aarch64-linux-gnu;/lib/aarch64-linux-gnu")
link_directories(/usr/lib/aarch64-linux-gnu)

# Compiler-Flags für wxWidgets und GTK3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/wx-3.2 -I/usr/lib/aarch64-linux-gnu/wx/include/gtk3-unicode-3.2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/gtk-3.0 -I/usr/include/aarch64-linux-gnu/gtk-3.0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/glib-2.0 -I/usr/include/aarch64-linux-gnu/glib-2.0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/lib/aarch64-linux-gnu/glib-2.0/include")

# Sysroot und Find-Pfade
set(CMAKE_SYSROOT /)
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu;/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Rpath Settings
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/lib/aarch64-linux-gnu;/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,/usr/lib/aarch64-linux-gnu")