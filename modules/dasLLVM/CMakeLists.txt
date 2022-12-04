
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
	)

	ADD_MODULE_LIB(libDasModuleLlvm)
	ADD_MODULE_CPP(dasLLVM)
	ADD_MODULE_DAS(llvm daslib llvm_boost)
	ADD_MODULE_DAS(llvm daslib llvm_jit)
	ADD_MODULE_DAS(llvm daslib llvm_jit_intrin)
	ADD_MODULE_DAS(llvm daslib llvm_jit_common)
	ADD_LIBRARY(libDasModuleLlvm ${DAS_LLVM_MODULE_SRC} ${DAS_LLVM_MODULE_PLATFORM_SRC})
	IF(APPLE)
		TARGET_LINK_LIBRARIES(libDasModuleLlvm ${LLVM_LIBRARIES}
			${LIBCLANG_LIB}/libLTO.dylib
			${LIBCLANG_LIB}/libRemarks.dylib

			${LIBCLANG_LIB}/libLLVMAggressiveInstCombine.a
			${LIBCLANG_LIB}/libLLVMAnalysis.a
			${LIBCLANG_LIB}/libLLVMAsmParser.a
			${LIBCLANG_LIB}/libLLVMAsmPrinter.a
			${LIBCLANG_LIB}/libLLVMBinaryFormat.a
			${LIBCLANG_LIB}/libLLVMBitReader.a
			${LIBCLANG_LIB}/libLLVMBitWriter.a
			${LIBCLANG_LIB}/libLLVMBitstreamReader.a
			${LIBCLANG_LIB}/libLLVMCFGuard.a
			${LIBCLANG_LIB}/libLLVMCFIVerify.a
			${LIBCLANG_LIB}/libLLVMCodeGen.a
			${LIBCLANG_LIB}/libLLVMCore.a
			${LIBCLANG_LIB}/libLLVMCoroutines.a
			${LIBCLANG_LIB}/libLLVMCoverage.a
			${LIBCLANG_LIB}/libLLVMDWARFLinker.a
			${LIBCLANG_LIB}/libLLVMDWP.a
			${LIBCLANG_LIB}/libLLVMDebugInfoCodeView.a
			${LIBCLANG_LIB}/libLLVMDebugInfoDWARF.a
			${LIBCLANG_LIB}/libLLVMDebugInfoGSYM.a
			${LIBCLANG_LIB}/libLLVMDebugInfoMSF.a
			${LIBCLANG_LIB}/libLLVMDebugInfoPDB.a
			${LIBCLANG_LIB}/libLLVMDemangle.a
			${LIBCLANG_LIB}/libLLVMDlltoolDriver.a
			${LIBCLANG_LIB}/libLLVMExecutionEngine.a
			${LIBCLANG_LIB}/libLLVMExegesis.a
			${LIBCLANG_LIB}/libLLVMExegesisX86.a
			${LIBCLANG_LIB}/libLLVMExtensions.a
			${LIBCLANG_LIB}/libLLVMFileCheck.a
			${LIBCLANG_LIB}/libLLVMFrontendOpenACC.a
			${LIBCLANG_LIB}/libLLVMFrontendOpenMP.a
			${LIBCLANG_LIB}/libLLVMFuzzMutate.a
			${LIBCLANG_LIB}/libLLVMGlobalISel.a
			${LIBCLANG_LIB}/libLLVMIRReader.a
			${LIBCLANG_LIB}/libLLVMInstCombine.a
			${LIBCLANG_LIB}/libLLVMInstrumentation.a
			${LIBCLANG_LIB}/libLLVMInterfaceStub.a
			${LIBCLANG_LIB}/libLLVMInterpreter.a
			${LIBCLANG_LIB}/libLLVMJITLink.a
			${LIBCLANG_LIB}/libLLVMLTO.a
			${LIBCLANG_LIB}/libLLVMLibDriver.a
			${LIBCLANG_LIB}/libLLVMLineEditor.a
			${LIBCLANG_LIB}/libLLVMLinker.a
			${LIBCLANG_LIB}/libLLVMMC.a
			${LIBCLANG_LIB}/libLLVMMCA.a
			${LIBCLANG_LIB}/libLLVMMCDisassembler.a
			${LIBCLANG_LIB}/libLLVMMCJIT.a
			${LIBCLANG_LIB}/libLLVMMCParser.a
			${LIBCLANG_LIB}/libLLVMMIRParser.a
			${LIBCLANG_LIB}/libLLVMObjCARCOpts.a
			${LIBCLANG_LIB}/libLLVMObject.a
			${LIBCLANG_LIB}/libLLVMObjectYAML.a
			${LIBCLANG_LIB}/libLLVMOption.a
			${LIBCLANG_LIB}/libLLVMOrcJIT.a
			${LIBCLANG_LIB}/libLLVMOrcShared.a
			${LIBCLANG_LIB}/libLLVMOrcTargetProcess.a
			${LIBCLANG_LIB}/libLLVMPasses.a
			${LIBCLANG_LIB}/libLLVMProfileData.a
			${LIBCLANG_LIB}/libLLVMRemarks.a
			${LIBCLANG_LIB}/libLLVMRuntimeDyld.a
			${LIBCLANG_LIB}/libLLVMScalarOpts.a
			${LIBCLANG_LIB}/libLLVMSelectionDAG.a
			${LIBCLANG_LIB}/libLLVMSupport.a
			${LIBCLANG_LIB}/libLLVMSymbolize.a
			${LIBCLANG_LIB}/libLLVMTableGen.a
			${LIBCLANG_LIB}/libLLVMTableGenGlobalISel.a
			${LIBCLANG_LIB}/libLLVMTarget.a
			${LIBCLANG_LIB}/libLLVMTextAPI.a
			${LIBCLANG_LIB}/libLLVMTransformUtils.a
			${LIBCLANG_LIB}/libLLVMVectorize.a
			${LIBCLANG_LIB}/libLLVMWindowsManifest.a
			${LIBCLANG_LIB}/libLLVMX86AsmParser.a
			${LIBCLANG_LIB}/libLLVMX86CodeGen.a
			${LIBCLANG_LIB}/libLLVMX86Desc.a
			${LIBCLANG_LIB}/libLLVMX86Disassembler.a
			${LIBCLANG_LIB}/libLLVMX86Info.a
			${LIBCLANG_LIB}/libLLVMXRay.a
			${LIBCLANG_LIB}/libLLVMipo.a
		)
	ELSEIF(WIN32)
		TARGET_LINK_LIBRARIES(libDasModuleLlvm ${LLVM_LIBRARIES}
			${LIBCLANG_LIB}/LLVM-C.lib
			${LIBCLANG_LIB}/Remarks.lib
			${LIBCLANG_LIB}/LTO.lib
		)
	ENDIF()
	# ADD_DEPENDENCIES(libDasModuleLlvm)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleLlvm PUBLIC ${LLVM_INCLUDE_DIR} ${LIBCLANG_INCLUDE})

    #ADD_MODULE_DAS(cbind cbind cbind_boost)

	SETUP_CPP11(libDasModuleLlvm)
ENDIF()
