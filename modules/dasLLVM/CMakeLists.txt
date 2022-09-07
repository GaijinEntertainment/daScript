
SET(PATH_TO_LIBCLANG ${PROJECT_SOURCE_DIR}/../libclang)
IF(NOT IS_DIRECTORY ${PATH_TO_LIBCLANG})
	SET(DAS_LIBCLANG_DETECTED FALSE)
	message(WARNING "can't find libclang at ${PATH_TO_LIBCLANG}")
ELSE()
	IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
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
	#	${DAS_LLVM_DIR}/src/need_dasLlvm.h
		${DAS_LLVM_DIR}/src/dasLLVM.h
		${DAS_LLVM_DIR}/src/dasLLVM.cpp
#	# user include
#		${DAS_LLVM_DIR}/src/dasLlvm.main.cpp
#	# generated binding includes
#		${DAS_LLVM_DIR}/src/dasLlvm.alias.add.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.dummy.add.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.enum.add.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.enum.class.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.enum.decl.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.enum.decl.cast.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.struct.add.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.struct.postadd.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.struct.class.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.struct.decl.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.struct.impl.inc
#		${DAS_LLVM_DIR}/src/dasLlvm.struct.cast.inc
#	# generated functions
#		${DAS_LLVM_DIR}/src/dasLlvm.func_1.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_2.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_3.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_4.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_5.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_6.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_7.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_8.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_9.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_10.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_11.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_12.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_13.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_14.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_15.cpp
#		${DAS_LLVM_DIR}/src/dasLlvm.func_16.cpp
	)

	ADD_MODULE_LIB(libDasModuleLlvm)
	ADD_MODULE_CPP(dasLlvm)
	# ADD_MODULE_NATIVE(LLVM_boost)
	ADD_LIBRARY(libDasModuleLlvm ${DAS_LLVM_MODULE_SRC} ${DAS_LLVM_MODULE_PLATFORM_SRC})
	TARGET_LINK_LIBRARIES(libDasModuleLlvm ${LLVM_LIBRARIES} ${LIBCLANG_LIB}/libclang.lib)
	# ADD_DEPENDENCIES(libDasModuleLlvm)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleLlvm PUBLIC ${LLVM_INCLUDE_DIR} ${LIBCLANG_INCLUDE})

    #ADD_MODULE_DAS(cbind cbind cbind_boost)

	SETUP_CPP11(libDasModuleLlvm)
ENDIF()
