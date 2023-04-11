IF (WIN32 AND ((NOT DAS_TORCH_INCLUDED) AND ((NOT ${DAS_TORCH_DISABLED}) OR (NOT DEFINED DAS_TORCH_DISABLED))))
    SET(DAS_TORCH_INCLUDED TRUE)
    MESSAGE(STATUS "dasTORCH module included.")

	SET(DAS_TORCH_DIR ${PROJECT_SOURCE_DIR}/modules/dasTORCH)
	SET(TORCH_INCLUDE_DIR ${DAS_TORCH_DIR}/TORCH)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_TORCH_DIR})

	include(ExternalProject)

	ExternalProject_Add(libtorch
		URL "https://download.pytorch.org/libtorch/cpu/libtorch-win-shared-with-deps-2.0.0%2Bcpu.zip"
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libtorch
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
	)

	# Set LibTorch_DIR to the directory containing the LibTorch CMake configuration file
	set(LibTorch_DIR ${CMAKE_CURRENT_BINARY_DIR}/libtorch/src/libtorch)
	set(LibTorch_INCLUDE_DIR ${LibTorch_DIR}/include)
	set(LibTorch_CSRC_INCLUDE_DIR ${LibTorch_DIR}/include/Torch/csrc/api/include)
	set(LibTorch_LIB_DIR ${LibTorch_DIR}/lib)

	set(LibTorch_LIBRARIES
		${LibTorch_LIB_DIR}/torch.lib
		${LibTorch_LIB_DIR}/torch_cpu.lib
		${LibTorch_LIB_DIR}/c10.lib
	)

	# find_package(Torch REQUIRED PATHS ${LibTorch_DIR})

	# libDasModuleTORCH
	SET(DAS_TORCH_MODULE_SRC
		${DAS_TORCH_DIR}/src/dasTORCH.h
		${DAS_TORCH_DIR}/src/dasTORCH.cpp
	)

	ADD_MODULE_LIB(libDasModuleTORCH)
	ADD_MODULE_CPP(TORCH)
	# ADD_MODULE_NATIVE(TORCH_boost)
	ADD_LIBRARY(libDasModuleTORCH ${DAS_TORCH_MODULE_SRC})
	TARGET_LINK_LIBRARIES(libDasModuleTORCH ${TORCH_LIBRARIES} ${LibTorch_LIBRARIES})
	ADD_DEPENDENCIES(libDasModuleTORCH libtorch)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleTORCH PUBLIC ${TORCH_INCLUDE_DIR} ${LibTorch_INCLUDE_DIR} ${LibTorch_CSRC_INCLUDE_DIR})
	SETUP_CPP11(libDasModuleTORCH)

	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/torch.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/c10.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/torch_cpu.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/uv.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/fbgemm.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/asmjit.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
	add_custom_command(TARGET libDasModuleTORCH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LibTorch_LIB_DIR}/libiomp5md.dll" ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)

ENDIF()
