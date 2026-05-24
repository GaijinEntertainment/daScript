IF (NOT DEFINED PATH_TO_LIBCLANG)
	SET(PATH_TO_LIBCLANG "${PROJECT_SOURCE_DIR}/../libclang")
ENDIF()
IF ((NOT ${DAS_CLANG_BIND_DISABLED}) OR (NOT DEFINED DAS_CLANG_BIND_DISABLED))
	find_package(Clang 22.1 PATHS "${PATH_TO_LIBCLANG}")
ENDIF()
IF ((NOT DAS_CLANG_BIND_INCLUDED) AND (${Clang_FOUND}) AND ((NOT ${DAS_CLANG_BIND_DISABLED}) OR (NOT DEFINED DAS_CLANG_BIND_DISABLED)))
    SET(DAS_CLANG_BIND_INCLUDED TRUE)
    MESSAGE(STATUS "dasClangBind module included.")

	SET(DAS_CLANG_BIND_DIR ${PROJECT_SOURCE_DIR}/modules/dasClangBind)

	MESSAGE("LIBCLANG_LIB at ${LLVM_INSTALL_PREFIX}")

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_CLANG_BIND_DIR})

	SET(DAS_CLANG_BIND_MODULE_SRC
		${DAS_CLANG_BIND_DIR}/src/need_dasClangBind.h
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.h
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.cpp
	# user include
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.main.cpp
	# generated binding includes
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.alias.add.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.dummy.add.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.enum.add.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.enum.class.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.enum.decl.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.enum.decl.cast.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.struct.add.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.struct.postadd.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.struct.class.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.struct.decl.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.struct.impl.inc
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.struct.cast.inc
	# generated functions
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_1.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_2.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_3.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_4.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_5.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_6.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_7.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_8.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_9.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_10.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_11.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_12.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_13.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_14.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_15.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_16.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_17.cpp
		${DAS_CLANG_BIND_DIR}/src/dasClangBind.func_18.cpp
	)

	ADD_MODULE_CPP(dasClangBind)
	# ADD_MODULE_NATIVE(CLANG_BIND_boost)
	ADD_MODULE_LIB(libDasModuleClangBind dasModuleClangBind ${DAS_CLANG_BIND_MODULE_SRC} ${DAS_CLANG_BIND_MODULE_PLATFORM_SRC})
	IF(APPLE)
		TARGET_LINK_LIBRARIES(libDasModuleClangBind PRIVATE ${CLANG_BIND_LIBRARIES} ${LIBCLANG_LIB}/libclang.dylib)
		TARGET_LINK_LIBRARIES(dasModuleClangBind PRIVATE ${CLANG_BIND_LIBRARIES} ${LIBCLANG_LIB}/libclang.dylib)
    ELSE()
		TARGET_LINK_LIBRARIES(libDasModuleClangBind PRIVATE ${CLANG_BIND_LIBRARIES} libclang)
		TARGET_LINK_LIBRARIES(dasModuleClangBind PRIVATE ${CLANG_BIND_LIBRARIES} libclang)
	ENDIF()
	# ADD_DEPENDENCIES(libDasModuleClangBind)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleClangBind PUBLIC ${CLANG_BIND_INCLUDE_DIR} ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
	TARGET_INCLUDE_DIRECTORIES(dasModuleClangBind PUBLIC ${CLANG_BIND_INCLUDE_DIR} ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})

IF(WIN32)
    # POST_BUILD copy libclang + transitive runtime deps next to daslang.exe.
    # daslang loads .shared_module files via LoadLibraryEx with restricted
    # search flags (LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | _APPLICATION_DIR |
    # _DEFAULT_DIRS — see src/misc/sysos.cpp:loadDynamicLibrary). PATH is
    # NOT searched, so the .shared_module's libclang.dll import must live
    # in bin/ or alongside the .shared_module itself.
    #
    # Hook on dasModuleClangBind (the shared lib target), NOT on the static
    # libDasModuleClangBind — single-config Ninja builds only realize the
    # shared target when dynamic daslang.exe is the link consumer.
    #
    # Multi-config / single-config split mirrors dasHV/CMakeLists.txt.
    IF(MSVC)
        # Official LLVM Windows release ships a self-contained libclang.dll
        # (LLVM statically linked in). Companion DLLs LLVM-C/LTO/Remarks are
        # NOT deps of libclang.dll — copied for completeness only.
        SET(_DAS_CLANG_BIND_DLLS
            ${LLVM_INSTALL_PREFIX}/bin/libclang.dll
            ${LLVM_INSTALL_PREFIX}/bin/LLVM-C.dll
            ${LLVM_INSTALL_PREFIX}/bin/LTO.dll
            ${LLVM_INSTALL_PREFIX}/bin/Remarks.dll)
    ELSE()
        # mingw / clang-mingw64: msys2 ships libclang.dll dynamically linked
        # against libLLVM-N.dll, which in turn drags in libffi/zlib/zstd/libxml2.
        # Full chain (from objdump -p libclang.dll + libLLVM-N.dll):
        #   libclang.dll → libLLVM-N.dll, libc++, KERNEL32, api-ms-win-crt-*
        #   libLLVM-N.dll → libffi-N, zlib1, libzstd, libxml2-N, libc++,
        #                   SHELL32, ole32, ADVAPI32, WS2_32, ntdll, ...
        #   libc++.dll → libunwind, api-ms-win-crt-*
        # Glob the numbered variants (libLLVM-22, libffi-8, libxml2-16, …) to
        # absorb major-version bumps without per-bump CMake churn.
        FILE(GLOB _DAS_CLANG_BIND_DLLS
            "${LLVM_INSTALL_PREFIX}/bin/libclang.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libLLVM-*.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libffi-*.dll"
            "${LLVM_INSTALL_PREFIX}/bin/zlib1.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libzstd.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libxml2-*.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libiconv-*.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libc++.dll"
            "${LLVM_INSTALL_PREFIX}/bin/libunwind.dll")
    ENDIF()
    IF(_DAS_CLANG_BIND_DLLS)
        IF(CMAKE_CONFIGURATION_TYPES)
            SET(_DAS_CLANG_BIND_COPY_DIR "${EXECUTABLE_OUTPUT_PATH}/$<CONFIG>")
        ELSE()
            SET(_DAS_CLANG_BIND_COPY_DIR "${EXECUTABLE_OUTPUT_PATH}")
        ENDIF()
        ADD_CUSTOM_COMMAND(TARGET dasModuleClangBind POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${_DAS_CLANG_BIND_DLLS}
                "${_DAS_CLANG_BIND_COPY_DIR}/")
    ELSE()
        MESSAGE(WARNING "dasClangBind: no libclang DLLs found under ${LLVM_INSTALL_PREFIX}/bin — dasModuleClangBind.shared_module dlopen will fail at runtime.")
    ENDIF()
    # Guard install() with the same predicate as POST_BUILD —
    # install(FILES "" DESTINATION ...) errors at configure time, which
    # would defeat the soft warning above.
    IF(_DAS_CLANG_BIND_DLLS)
        install(FILES ${_DAS_CLANG_BIND_DLLS}
            DESTINATION ${DAS_INSTALL_BINDIR}
        )
    ENDIF()
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasClangBind/LICENSE.TXT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLVM.LICENSE)
ENDIF()

    ADD_MODULE_DAS(cbind cbind cbind_boost)

	SETUP_CPP11(libDasModuleClangBind)
	SETUP_CPP11(dasModuleClangBind)
ENDIF()
