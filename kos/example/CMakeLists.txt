cmake_minimum_required(VERSION 3.14)

project (nginx_example)

# Add functions for installing artefacts.
include (platform/install)

# Platform definitions.
include (platform/options)

# Initialize CMake library for KasperskyOS SDK.
include (platform)
initialize_platform()

include (platform/nk)

find_package (nginx REQUIRED)

# Add a package for working with the virtual file system.
find_package (vfs REQUIRED)
include_directories (${vfs_INCLUDE})

# Add a package with the Dhcpcd program implementation.
find_package (rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories (${rump_INCLUDE})

# Add a package with VFS progam implementations.
find_package (precompiled_vfs REQUIRED)
include_directories (${precompiled_vfs_INCLUDE})

# Choose the storage devices for the configuration files.
# If none of the options are selected, the ramfs device is used.
option (ROOTFS_SDCARD   "Use SD card to get configuration file" OFF)

add_subdirectory (netinit)
add_subdirectory (einit)
