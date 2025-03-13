cmake_minimum_required(VERSION 3.25)

project (nginx_example)

# Add functions for installing artefacts.
include (platform/install)

# Platform definitions.
include (platform/options)

# Initialize CMake library for KasperskyOS SDK.
include (platform)
initialize_platform (FORCE_STATIC)

include (platform/nk)

# Detect board.
if (CMAKE_SYSTEM_SOC STREQUAL "BCM2711")
    set (BOARD "RPI4_BCM2711")
elseif (CMAKE_SYSTEM_SOC STREQUAL "RK3568")
    set (BOARD "RADXA_ROCK_3A")
else ()
    message (FATAL_ERROR "Unsupported board for SoC '${CMAKE_SYSTEM_SOC}'")
endif ()

find_package (nginx REQUIRED)

# Add a package for working with the virtual file system.
find_package (vfs REQUIRED)
include_directories (${vfs_INCLUDE})

# Add a package with the Dhcpcd program implementation.
find_package (rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories (${rump_INCLUDE})

# Add a package with VFS program implementations.
find_package (precompiled_vfs REQUIRED)
include_directories (${precompiled_vfs_INCLUDE})

# Choose the storage devices for the configuration files.
# If none of the options are selected, the ramfs device is used.
option (ROOTFS_SDCARD   "Use SD card to get configuration file" OFF)

add_subdirectory (netinit)
add_subdirectory (einit)
