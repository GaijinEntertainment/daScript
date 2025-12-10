IF (NOT DEFINED PATH_TO_LIBCLANG)
	SET(PATH_TO_LIBCLANG "${PROJECT_SOURCE_DIR}/../libclang")
ENDIF()
find_package(LLVM 16.0.6 PATHS "${PATH_TO_LIBCLANG}")

IF ((NOT DAS_LLVM_INCLUDED) AND ${LLVM_FOUND} AND ((NOT ${DAS_LLVM_DISABLED}) OR (NOT DEFINED DAS_LLVM_DISABLED)))
    SET(DAS_LLVM_INCLUDED TRUE)
    MESSAGE(STATUS "dasLlvm module included.")
	MESSAGE(STATUS "LLVM found at: ${LLVM_INSTALL_PREFIX}")

	SET(DAS_LLVM_DIR ${PROJECT_SOURCE_DIR}/modules/dasLLVM)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_LLVM_DIR})

	SET(DAS_LLVM_MODULE_SRC
		${DAS_LLVM_DIR}/src/dasLLVM.cpp
	)

	ADD_MODULE_CPP(dasLLVM)

	ADD_MODULE_DAS(llvm das_llvm llvm_const)
	ADD_MODULE_DAS(llvm das_llvm llvm_enum)
	ADD_MODULE_DAS(llvm das_llvm llvm_func)
	ADD_MODULE_DAS(llvm das_llvm llvm_struct)

	ADD_MODULE_DAS(llvm daslib llvm_boost)
	ADD_MODULE_DAS(llvm daslib llvm_debug)
	ADD_MODULE_DAS(llvm daslib llvm_jit)
	ADD_MODULE_DAS(llvm daslib llvm_targets)
	ADD_MODULE_DAS(llvm daslib llvm_jit_intrin)
	ADD_MODULE_DAS(llvm daslib llvm_jit_common)
	ADD_MODULE_DAS(llvm daslib llvm_dll_utils)
	ADD_MODULE_LIB(libDasModuleLlvm ${DAS_LLVM_MODULE_SRC})

	TARGET_INCLUDE_DIRECTORIES(libDasModuleLlvm PUBLIC ${LLVM_INCLUDE_DIRS})


IF(WIN32)
	# Copy clang-cl to use it as a linker
	add_custom_command(TARGET libDasModuleLlvm POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${LLVM_INSTALL_PREFIX}/bin/clang-cl.exe"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/clang-cl.exe"
	)
	install(FILES ${DAS_CLANG_DLLS} DESTINATION ${DAS_INSTALL_BINDIR})
	set(DAS_CLANG_DLLS ${LLVM_INSTALL_PREFIX}/bin/LLVM-C.dll)
ELSEIF(UNIX)
    set(DAS_CLANG_DLLS ${LLVM_INSTALL_PREFIX}/lib/libLLVM.so )
ELSEIF(APPLE)
    set(DAS_CLANG_DLLS ${LLVM_INSTALL_PREFIX}/lib/libLLVM-C.dylib)
ENDIF()
	# Always put to single place, it should match with llvm_func.das definitions
    set(DAS_CLANG_OUTPUT "${PROJECT_SOURCE_DIR}/lib/")
	foreach(dll ${DAS_CLANG_DLLS})
		get_filename_component(dll_name "${dll}" NAME)
		add_custom_command(TARGET libDasModuleLlvm POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${dll}"
            "${DAS_CLANG_OUTPUT}/${dll_name}"
		)
	endforeach()
	install(FILES ${DAS_CLANG_DLLS} DESTINATION ${DAS_INSTALL_LIBDIR})

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
