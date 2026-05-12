IF ((NOT DAS_LLVM_INCLUDED) AND ((NOT ${DAS_LLVM_DISABLED}) OR (NOT DEFINED DAS_LLVM_DISABLED)))
    SET(DAS_LLVM_INCLUDED TRUE)
    MESSAGE(STATUS "dasLlvm module included.")

	SET(DAS_LLVM_DIR ${PROJECT_SOURCE_DIR}/modules/dasLLVM)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_LLVM_DIR})

    set(DAS_LLVM_LIB_OUTPUT "${PROJECT_SOURCE_DIR}/lib")
    set(DAS_CLANG_OUTPUT "${PROJECT_SOURCE_DIR}/bin")
    set(DAS_LLVM_DLL_NAME LLVM.dll)

    # Cache-invalidating sentinel: ${DAS_LLVM_LIB_OUTPUT}/LLVM.dll.version
    # holds the LLVM target version of the prebuilt currently on disk. Mismatch
    # (or missing file) forces re-discovery / re-download. Without this, bumping
    # the LLVM version in this file does NOT replace a stale lib/LLVM.dll from
    # a prior build.
    SET(DAS_LLVM_TARGET_VERSION "22.1.5")
    SET(DAS_LLVM_REFETCH FALSE)
    IF (NOT EXISTS "${DAS_LLVM_LIB_OUTPUT}/${DAS_LLVM_DLL_NAME}")
        SET(DAS_LLVM_REFETCH TRUE)
    ELSEIF (NOT EXISTS "${DAS_LLVM_LIB_OUTPUT}/LLVM.dll.version")
        SET(DAS_LLVM_REFETCH TRUE)
    ELSE()
        file(READ "${DAS_LLVM_LIB_OUTPUT}/LLVM.dll.version" _das_llvm_cached_ver)
        STRING(STRIP "${_das_llvm_cached_ver}" _das_llvm_cached_ver)
        IF (NOT _das_llvm_cached_ver STREQUAL DAS_LLVM_TARGET_VERSION)
            MESSAGE(STATUS "dasLlvm: cached LLVM ${_das_llvm_cached_ver} != target ${DAS_LLVM_TARGET_VERSION}, refetching")
            SET(DAS_LLVM_REFETCH TRUE)
        ENDIF()
    ENDIF()

    IF (DAS_LLVM_REFETCH)
        find_package(LLVM ${DAS_LLVM_TARGET_VERSION})
        IF (LLVM_FOUND)
            MESSAGE(STATUS "LLVM found at: ${LLVM_INSTALL_PREFIX}")
            IF(WIN32)
                # Copy clang-cl to use it as a linker
                FILE(COPY
                    "${LLVM_INSTALL_PREFIX}/bin/clang-cl.exe" DESTINATION
                    "${DAS_CLANG_OUTPUT}/clang-cl.exe"
                )
                set(DAS_LLVM_DLL_PATH ${LLVM_INSTALL_PREFIX}/bin/LLVM-C.dll)
            ELSEIF(APPLE)
                set(DAS_LLVM_DLL_PATH ${LLVM_INSTALL_PREFIX}/lib/libLLVM.dylib)
            ELSE()
                set(DAS_LLVM_DLL_PATH ${LLVM_INSTALL_PREFIX}/lib/libLLVM.so )
            ENDIF()
            get_filename_component(dll_name "${dll}" NAME)
            configure_file("${DAS_LLVM_DLL_PATH}"
                "${DAS_LLVM_LIB_OUTPUT}/${DAS_LLVM_DLL_NAME}"
                COPYONLY
            )
            file(WRITE "${DAS_LLVM_LIB_OUTPUT}/LLVM.dll.version" "${DAS_LLVM_TARGET_VERSION}")
        ELSE()
            # File not exists locally and LLVM lib not found.
            # I created prebuilt LLVM.dll for various platforms, let's download it.
            # Prebuilt archive contains single file named as LLVM.dll, on Windows it also should contain
            # clang-cl.exe
            # SHA256 hashes for the v22.1.5 release at
            # github.com/aleksisch/llvm-shared-builds. mac_64 has no asset in
            # this release — bail with a clear message pointing at brew.
            SET(DAS_LLVM_HAS_PREBUILT TRUE)
            IF(WIN32)
                SET(DAS_LLVM_FILE_PLATFORM win64)
                SET(DAS_LLVM_FILE_HASH "db6c151d26e06aad81682bbb46b5e7e3f454c51089c5d03c11651cd03b97494c")
            ELSEIF(APPLE)
                MESSAGE(STATUS ${CMAKE_SYSTEM_PROCESSOR})
                IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|aarch64")
                    SET(DAS_LLVM_FILE_PLATFORM mac_arm64)
                    SET(DAS_LLVM_FILE_HASH "55aa79531ac8aa0c3d62984e47d17e469fb4ece902ad36eb1514c0b9cef1938e")
                ELSE()
                    SET(DAS_LLVM_HAS_PREBUILT FALSE)
                ENDIF()
            ELSEIF(UNIX)
                MESSAGE(STATUS ${CMAKE_SYSTEM_PROCESSOR})
                IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|aarch64")
                    SET(DAS_LLVM_FILE_PLATFORM linux_arm64)
                    SET(DAS_LLVM_FILE_HASH "943696ac7f9875988d317bcd759cd5ab065a5ee02870a5ee92dab97163be4f9b")
                ELSE()
                    SET(DAS_LLVM_FILE_PLATFORM linux_64)
                    SET(DAS_LLVM_FILE_HASH "0f247652c43ac4da93d61b63db237f7d2f3eb74d09f98d810970687d1646ce9c")
                ENDIF()
            ENDIF()
            IF (NOT DAS_LLVM_HAS_PREBUILT)
                MESSAGE(FATAL_ERROR
                    "dasLlvm: no prebuilt LLVM.dll for this platform "
                    "(${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}) at "
                    "github.com/aleksisch/llvm-shared-builds v22.1.5. "
                    "Install LLVM ${DAS_LLVM_TARGET_VERSION} system-wide "
                    "(e.g. `brew install llvm` on macOS Intel) and re-run "
                    "cmake; find_package will pick it up. To disable the "
                    "module entirely set -DDAS_LLVM_DISABLED=ON.")
            ENDIF()
            SET(LLVM_LINK https://github.com/aleksisch/llvm-shared-builds/releases/download/v22.1.5/${DAS_LLVM_FILE_PLATFORM}_llvm.tar.gz)
            MESSAGE(STATUS "Download LLVM at: ${LLVM_LINK}")
            include(FetchContent)

            FetchContent_Declare(
                das_llvm_shared_lib
                URL_HASH               SHA256=${DAS_LLVM_FILE_HASH}
                DOWNLOAD_EXTRACT_TIMESTAMP 1
                INACTIVITY_TIMEOUT 60
                DOWNLOAD_RETRIES 5
                URL ${LLVM_LINK}
            )
            FetchContent_MakeAvailable(das_llvm_shared_lib)

            file(COPY ${das_llvm_shared_lib_SOURCE_DIR}/LLVM.dll DESTINATION ${CMAKE_SOURCE_DIR}/lib)
            IF(WIN32)
                file(COPY ${das_llvm_shared_lib_SOURCE_DIR}/clang-cl.exe DESTINATION ${CMAKE_SOURCE_DIR}/bin)
            ENDIF()
            file(WRITE "${DAS_LLVM_LIB_OUTPUT}/LLVM.dll.version" "${DAS_LLVM_TARGET_VERSION}")
        ENDIF()
    ELSE()
        MESSAGE(STATUS "LLVM lib exists at: ${DAS_LLVM_LIB_OUTPUT}/${DAS_LLVM_DLL_NAME}")
    ENDIF()

    IF(WIN32)
        install(FILES "${DAS_CLANG_OUTPUT}/clang-cl.exe" DESTINATION ${DAS_INSTALL_BINDIR})
    ENDIF()
    # Always put to single place, path and name should match with llvm_func.das definitions
    install(FILES "${DAS_LLVM_LIB_OUTPUT}/${DAS_LLVM_DLL_NAME}" DESTINATION ${DAS_INSTALL_LIBDIR})
	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLVM/LICENSE.TXT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLVM.LICENSE)

	file(GLOB DASLLVM_SOURCES
			${DAS_LLVM_DIR}/daslib/*.das
	)
	install(FILES ${DASLLVM_SOURCES}
			DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLVM/daslib
	)
	file(GLOB DASLLVM_BINDINGS ${DAS_LLVM_DIR}/bindings/*.das)
	install(FILES ${DASLLVM_BINDINGS} DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLVM/bindings)
	install(FILES ${DAS_LLVM_DIR}/.das_module DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLVM/)

	ADD_EXAMPLE_RUN(modules/dasLLVM/examples/hello_jit.das)
	ADD_EXAMPLE_RUN(modules/dasLLVM/examples/hello_llvm.das)
	ADD_EXAMPLE_RUN(modules/dasLLVM/examples/hello_llvm_jit.das)
	ADD_EXAMPLE_RUN(modules/dasLLVM/examples/hello_llvm_jit_function.das FALSE)

ENDIF()
