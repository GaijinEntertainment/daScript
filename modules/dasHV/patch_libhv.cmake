cmake_minimum_required(VERSION 3.12)
# Apply the libhv fixes this tree carries on top of the pinned tarball; each one is dropped when the
# pin moves past its upstream merge. The first run saves each file as <file>.das_pristine, and every
# run patches from that copy - so re-running, or editing a hunk here, rebuilds the file from the
# tarball's text. A pristine anchor that is missing fails the build: a pin bump that touched the
# patched code re-decides the patch, it never builds unpatched.
#
#   base/hlog.c - logger_enable_fsync uses the logger mutex shared by all serving threads.
#   base/htime.c - hv_compile_datetime reads a fixed date under HV_REPRODUCIBLE_BUILD, so the build
#                  is byte-reproducible; without it, __DATE__/__TIME__ as before.
#
# Usage: cmake -DLIBHV_SRC_DIR=<libhv source dir> -P patch_libhv.cmake

if(NOT LIBHV_SRC_DIR)
    message(FATAL_ERROR "patch_libhv.cmake: pass -DLIBHV_SRC_DIR=<libhv source dir>")
endif()

function(das_hv_patch_begin file)
    set(path "${LIBHV_SRC_DIR}/${file}")
    if(NOT EXISTS "${path}.das_pristine")
        configure_file("${path}" "${path}.das_pristine" COPYONLY)
    endif()
    get_property(patched GLOBAL PROPERTY das_hv_patched_files)
    if(path IN_LIST patched)
        file(READ "${path}" src)
    else()
        file(READ "${path}.das_pristine" src)
    endif()
    set(das_hv_file "${file}" PARENT_SCOPE)
    set(das_hv_src "${src}" PARENT_SCOPE)
endfunction()

function(das_hv_hunk pristine patched)
    string(FIND "${das_hv_src}" "${pristine}" at)
    if(at EQUAL -1)
        string(REGEX MATCH "^[^\n]*" first_line "${pristine}")
        string(STRIP "${first_line}" first_line)
        message(FATAL_ERROR "patch_libhv.cmake: ${das_hv_file} no longer carries the pristine anchor "
            "'${first_line}' - the libhv pin changed this code; re-decide the patch in modules/dasHV/patch_libhv.cmake")
    endif()
    string(REPLACE "${pristine}" "${patched}" src "${das_hv_src}")
    set(das_hv_src "${src}" PARENT_SCOPE)
endfunction()

function(das_hv_patch_end)
    file(WRITE "${LIBHV_SRC_DIR}/${das_hv_file}" "${das_hv_src}")
    set_property(GLOBAL APPEND PROPERTY das_hv_patched_files "${LIBHV_SRC_DIR}/${das_hv_file}")
    message(STATUS "patch_libhv.cmake: patched ${das_hv_file}")
endfunction()

das_hv_patch_begin("base/hlog.c")
das_hv_hunk([=[
void logger_enable_fsync(logger_t* logger, int on) {
    logger->enable_fsync = on;
}
]=] [=[
void logger_enable_fsync(logger_t* logger, int on) {
    hmutex_lock(&logger->mutex_);
    logger->enable_fsync = on;
    hmutex_unlock(&logger->mutex_);
}
]=])
das_hv_patch_end()

das_hv_patch_begin("base/htime.c")
das_hv_hunk([=[
    sscanf(__DATE__, "%s %d %d", month, &dt.day, &dt.year);
    sscanf(__TIME__, "%d:%d:%d", &dt.hour, &dt.min, &dt.sec);
]=] [=[
#ifdef HV_REPRODUCIBLE_BUILD
    sscanf("Jan 1 1970", "%s %d %d", month, &dt.day, &dt.year);
    sscanf("00:00:00", "%d:%d:%d", &dt.hour, &dt.min, &dt.sec);
#else
    sscanf(__DATE__, "%s %d %d", month, &dt.day, &dt.year);
    sscanf(__TIME__, "%d:%d:%d", &dt.hour, &dt.min, &dt.sec);
#endif
]=])
das_hv_patch_end()

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_limits.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_upgrade.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_tls.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_pipeline.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_logging.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_cookies.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_shutdown.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_date.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/patch_libhv_dns.cmake)
