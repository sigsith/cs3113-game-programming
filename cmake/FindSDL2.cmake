# FindSDL2.cmake
find_path(SDL2_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)

set(SDL2_NAMES ${SDL2_NAMES} SDL2)
find_library(SDL2_LIBRARY NAMES ${SDL2_NAMES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
