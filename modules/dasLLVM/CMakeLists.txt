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
	# user include
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

	IF(WIN32)
		TARGET_LINK_LIBRARIES(libDasModuleLlvm PUBLIC LLVM-C LTO Remarks)
	ELSE()
		TARGET_LINK_LIBRARIES(libDasModuleLlvm PUBLIC LLVM LTO Remarks)
	ENDIF()
	# ADD_DEPENDENCIES(libDasModuleLlvm)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleLlvm PUBLIC ${LLVM_INCLUDE_DIRS})


IF(WIN32)
	set(DAS_CLANG_DLLS
		${LLVM_INSTALL_PREFIX}/bin/LLVM-C.dll
		${LLVM_INSTALL_PREFIX}/bin/LTO.dll
		${LLVM_INSTALL_PREFIX}/bin/Remarks.dll
		${LLVM_INSTALL_PREFIX}/bin/clang-cl.exe
		${LLVM_INSTALL_PREFIX}/bin/libclang.dll
	)
	foreach(dll ${DAS_CLANG_DLLS})
		get_filename_component(dll_name "${dll}" NAME)
		add_custom_command(TARGET libDasModuleLlvm POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${dll}"
			"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/${dll_name}"
		)
	endforeach()
	install(FILES ${DAS_CLANG_DLLS}
			DESTINATION ${DAS_INSTALL_BINDIR}
	)
ENDIF()

	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLVM/LICENSE.TXT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLVM.LICENSE)

	file(GLOB DASLLVM_SOURCES
			${DAS_LLVM_DIR}/daslib/*.das
	)
	install(FILES ${DASLLVM_SOURCES}
			DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLVM/daslib
	)
    #ADD_MODULE_DAS(cbind cbind cbind_boost)

	SETUP_CPP11(libDasModuleLlvm)
ENDIF()
