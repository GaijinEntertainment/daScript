IF ((NOT DAS_LLVM_INCLUDED) AND ((NOT ${DAS_LLVM_DISABLED}) OR (NOT DEFINED DAS_LLVM_DISABLED)))
    SET(DAS_LLVM_INCLUDED TRUE)
    MESSAGE(STATUS "dasLlvm module included.")

	SET(DAS_LLVM_DIR ${PROJECT_SOURCE_DIR}/modules/dasLLVM)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_LLVM_DIR})

	SET(DAS_LLVM_MODULE_SRC
		${DAS_LLVM_DIR}/src/dasLLVM.cpp
	)

	ADD_MODULE_CPP(dasLLVM)

	ADD_MODULE_DAS(llvm bindings llvm_const)
	ADD_MODULE_DAS(llvm bindings llvm_enum)
	ADD_MODULE_DAS(llvm bindings llvm_func)
	ADD_MODULE_DAS(llvm bindings llvm_struct)

	ADD_MODULE_DAS(llvm daslib llvm_boost)
	ADD_MODULE_DAS(llvm daslib llvm_debug)
	ADD_MODULE_DAS(llvm daslib llvm_jit)
	ADD_MODULE_DAS(llvm daslib llvm_targets)
	ADD_MODULE_DAS(llvm daslib llvm_jit_intrin)
	ADD_MODULE_DAS(llvm daslib llvm_jit_common)
	ADD_MODULE_DAS(llvm daslib llvm_dll_utils)
	ADD_MODULE_LIB(libDasModuleLlvm ${DAS_LLVM_MODULE_SRC})

	TARGET_INCLUDE_DIRECTORIES(libDasModuleLlvm PUBLIC ${LLVM_INCLUDE_DIRS})

    set(DAS_LLVM_LIB_OUTPUT "${PROJECT_SOURCE_DIR}/lib")
    set(DAS_CLANG_OUTPUT "${PROJECT_SOURCE_DIR}/bin")
    set(DAS_LLVM_DLL_NAME LLVM.dll)

    IF (NOT EXISTS "${DAS_LLVM_LIB_OUTPUT}/${DAS_LLVM_DLL_NAME}")
        find_package(LLVM 16.0.6)
        IF (LLVM_FOUND)
            MESSAGE(STATUS "LLVM found at: ${LLVM_INSTALL_PREFIX}")
            IF(WIN32)
                # Copy clang-cl to use it as a linker
                add_custom_command(TARGET libDasModuleLlvm POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${LLVM_INSTALL_PREFIX}/bin/clang-cl.exe"
                    "${DAS_CLANG_OUTPUT}/clang-cl.exe"
                )
                set(DAS_LLVM_DLL_PATH ${LLVM_INSTALL_PREFIX}/bin/LLVM-C.dll)
            ELSEIF(APPLE)
                set(DAS_LLVM_DLL_PATH ${LLVM_INSTALL_PREFIX}/lib/libLLVM.dylib)
            ELSE()
                set(DAS_LLVM_DLL_PATH ${LLVM_INSTALL_PREFIX}/lib/libLLVM.so )
            ENDIF()
            get_filename_component(dll_name "${dll}" NAME)
            add_custom_command(TARGET libDasModuleLlvm POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DAS_LLVM_DLL_PATH}"
                "${DAS_LLVM_LIB_OUTPUT}/${DAS_LLVM_DLL_NAME}"
            )
        ELSE()
            # File not exists locally and LLVM lib not found.
            # I created prebuilt LLVM.dll for various platforms, let's download it.
            # Prebuilt archive contains single file named as LLVM.dll, on Windows it also should contain
            # clang-cl.exe
            IF(WIN32)
                SET(DAS_LLVM_FILE_PLATFORM win64)
                SET(DAS_LLVM_FILE_HASH "429f819c7e9b12f225f1754a8208ea2a2645a8f5b814de213e8201e299d2191e")
            ELSEIF(APPLE)
                MESSAGE(STATUS ${CMAKE_SYSTEM_PROCESSOR})
                IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|aarch64")
                    SET(DAS_LLVM_FILE_PLATFORM mac_arm64)
                    SET(DAS_LLVM_FILE_HASH "5a697ff554d2b41aa47a4baf67516548db2ca348e7f39e92e53b5e52e77f518a")
                ELSE()
                    SET(DAS_LLVM_FILE_PLATFORM mac_64)
                    SET(DAS_LLVM_FILE_HASH "6b919de9d013828c0bb0ef8297c7aee5c2c1859b748fb54f949615c08c57166b")
                ENDIF()
            ELSEIF(UNIX)
                MESSAGE(STATUS ${CMAKE_SYSTEM_PROCESSOR})
                IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|aarch64")
                    SET(DAS_LLVM_FILE_PLATFORM linux_arm64)
                    SET(DAS_LLVM_FILE_HASH "9d86a9d078ca95e3797ecbca5fb590d1b57156474c43df141b7f9ee8a5e6826e")
                ELSE()
                    SET(DAS_LLVM_FILE_PLATFORM linux_64)
                    SET(DAS_LLVM_FILE_HASH "b2553e1cbf71a939584366aa896752a5f598f2f26eaa79d8105288fb58f47da7")
                ENDIF()
            ENDIF()
            SET(LLVM_LINK https://github.com/aleksisch/llvm-16-prebuilt/releases/download/v2.0.0/${DAS_LLVM_FILE_PLATFORM}_llvm.tar.gz)
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
	file(GLOB DASLLVM_BINDINGS ${DAS_LLVM_DIR}/das_llvm/*.das)
	install(FILES ${DASLLVM_BINDINGS} DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLVM/das_llvm)

	SETUP_CPP11(libDasModuleLlvm)
ENDIF()
