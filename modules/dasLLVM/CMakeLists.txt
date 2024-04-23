SET(PATH_TO_LIBCLANG ${PROJECT_SOURCE_DIR}/../libclang)
IF(NOT IS_DIRECTORY ${PATH_TO_LIBCLANG})
	SET(DAS_LIBCLANG_DETECTED FALSE)
	message(WARNING "can't find libclang at ${PATH_TO_LIBCLANG}")
ELSE()
	IF(FALSE) #"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		SET(DAS_LIBCLANG_DETECTED FALSE)
		message(WARNING "No libclang on CLANG/WIN for now.")
	ELSE()
		SET(DAS_LIBCLANG_DETECTED TRUE)
	ENDIF()
ENDIF()

IF ((NOT DAS_LLVM_INCLUDED) AND DAS_LIBCLANG_DETECTED AND ((NOT ${DAS_LLVM_DISABLED}) OR (NOT DEFINED DAS_LLVM_DISABLED)))
    SET(DAS_LLVM_INCLUDED TRUE)
    MESSAGE(STATUS "dasLlvm module included.")

	SET(DAS_LLVM_DIR ${PROJECT_SOURCE_DIR}/modules/dasLlvm)
	SET(LIBCLANG_INCLUDE ${PATH_TO_LIBCLANG}/include)
	SET(LIBCLANG_LIB ${PATH_TO_LIBCLANG}/lib)

	MESSAGE("LIBCLANG_INCLUDE at ${LIBCLANG_INCLUDE}")
	MESSAGE("LIBCLANG_LIB at ${LIBCLANG_LIB}")

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

	ADD_MODULE_LIB(libDasModuleLlvm)
	ADD_MODULE_CPP(LlvmConfig)
	ADD_MODULE_CPP(dasLLVM)
	ADD_MODULE_DAS(llvm daslib llvm_boost)
	ADD_MODULE_DAS(llvm daslib llvm_debug)
	ADD_MODULE_DAS(llvm daslib llvm_jit)
	ADD_MODULE_DAS(llvm daslib llvm_jit_intrin)
	ADD_MODULE_DAS(llvm daslib llvm_jit_common)
	ADD_LIBRARY(libDasModuleLlvm ${DAS_LLVM_MODULE_SRC} ${DAS_LLVM_MODULE_PLATFORM_SRC})
	IF(APPLE)

		file(GLOB LLVM_LIB_SRC_A
			${LIBCLANG_LIB}/*.a
		)
		TARGET_LINK_LIBRARIES(libDasModuleLlvm ${LLVM_LIBRARIES}
			${LIBCLANG_LIB}/libLTO.dylib
			${LIBCLANG_LIB}/libRemarks.dylib
			${LLVM_LIB_SRC_A}
		)


		find_package(ZSTD REQUIRED)

		SET(LLVM_DIR ${PATH_TO_LIBCLANG}/LIB/CMAKE/LLVM)

		find_package(LLVM REQUIRED CONFIG)

		message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
		message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
		include_directories(${LLVM_INCLUDE_DIRS})
		add_definitions(${LLVM_DEFINITIONS})
		llvm_map_components_to_libnames(llvm_libs support core irreader)
		target_link_libraries(libDasModuleLlvm ${llvm_libs})

	ELSEIF(WIN32)
		TARGET_LINK_LIBRARIES(libDasModuleLlvm ${LLVM_LIBRARIES}
			${LIBCLANG_LIB}/LLVM-C.lib
			${LIBCLANG_LIB}/Remarks.lib
			${LIBCLANG_LIB}/LTO.lib
		)
	ENDIF()
	# ADD_DEPENDENCIES(libDasModuleLlvm)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleLlvm PUBLIC ${LLVM_INCLUDE_DIR} ${LIBCLANG_INCLUDE})


IF(WIN32)
	add_custom_command(TARGET libDasModuleLlvm POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	        "${PATH_TO_LIBCLANG}/bin/LLVM-C.dll"
    	    "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/LLVM-C.dll")
	add_custom_command(TARGET libDasModuleLlvm POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PATH_TO_LIBCLANG}/bin/LTO.dll"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/LTO.dll")
	add_custom_command(TARGET libDasModuleLlvm POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PATH_TO_LIBCLANG}/bin/Remarks.dll"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/Remarks.dll")
		add_custom_command(TARGET libDasModuleLlvm POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PATH_TO_LIBCLANG}/bin/libclang.dll"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/libclang.dll")
	set(DAS_CLANG_DLLS
		${PATH_TO_LIBCLANG}/bin/LLVM-C.dll
		${PATH_TO_LIBCLANG}/bin/LTO.dll
		${PATH_TO_LIBCLANG}/bin/Remarks.dll
		${PATH_TO_LIBCLANG}/bin/libclang.dll
	)
	install(FILES ${DAS_CLANG_DLLS}
        DESTINATION ${DAS_INSTALL_BINDIR}
    )
	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLVM/LICENSE.TXT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLVM.LICENSE)


    file(GLOB DASLLVM_SOURCES
		${DAS_LLVM_DIR}/daslib/*.das
    )
    install(FILES ${DASLLVM_SOURCES}
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLVM/daslib
    )
ENDIF()

    #ADD_MODULE_DAS(cbind cbind cbind_boost)

	SETUP_CPP11(libDasModuleLlvm)
ENDIF()
