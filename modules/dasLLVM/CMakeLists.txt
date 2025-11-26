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
	# monkey patched
		${DAS_LLVM_DIR}/src/cb_dasLLVM.h
	# GENERATED
		${DAS_LLVM_DIR}/src/need_dasLLVM.h
		${DAS_LLVM_DIR}/src/dasLLVM.h
		${DAS_LLVM_DIR}/src/dasLLVM.cpp
	# user include
		${DAS_LLVM_DIR}/src/dasLLVM.main.cpp
	# generated binding includes
		${DAS_LLVM_DIR}/src/dasLLVM.alias.add.inc
		${DAS_LLVM_DIR}/src/dasLLVM.dummy.add.inc
		${DAS_LLVM_DIR}/src/dasLLVM.enum.add.inc
		${DAS_LLVM_DIR}/src/dasLLVM.enum.class.inc
		${DAS_LLVM_DIR}/src/dasLLVM.enum.decl.inc
		${DAS_LLVM_DIR}/src/dasLLVM.enum.decl.cast.inc
		${DAS_LLVM_DIR}/src/dasLLVM.struct.add.inc
		${DAS_LLVM_DIR}/src/dasLLVM.struct.postadd.inc
		${DAS_LLVM_DIR}/src/dasLLVM.struct.class.inc
		${DAS_LLVM_DIR}/src/dasLLVM.struct.decl.inc
		${DAS_LLVM_DIR}/src/dasLLVM.struct.impl.inc
		${DAS_LLVM_DIR}/src/dasLLVM.struct.cast.inc
		${DAS_LLVM_DIR}/src/dasLLVM.func.reg.inc
		${DAS_LLVM_DIR}/src/dasLLVM.func.decl.inc
		${DAS_LLVM_DIR}/src/dasLLVM.func.aot.inc
		${DAS_LLVM_DIR}/src/dasLLVM.func.aot.decl.inc
	# generated functions
		${DAS_LLVM_DIR}/src/dasLLVM.func_1.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_2.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_3.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_4.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_5.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_6.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_7.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_8.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_9.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_10.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_11.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_12.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_13.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_14.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_15.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_16.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_17.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_18.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_19.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_20.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_21.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_22.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_23.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_24.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_25.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_26.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_27.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_28.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_29.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_30.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_31.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_32.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_33.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_34.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_35.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_36.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_37.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_38.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_39.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_40.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_41.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_42.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_43.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_44.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_45.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_46.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_47.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_48.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_49.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_50.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_51.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_52.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_53.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_54.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_55.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_56.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_57.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_58.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_59.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_60.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_61.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_62.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_63.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_64.cpp
		${DAS_LLVM_DIR}/src/dasLLVM.func_65.cpp
	# llvm config module ( for passing setting externally )
		${DAS_LLVM_DIR}/src/llvm_config/llvmConfigModule.cpp
		${DAS_LLVM_DIR}/src/llvm_config/llvmConfigStub.cpp
	)

	ADD_MODULE_CPP(LlvmConfig)
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
