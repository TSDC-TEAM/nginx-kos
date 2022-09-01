project (netinit)

set(TARGET_NAME NetInit)

set(TARGET_EDL netinit_netinit)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

nk_build_edl_files (${TARGET_EDL}
		NK_MODULE ${PROJECT_NAME}
		EDL ${TARGET_NAME}.edl)

add_executable (${TARGET_NAME} "main.c")
add_dependencies (${TARGET_NAME} ${TARGET_EDL})
target_link_libraries (${TARGET_NAME} ${vfs_CLIENT_LIB})

# We do not need the default VFS entity which comes from ${vfs_CLIENT_LIB}.
set_target_properties (${TARGET_NAME} PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
