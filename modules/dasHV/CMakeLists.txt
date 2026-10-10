
IF ((NOT DAS_HV_INCLUDED) AND ((NOT ${DAS_HV_DISABLED}) OR (NOT DEFINED DAS_HV_DISABLED)))
    SET(DAS_HV_INCLUDED TRUE)
    MESSAGE(STATUS "dasHV module included.")

	SET(DAS_HV_DIR ${PROJECT_SOURCE_DIR}/modules/dasHV)

	include(ExternalProject)

    include(${PROJECT_SOURCE_DIR}/cmake/DasOpenSSL.cmake)
    if(WIN32 AND MSVC)
        set(HV_LIBRARIES ${DAS_HV_DIR}/hv/$<CONFIG>/lib/hv_static.lib)
    elseif(WIN32)
        set(HV_LIBRARIES ${DAS_HV_DIR}/hv/$<CONFIG>/lib/libhv_static.a winmm)
    else()
        set(HV_LIBRARIES ${DAS_HV_DIR}/hv/$<CONFIG>/lib/libhv.a)
    endif()


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
	# Forward the parent build's C/C++ + linker flags into the libhv sub-build so its
	# stdlib/ABI matches daslang. Without this a libc++ host build (-stdlib=libc++, the
	# wasm cross-compile host) leaves libhv on clang's default libstdc++, and the resulting
	# dasModuleHV.shared_module carries undefined std::__cxx11 symbols that fail to dlopen
	# under the libc++ daslang (surfaces as a misleading error[20605] missing prereq 'dashv').
	SET(_HV_C_FLAGS "${CMAKE_C_FLAGS}")
	SET(_HV_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	# libhv's parse_json() (http/http_content.cpp) wraps nlohmann::json::parse in
	# try/catch, so libhv MUST build with C++ exceptions. daslang strips /EHsc from
	# CMAKE_CXX_FLAGS (CMakeCommon.txt) and re-adds /EHa only to the per-config flag
	# vars, which are NOT forwarded here — so the sub-build inherits no /EH at all.
	# Plain cl only warns (C4530); clang-cl (the ClangCL-toolset nightly lane) makes
	# it a hard error ("cannot use 'try' with exceptions disabled"). Re-add /EHsc for
	# the MSVC drivers (cl + clang-cl); non-MSVC (emscripten host, gcc/clang) is
	# untouched — those keep exceptions on by default.
	IF(MSVC)
		SET(_HV_CXX_FLAGS "${_HV_CXX_FLAGS} /EHsc")
	ENDIF()
	SET(_HV_SAN_FLAG "")
	IF(DAS_USE_SANITIZER STREQUAL "address" OR DAS_USE_SANITIZER STREQUAL "asan")
		IF(MSVC)
			SET(_HV_SAN_FLAG "/fsanitize=address")
		ELSE()
			SET(_HV_SAN_FLAG "-fsanitize=address")
		ENDIF()
	ELSEIF((DAS_USE_SANITIZER STREQUAL "thread" OR DAS_USE_SANITIZER STREQUAL "tsan") AND NOT MSVC)
		# An uninstrumented libhv hides its atomics from tsan — proper shared_ptr teardown
		# then reads as a false "data race in operator delete" (partially-instrumented code).
		SET(_HV_SAN_FLAG "-fsanitize=thread")
	ENDIF()
	IF(_HV_SAN_FLAG)
		SET(_HV_C_FLAGS "${_HV_C_FLAGS} ${_HV_SAN_FLAG}")
		SET(_HV_CXX_FLAGS "${_HV_CXX_FLAGS} ${_HV_SAN_FLAG}")
	ENDIF()
	LIST(APPEND HV_CMAKE_FLAGS
		"-DCMAKE_C_FLAGS:STRING=${_HV_C_FLAGS}"
		"-DCMAKE_CXX_FLAGS:STRING=${_HV_CXX_FLAGS}"
		"-DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}"
		"-DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}"
	)
	# libhv's ExternalProject picks its own CRT; if it disagrees with daslang's,
	# hv_static.lib fails to link into dasModuleHV (LNK2038 RuntimeLibrary
	# mismatch: MT_StaticRelease vs MD_DynamicRelease). daslang uses the dynamic
	# CRT (/MD) by default and switches to the static CRT (/MT) only when
	# DAS_USE_STATIC_STD_LIBS is ON (see CMakeCommon.txt). Mirror that exact knob
	# into libhv's own BUILD_FOR_MT so the two always agree; when it is OFF,
	# libhv's /MD default already matches and we leave it alone.
	IF(MSVC AND DAS_USE_STATIC_STD_LIBS)
		LIST(APPEND HV_CMAKE_FLAGS -DBUILD_FOR_MT=ON)
	ENDIF()
    set(HV_BUILD_DEPENDENCIES)
    if(TARGET openssl_build)
        list(APPEND HV_BUILD_DEPENDENCIES openssl_build)
    endif()
	ExternalProject_Add(
		LIBHV
        DEPENDS ${HV_BUILD_DEPENDENCIES}
		URL https://github.com/ithewei/libhv/archive/303f50c7b23f250c5b620d92c04b41060e1701c2.tar.gz
		URL_HASH SHA256=43f425dae61d060913c5ce280c9776dfb98c61bb965a38c5a38d4788a7295201
		DOWNLOAD_EXTRACT_TIMESTAMP FALSE
		PATCH_COMMAND ${CMAKE_COMMAND} -DLIBHV_SRC_DIR=<SOURCE_DIR> -P ${DAS_HV_DIR}/patch_libhv.cmake
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libhv
		CMAKE_ARGS ${HV_CMAKE_FLAGS}
		BUILD_BYPRODUCTS ${HV_LIBRARIES}
		BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG> ${DAS_NESTED_BUILD_TOOL_ARGS}
		INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG> --prefix ${DAS_HV_DIR}/hv/$<CONFIG>
	)
	ExternalProject_Add_StepDependencies(LIBHV patch ${DAS_HV_DIR}/patch_libhv.cmake ${DAS_HV_DIR}/patch_libhv_limits.cmake ${DAS_HV_DIR}/patch_libhv_upgrade.cmake ${DAS_HV_DIR}/patch_libhv_tls.cmake ${DAS_HV_DIR}/patch_libhv_pipeline.cmake ${DAS_HV_DIR}/patch_libhv_logging.cmake ${DAS_HV_DIR}/patch_libhv_cookies.cmake ${DAS_HV_DIR}/patch_libhv_shutdown.cmake ${DAS_HV_DIR}/patch_libhv_date.cmake)
    set(HV_PATCHED_PUBLIC_HEADERS)
    foreach(header AsyncHttpClient.h HttpParser.h HttpMessage.h WebSocketParser.h WebSocketServer.h HttpResponseWriter.h HttpService.h)
        list(APPEND HV_PATCHED_PUBLIC_HEADERS "${DAS_HV_DIR}/hv/$<CONFIG>/include/hv/${header}")
    endforeach()
    ExternalProject_Add_Step(LIBHV public_headers
        DEPENDEES install
        COMMAND ${CMAKE_COMMAND} -E touch ${HV_PATCHED_PUBLIC_HEADERS}
        BYPRODUCTS ${HV_PATCHED_PUBLIC_HEADERS})

	# Copy cert files from downloaded libhv source (not stored in repo to avoid pushing private keys)
	SET(LIBHV_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/libhv/src/LIBHV)
	ExternalProject_Add_Step(LIBHV copy_certs
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBHV_SRC_DIR}/cert ${DAS_HV_DIR}/cert
		DEPENDEES install
		COMMENT "Copying libhv cert files to ${DAS_HV_DIR}/cert"
	)

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
		TARGET_INCLUDE_DIRECTORIES(${lib} SYSTEM PRIVATE "${DAS_HV_DIR}/hv/$<CONFIG>/include")
		TARGET_LINK_LIBRARIES(${lib} PRIVATE ${HV_LIBRARIES})
        das_link_openssl(${lib})
        if(APPLE)
            target_link_libraries(${lib} PRIVATE "-framework Security" "-framework CoreFoundation")
        endif()
		ADD_DEPENDENCIES(${lib} LIBHV)
		SETUP_CPP11(${lib})
	ENDMACRO()
	SETUP_HV(libDasModuleHV)
	SETUP_HV(dasModuleHV)

    das_copy_openssl_runtime(dasModuleHV dasHV)

	ADD_MODULE_DAS_FROM_DESCRIPTOR(dashv dashv)

	# AOT-able sources (for test_aot / any consumer); empty when disabled. The test-side
	# fixture (_dashv_test_common) is appended by tests/aot, it is not a module source.
	SET(DASHV_AOT_FILES
		modules/dasHV/dashv/dashv_boost.das
)

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
	# OpenSSL is statically linked (MSVC) or shipped as DLLs (mingw) — Apache-2.0 notice must ride along
	install(FILES ${DAS_HV_DIR}/OPENSSL.LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME OPENSSL.LICENSE)

ENDIF()
