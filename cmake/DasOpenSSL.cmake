include_guard(GLOBAL)
include(ExternalProject)

	IF(WIN32 AND MSVC)
		# MSVC: build OpenSSL from sources via `perl Configure VC-WIN64A` + nmake.
		# Both are MSVC-only — perl invokes OpenSSL's MSVC-flavored config and
		# nmake is the MSVC make tool. clang-mingw / gcc-mingw take the ELSE
		# branch below and pick up the system OpenSSL from the mingw sysroot.
		# OPENSSL_ROOT_DIR defaults to a build-dir-local openssl/, but honors an
		# override (env DASLANG_OPENSSL_DIR, or -DOPENSSL_ROOT_DIR) so OpenSSL can
		# be built once into a shared per-machine cache and reused across build
		# dirs / worktrees instead of rebuilt per build dir. CI sets neither, so
		# its per-build-dir path (and the build-clangcl/openssl cache) is unchanged.
		if(NOT OPENSSL_ROOT_DIR AND DEFINED ENV{DASLANG_OPENSSL_DIR})
			set(OPENSSL_ROOT_DIR "$ENV{DASLANG_OPENSSL_DIR}")
		endif()
		if(NOT OPENSSL_ROOT_DIR)
			set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/openssl")
		endif()
		find_package(OpenSSL QUIET PATHS "${OPENSSL_ROOT_DIR}")
		IF(NOT OpenSSL_FOUND)
			MESSAGE(STATUS "OpenSSL: OpenSSL not found at ${OPENSSL_ROOT_DIR} - building 3.5.1 from source (one-time per OPENSSL_ROOT_DIR)")
			# Build openssl from sources
			if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
				set(OPENSSL_ARCH "VC-WIN64A")
			else()
				set(OPENSSL_ARCH "VC-WIN32")
			endif()
			set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
			set(OPENSSL_LIBRARIES ${OPENSSL_ROOT_DIR}/lib)
			set(OPENSSL_LIBRARIES_FILES ${OPENSSL_ROOT_DIR}/lib/libcrypto.lib ${OPENSSL_ROOT_DIR}/lib/libssl.lib)

			# clang-cl also satisfies MSVC, but the VS ClangCL toolset puts clang's
			# resource include (whose <stdint.h>/<vadefs.h> use #include_next) on
			# INCLUDE; OpenSSL builds with plain cl, which rejects #include_next (C1021).
			# Run nmake through a wrapper that strips the clang resource dir from INCLUDE
			# so cl falls back to MSVC's stdint.h. (The strip propagates via env to
			# nmake's recursive cl children; a `nmake CC=clang-cl` override does not.)
			IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
				SET(_OSSL_BUILD_CMD ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/scrub_openssl_include.cmake -- nmake)
				SET(_OSSL_INSTALL_CMD ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/scrub_openssl_include.cmake -- nmake install_sw)
			ELSE()
				SET(_OSSL_BUILD_CMD nmake)
				SET(_OSSL_INSTALL_CMD nmake install_sw)
			ENDIF()
			# Note that under Ninja it should be run from VS developer code Prompt to setup all variables (or call vcvarsall.bat)
			ExternalProject_Add(openssl_build
				URL "https://www.openssl.org/source/openssl-3.5.1.tar.gz"
				URL_HASH SHA256=529043b15cffa5f36077a4d0af83f3de399807181d607441d734196d889b641f
				DOWNLOAD_EXTRACT_TIMESTAMP TRUE
				PREFIX ${OPENSSL_ROOT_DIR}
				CONFIGURE_COMMAND perl Configure ${OPENSSL_ARCH} no-shared --prefix=${OPENSSL_ROOT_DIR} --openssldir=${OPENSSL_ROOT_DIR}
				BUILD_COMMAND ${_OSSL_BUILD_CMD}
				INSTALL_COMMAND ${_OSSL_INSTALL_CMD}
				BUILD_IN_SOURCE 1
				BUILD_BYPRODUCTS ${OPENSSL_LIBRARIES_FILES}
			)
			list(APPEND OPENSSL_LIBRARIES_FILES Crypt32.lib)
			SET(OPENSSL_FROM_SOURCES TRUE)
		ELSE()
			MESSAGE(STATUS "OpenSSL: using prebuilt OpenSSL ${OPENSSL_VERSION} (include: ${OPENSSL_INCLUDE_DIR}) - skipping source build")
			SET(OPENSSL_LIBRARIES_FILES OpenSSL::Crypto OpenSSL::SSL)
		ENDIF()

	ELSE()
		# POSIX + mingw (clang-mingw / gcc-mingw): use system OpenSSL.
		IF(WIN32 AND NOT DEFINED OPENSSL_ROOT_DIR)
			# FindOpenSSL.cmake on Windows is MSVC-centric and doesn't look in
			# the mingw sysroot by default. Derive the sysroot from the C
			# compiler path: <SYSROOT>/bin/clang.exe -> <SYSROOT>. clang64
			# ships <SYSROOT>/lib/libcrypto.dll.a + libssl.dll.a, which
			# find_package then locates correctly with this hint.
			get_filename_component(_MINGW_BIN ${CMAKE_C_COMPILER} DIRECTORY)
			get_filename_component(OPENSSL_ROOT_DIR ${_MINGW_BIN} DIRECTORY)
		ENDIF()
		IF(APPLE)
			# Homebrew's OpenSSL dylibs carry absolute install names under /opt/homebrew, so a
			# module that links them loads only on a box with that Homebrew tree. The static
			# archives beside them fold OpenSSL into the linked shared module, and a daspkg
			# bundle or an SDK install runs on a Mac with no Homebrew at all.
			SET(OPENSSL_USE_STATIC_LIBS TRUE)
		ENDIF()
		find_package(OpenSSL REQUIRED)
		SET(OPENSSL_LIBRARIES_FILES OpenSSL::Crypto OpenSSL::SSL)
	ENDIF()


function(das_link_openssl target)
    target_include_directories(${target} SYSTEM PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(${target} PRIVATE ${OPENSSL_LIBRARIES_FILES})
    if(WIN32)
        target_link_libraries(${target} PRIVATE ws2_32 crypt32)
    endif()
    if(TARGET openssl_build)
        add_dependencies(${target} openssl_build)
    endif()
endfunction()

function(das_copy_openssl_runtime target module_folder)
	IF(WIN32)
		# Shared OpenSSL DLLs must be beside the executable during builds and
		# beside the module in installs for Windows to load ${target}.shared_module.
		# The MSVC source build is static, so its DLL globs remain empty.
		FILE(GLOB _DAS_HV_OPENSSL_DLLS
			"${OPENSSL_ROOT_DIR}/bin/libssl-*.dll"
			"${OPENSSL_ROOT_DIR}/bin/libcrypto-*.dll")
		IF(NOT _DAS_HV_OPENSSL_DLLS AND OPENSSL_INCLUDE_DIR)
			# vcpkg layout: <triplet-root>/include + <triplet-root>/bin DLLs.
			FILE(GLOB _DAS_HV_OPENSSL_DLLS
				"${OPENSSL_INCLUDE_DIR}/../bin/libssl-*.dll"
				"${OPENSSL_INCLUDE_DIR}/../bin/libcrypto-*.dll")
		ENDIF()
		IF(_DAS_HV_OPENSSL_DLLS)
			IF(CMAKE_CONFIGURATION_TYPES)
				SET(_DAS_HV_COPY_DIR "${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>")
			ELSE()
				SET(_DAS_HV_COPY_DIR "${EXECUTABLE_OUTPUT_PATH}")
			ENDIF()
			ADD_CUSTOM_COMMAND(TARGET ${target} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					${_DAS_HV_OPENSSL_DLLS}
					"${_DAS_HV_COPY_DIR}/"
			)
			install(FILES ${_DAS_HV_OPENSSL_DLLS}
				DESTINATION ${DAS_INSTALL_MODULESDIR}/${module_folder}
			)
		ELSEIF(NOT MSVC)
			MESSAGE(WARNING "${module_folder}: no libssl/libcrypto DLLs found under ${OPENSSL_ROOT_DIR}/bin — ${target}.shared_module may fail to load at runtime")
		ENDIF()
	ENDIF()

endfunction()

if(NOT OPENSSL_FROM_SOURCES AND OPENSSL_VERSION VERSION_LESS "1.1.1")
    message(FATAL_ERROR "OpenSSL 1.1.1 or newer is required")
endif()
