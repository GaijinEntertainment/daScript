
IF ((NOT DAS_HV_INCLUDED) AND ((NOT ${DAS_HV_DISABLED}) OR (NOT DEFINED DAS_HV_DISABLED)))
    SET(DAS_HV_INCLUDED TRUE)
    MESSAGE(STATUS "dasHV module included.")

	SET(DAS_HV_DIR ${PROJECT_SOURCE_DIR}/modules/dasHV)

	include(ExternalProject)

	IF(WIN32 AND MSVC)
		# MSVC: build OpenSSL from sources via `perl Configure VC-WIN64A` + nmake.
		# Both are MSVC-only — perl invokes OpenSSL's MSVC-flavored config and
		# nmake is the MSVC make tool. clang-mingw / gcc-mingw take the ELSE
		# branch below and pick up the system OpenSSL from the mingw sysroot.
		set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/openssl")
		find_package(OpenSSL QUIET PATHS ${OPENSSL_ROOT_DIR})
		IF(NOT OpenSSL_FOUND)
			# Build openssl from sources
			if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
				set(OPENSSL_ARCH "VC-WIN64A")
			else()
				set(OPENSSL_ARCH "VC-WIN32")
			endif()
			set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
			set(OPENSSL_LIBRARIES ${OPENSSL_ROOT_DIR}/lib)
			set(OPENSSL_LIBRARIES_FILES ${OPENSSL_ROOT_DIR}/lib/libcrypto.lib ${OPENSSL_ROOT_DIR}/lib/libssl.lib)

			# Note that under Ninja it should be run from VS developer code Prompt to setup all variables (or call vcvarsall.bat)
			ExternalProject_Add(openssl_build
				URL "https://www.openssl.org/source/openssl-3.5.1.tar.gz"
				URL_HASH SHA256=529043b15cffa5f36077a4d0af83f3de399807181d607441d734196d889b641f
				DOWNLOAD_EXTRACT_TIMESTAMP TRUE
				PREFIX ${OPENSSL_ROOT_DIR}
				CONFIGURE_COMMAND perl Configure ${OPENSSL_ARCH} no-shared --prefix=${OPENSSL_ROOT_DIR} --openssldir=${OPENSSL_ROOT_DIR}
				BUILD_COMMAND nmake
				INSTALL_COMMAND nmake install_sw
				BUILD_IN_SOURCE 1
				BUILD_BYPRODUCTS ${OPENSSL_LIBRARIES_FILES}
			)
			list(APPEND OPENSSL_LIBRARIES_FILES Crypt32.lib)
			SET(OPENSSL_FROM_SOURCES TRUE)
		ELSE()
			SET(OPENSSL_LIBRARIES_FILES OpenSSL::Crypto OpenSSL::SSL)
		ENDIF()

		SET(HV_LIBRARIES ${DAS_HV_DIR}/hv/$<CONFIG>/lib/hv_static.lib)
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
		find_package(OpenSSL REQUIRED)
		# libhv ≥ master renames hv_static's OUTPUT_NAME to "hv" on POSIX
		# (-> libhv.a) but keeps the original name on mingw (-> libhv_static.a).
		IF(WIN32)
			# mingw: also need winmm for timeSetEvent/timeKillEvent. MSVC
			# auto-pulls it via libhv's #pragma comment(lib, "winmm"); mingw
			# does not honor those reliably, so list it explicitly.
			SET(HV_LIBRARIES
				${DAS_HV_DIR}/hv/$<CONFIG>/lib/libhv_static.a
				winmm
			)
		ELSE()
			SET(HV_LIBRARIES ${DAS_HV_DIR}/hv/$<CONFIG>/lib/libhv.a)
		ENDIF()
		SET(OPENSSL_LIBRARIES_FILES OpenSSL::Crypto OpenSSL::SSL)
	ENDIF()


	SET(HV_CMAKE_FLAGS
		# Pin the libhv sub-build's config. Multi-config generators (VS) honor
		# the --config in BUILD/INSTALL_COMMAND below; single-config ones (Ninja)
		# ignore --config, so without this libhv defaults to Debug and its
		# debug-CRT hv_static.lib fails to link into Release dasModuleHV on MSVC
		# (unresolved __imp__CrtDbgReport).
		-DCMAKE_BUILD_TYPE=$<CONFIG>
		-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
		-DCMAKE_INSTALL_PREFIX=${DAS_HV_DIR}/hv/Release
		-DBUILD_EXAMPLES=OFF
		-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON
		-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
		-DCMAKE_LINKER=${CMAKE_LINKER}
		-DWITH_OPENSSL=ON
		-DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
		-DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
		-DOPENSSL_LIBRARIES="${OPENSSL_CRYPTO_LIBRARY};${OPENSSL_SSL_LIBRARY}"
	)
	IF(DAS_USE_SANITIZER STREQUAL "address" OR DAS_USE_SANITIZER STREQUAL "asan")
		IF(MSVC)
			SET(_HV_ASAN_FLAG "/fsanitize=address")
		ELSE()
			SET(_HV_ASAN_FLAG "-fsanitize=address")
		ENDIF()
		LIST(APPEND HV_CMAKE_FLAGS
			"-DCMAKE_C_FLAGS=${_HV_ASAN_FLAG}"
			"-DCMAKE_CXX_FLAGS=${_HV_ASAN_FLAG}"
		)
	ENDIF()
	# daslang compiles its modules with the static CRT (/MT, /MTd); libhv's
	# ExternalProject otherwise defaults to /MD and the resulting hv_static.lib
	# fails to link into dasModuleHV on MSVC (LNK2038 RuntimeLibrary mismatch +
	# unresolved __imp_* dynamic-CRT imports). BUILD_FOR_MT is libhv's own knob
	# to switch its sub-build to /MT (Release) / /MTd (Debug) and match daslang.
	IF(MSVC)
		LIST(APPEND HV_CMAKE_FLAGS -DBUILD_FOR_MT=ON)
	ENDIF()
	ExternalProject_Add(
		LIBHV
		URL https://github.com/ithewei/libhv/archive/5da5fc22cdfd1b59ba2b0520abb34f95d050975c.tar.gz
		URL_HASH SHA256=b4f774936ccd6981df74b23f9e033e0b3ab584f6682d889490377dc9450b5f54
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libhv
		CMAKE_ARGS ${HV_CMAKE_FLAGS}
		BUILD_BYPRODUCTS ${HV_LIBRARIES}
		BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
		INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG> --prefix ${DAS_HV_DIR}/hv/$<CONFIG>
	)
	# Copy cert files from downloaded libhv source (not stored in repo to avoid pushing private keys)
	SET(LIBHV_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/libhv/src/LIBHV)
	ExternalProject_Add_Step(LIBHV copy_certs
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBHV_SRC_DIR}/cert ${DAS_HV_DIR}/cert
		DEPENDEES install
		COMMENT "Copying libhv cert files to ${DAS_HV_DIR}/cert"
	)
	IF(OPENSSL_FROM_SOURCES)
		add_dependencies(LIBHV openssl_build)
	ENDIF()

	IF(APPLE)
		list(APPEND HV_LIBRARIES
			"-framework CoreFoundation"
			"-framework Security"
		)
	ENDIF()

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_HV_DIR})

	# libDasModuleHV
	SET(DAS_HV_MODULE_SRC
		${DAS_HV_DIR}/src/dasHV.h
		${DAS_HV_DIR}/src/dasHV.cpp
	)

	ADD_MODULE_CPP(HV)
	ADD_MODULE_LIB(libDasModuleHV dasModuleHV ${DAS_HV_MODULE_SRC} ${DAS_HV_MODULE_PLATFORM_SRC})
	MACRO(SETUP_HV lib)
		TARGET_INCLUDE_DIRECTORIES(${lib} PRIVATE "${DAS_HV_DIR}/hv/$<CONFIG>/include")
		TARGET_LINK_LIBRARIES(${lib} PRIVATE ${HV_LIBRARIES} ${OPENSSL_LIBRARIES_FILES})
		ADD_DEPENDENCIES(${lib} LIBHV)
		SETUP_CPP11(${lib})
	ENDMACRO()
	SETUP_HV(libDasModuleHV)
	SETUP_HV(dasModuleHV)

	IF(WIN32 AND NOT MSVC)
		# mingw: land libssl-*.dll + libcrypto-*.dll NEXT TO daslang.exe so
		# dasModuleHV.shared_module can resolve its openssl deps at dlopen
		# time. MSVC has its own openssl build path (no-shared above), so it
		# doesn't need this. Without these copies, build-time AOT codegen
		# fails silently because register_dynamic_module's default overload
		# (register_dynamic_module_silent) suppresses the LoadLibrary error
		# and `require dashv` then surfaces as 20605 missing-prerequisite.
		# Glob picks up the version suffix (libssl-3-x64.dll today, future
		# major bumps without CMake churn).
		FILE(GLOB _DAS_HV_OPENSSL_DLLS
			"${OPENSSL_ROOT_DIR}/bin/libssl-*.dll"
			"${OPENSSL_ROOT_DIR}/bin/libcrypto-*.dll")
		IF(_DAS_HV_OPENSSL_DLLS)
			# Mirror dasGlfw's multi-config / single-config split — Ninja
			# (mingw default) is single-config; left in for symmetry with
			# the glfw rule and any future generator switch.
			IF(CMAKE_CONFIGURATION_TYPES)
				SET(_DAS_HV_COPY_DIR "${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>")
			ELSE()
				SET(_DAS_HV_COPY_DIR "${EXECUTABLE_OUTPUT_PATH}")
			ENDIF()
			ADD_CUSTOM_COMMAND(TARGET dasModuleHV POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
					${_DAS_HV_OPENSSL_DLLS}
					"${_DAS_HV_COPY_DIR}/"
			)
		ELSE()
			MESSAGE(WARNING "dasHV mingw: no libssl/libcrypto DLLs found under ${OPENSSL_ROOT_DIR}/bin — dasModuleHV.shared_module may fail to load at runtime")
		ENDIF()
	ENDIF()

	ADD_MODULE_DAS(dashv dashv dashv_boost)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasHV/dashv
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasHV
        FILES_MATCHING
        PATTERN "*.das"
    )

	install(DIRECTORY ${DAS_HV_DIR}/cert
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasHV
        FILES_MATCHING
        PATTERN "*.crt"
    )

	install(DIRECTORY ${DAS_HV_DIR}/cert
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasHV
        FILES_MATCHING
        PATTERN "*.key"
    )

	install(FILES ${DAS_HV_DIR}/HV.LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME HV.LICENSE)

ENDIF()
