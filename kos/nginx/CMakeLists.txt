cmake_minimum_required(VERSION 3.14)

project(nginx LANGUAGES C CXX)

if (KOS)
	# Initialize CMake library for KasperskyOS SDK.
	include (platform)
	initialize_platform (FORCE_STATIC)

    include (platform/nk)

	find_package (vfs REQUIRED)
    include_directories (${vfs_INCLUDE})

    # Attach a package with VFS entities implementation
    find_package (precompiled_vfs REQUIRED)
    include_directories (${precompiled_vfs_INCLUDE})
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpointer-arith -Wno-unused-parameter -Werror")

get_filename_component(NGINX_SRC_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

set(TARGET_NAME Nginx)

set(TARGET_EDL nginx_nginx)

set(TARGET_SRC
    ${NGINX_SRC_ROOT}/src/core/nginx.c
    ${NGINX_SRC_ROOT}/src/core/ngx_log.c
    ${NGINX_SRC_ROOT}/src/core/ngx_palloc.c
    ${NGINX_SRC_ROOT}/src/core/ngx_array.c
    ${NGINX_SRC_ROOT}/src/core/ngx_list.c
    ${NGINX_SRC_ROOT}/src/core/ngx_hash.c
    ${NGINX_SRC_ROOT}/src/core/ngx_buf.c
    ${NGINX_SRC_ROOT}/src/core/ngx_queue.c
    ${NGINX_SRC_ROOT}/src/core/ngx_output_chain.c
    ${NGINX_SRC_ROOT}/src/core/ngx_string.c
    ${NGINX_SRC_ROOT}/src/core/ngx_parse.c
    ${NGINX_SRC_ROOT}/src/core/ngx_parse_time.c
    ${NGINX_SRC_ROOT}/src/core/ngx_inet.c
    ${NGINX_SRC_ROOT}/src/core/ngx_file.c
    ${NGINX_SRC_ROOT}/src/core/ngx_crc32.c
    ${NGINX_SRC_ROOT}/src/core/ngx_murmurhash.c
    ${NGINX_SRC_ROOT}/src/core/ngx_md5.c
    ${NGINX_SRC_ROOT}/src/core/ngx_sha1.c
    ${NGINX_SRC_ROOT}/src/core/ngx_rbtree.c
    ${NGINX_SRC_ROOT}/src/core/ngx_radix_tree.c
	${NGINX_SRC_ROOT}/src/core/ngx_regex.c
    ${NGINX_SRC_ROOT}/src/core/ngx_slab.c
    ${NGINX_SRC_ROOT}/src/core/ngx_times.c
    ${NGINX_SRC_ROOT}/src/core/ngx_shmtx.c
    ${NGINX_SRC_ROOT}/src/core/ngx_connection.c
    ${NGINX_SRC_ROOT}/src/core/ngx_cycle.c
    ${NGINX_SRC_ROOT}/src/core/ngx_spinlock.c
    ${NGINX_SRC_ROOT}/src/core/ngx_rwlock.c
    ${NGINX_SRC_ROOT}/src/core/ngx_cpuinfo.c
    ${NGINX_SRC_ROOT}/src/core/ngx_conf_file.c
    ${NGINX_SRC_ROOT}/src/core/ngx_module.c
    ${NGINX_SRC_ROOT}/src/core/ngx_resolver.c
    ${NGINX_SRC_ROOT}/src/core/ngx_open_file_cache.c
    ${NGINX_SRC_ROOT}/src/core/ngx_crypt.c
    ${NGINX_SRC_ROOT}/src/core/ngx_proxy_protocol.c
    ${NGINX_SRC_ROOT}/src/core/ngx_syslog.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event_timer.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event_posted.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event_accept.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event_udp.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event_connect.c
    ${NGINX_SRC_ROOT}/src/event/ngx_event_pipe.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_time.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_errno.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_alloc.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_files.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_socket.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_recv.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_readv_chain.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_udp_recv.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_send.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_writev_chain.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_udp_send.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_udp_sendmsg_chain.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_channel.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_shmem.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_process.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_daemon.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_setaffinity.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_setproctitle.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_posix_init.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_user.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_process_cycle.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_linux_init.c
    ${NGINX_SRC_ROOT}/src/os/unix/ngx_linux_sendfile_chain.c
	${NGINX_SRC_ROOT}/src/event/modules/ngx_poll_module.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_core_module.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_special_response.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_request.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_parse.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_log_module.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_request_body.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_variables.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_script.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_upstream.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_upstream_round_robin.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_write_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_header_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_chunked_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_range_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_gzip_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_postpone_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_ssi_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_charset_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_userid_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_headers_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/ngx_http_copy_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_not_modified_filter_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_static_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_autoindex_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_index_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_mirror_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_try_files_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_auth_basic_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_access_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_limit_conn_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_limit_req_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_geo_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_map_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_split_clients_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_referer_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_proxy_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_fastcgi_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_uwsgi_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_scgi_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_memcached_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_empty_gif_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_browser_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_upstream_hash_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_upstream_ip_hash_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_upstream_least_conn_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_upstream_random_module.c
    ${NGINX_SRC_ROOT}/src/http/modules/ngx_http_upstream_keepalive_module.c
	${NGINX_SRC_ROOT}/src/http/modules/ngx_http_rewrite_module.c
    ${NGINX_SRC_ROOT}/objs/ngx_modules.c
)

set(TARGET_LIBS pthread z pcre)
if (NOT KOS)
	list(APPEND TARGET_LIBS crypt)
endif ()

if (KOS)
	# Setting compilation flags.
	project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

	nk_build_edl_files (${TARGET_EDL}
	        NK_MODULE ${PROJECT_NAME}
	        EDL ${TARGET_NAME}.edl)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

	add_executable(${TARGET_NAME} ${TARGET_SRC} ${EDL_FILES})

	add_dependencies(${TARGET_NAME} ${TARGET_EDL})

	target_link_libraries(${TARGET_NAME} ${TARGET_LIBS} ${vfs_CLIENT_LIB})

	# We do not need default VFS entity here, which comes from ${vfs_CLIENT_LIB}
	set_target_properties (${TARGET_NAME} PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
else ()
	add_executable(${TARGET_NAME} ${TARGET_SRC})

	if (TARGET_LIBS)
	    target_link_libraries(${TARGET_NAME} ${TARGET_LIBS})
	endif ()
endif ()

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
	    ${NGINX_SRC_ROOT}/src/core
	    ${NGINX_SRC_ROOT}/src/event
	    ${NGINX_SRC_ROOT}/src/event/modules
	    ${NGINX_SRC_ROOT}/src/os/unix
	    ${NGINX_SRC_ROOT}/src/os/kos
	    ${NGINX_SRC_ROOT}/objs
	    ${NGINX_SRC_ROOT}/src/http
	    ${NGINX_SRC_ROOT}/src/http/modules
)

install(TARGETS ${TARGET_NAME})

if(NOT TARGET uninstall)
	configure_file(
			"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
			"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
			IMMEDIATE @ONLY)

	add_custom_target(uninstall
			COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

#
# Package configure file
#
include(CMakePackageConfigHelpers)

set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)

configure_package_config_file(nginx-config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/nginx-config.cmake
	INSTALL_DESTINATION ${LIB_INSTALL_DIR}/nginx/cmake
	PATH_VARS BIN_INSTALL_DIR
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/nginx-config-version.cmake
	VERSION 1.0.0
	COMPATIBILITY SameMajorVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/nginx-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/nginx-config-version.cmake
	DESTINATION ${LIB_INSTALL_DIR}/cmake/nginx
)
