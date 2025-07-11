
IF (NOT DEFINED PATH_TO_LIBCLANG)
	SET(PATH_TO_LIBCLANG "${PROJECT_SOURCE_DIR}/../libclang")
ENDIF()
IF ((NOT ${DAS_CLANG_BIND_DISABLED}) OR (NOT DEFINED DAS_CLANG_BIND_DISABLED))
	IF(NOT IS_DIRECTORY ${PATH_TO_LIBCLANG})
		SET(DAS_LIBCLANG_DETECTED FALSE)
		message(WARNING "can't find libclang at ${PATH_TO_LIBCLANG}")
	ELSE()
		IF(FALSE) #("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			SET(DAS_LIBCLANG_DETECTED FALSE)
			message(WARNING "No libclang on CLANG/WIN for now.")
		ELSE()
			SET(DAS_LIBCLANG_DETECTED TRUE)
		ENDIF()
	ENDIF()
ENDIF()

IF ((NOT DAS_CLANG_BIND_INCLUDED) AND DAS_LIBCLANG_DETECTED AND ((NOT ${DAS_CLANG_BIND_DISABLED}) OR (NOT DEFINED DAS_CLANG_BIND_DISABLED)))
    SET(DAS_CLANG_BIND_INCLUDED TRUE)
    MESSAGE(STATUS "dasClangBind module included.")

	SET(DAS_CLANG_BIND_DIR ${PROJECT_SOURCE_DIR}/modules/dasClangBind)
	SET(LIBCLANG_INCLUDE ${PATH_TO_LIBCLANG}/include)
	SET(LIBCLANG_LIB ${PATH_TO_LIBCLANG}/lib)

	MESSAGE("LIBCLANG_INCLUDE at ${LIBCLANG_INCLUDE}")
	MESSAGE("LIBCLANG_LIB at ${LIBCLANG_LIB}")

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
	)

	ADD_MODULE_LIB(libDasModuleClangBind)
	ADD_MODULE_CPP(dasClangBind)
	# ADD_MODULE_NATIVE(CLANG_BIND_boost)
	ADD_LIBRARY(libDasModuleClangBind ${DAS_CLANG_BIND_MODULE_SRC} ${DAS_CLANG_BIND_MODULE_PLATFORM_SRC})
	IF(APPLE)
		TARGET_LINK_LIBRARIES(libDasModuleClangBind ${CLANG_BIND_LIBRARIES} ${LIBCLANG_LIB}/libclang.dylib)
	ELSEIF(UNIX)
		TARGET_LINK_LIBRARIES(libDasModuleClangBind ${CLANG_BIND_LIBRARIES} ${LIBCLANG_LIB}/libclang.so)
	ELSE()
		TARGET_LINK_LIBRARIES(libDasModuleClangBind ${CLANG_BIND_LIBRARIES} ${LIBCLANG_LIB}/libclang.lib)
	ENDIF()
	# ADD_DEPENDENCIES(libDasModuleClangBind)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleClangBind PUBLIC ${CLANG_BIND_INCLUDE_DIR} ${LIBCLANG_INCLUDE})

IF(WIN32)
	add_custom_command(TARGET libDasModuleClangBind POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	        "${PROJECT_SOURCE_DIR}/../libclang/bin/LLVM-C.dll"
    	    "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/LLVM-C.dll")
	add_custom_command(TARGET libDasModuleClangBind POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/../libclang/bin/LTO.dll"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/LTO.dll")
	add_custom_command(TARGET libDasModuleClangBind POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/../libclang/bin/Remarks.dll"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/Remarks.dll")
		add_custom_command(TARGET libDasModuleClangBind POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/../libclang/bin/libclang.dll"
		"${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/libclang.dll")
	set(DAS_CLANG_DLLS
		${PROJECT_SOURCE_DIR}/../libclang/bin/LLVM-C.dll
		${PROJECT_SOURCE_DIR}/../libclang/bin/LTO.dll
		${PROJECT_SOURCE_DIR}/../libclang/bin/Remarks.dll
		${PROJECT_SOURCE_DIR}/../libclang/bin/libclang.dll
	)
	install(FILES ${DAS_CLANG_DLLS}
        DESTINATION ${DAS_INSTALL_BINDIR}
    )
	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasClangBind/LICENSE.TXT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLVM.LICENSE)
ENDIF()

    ADD_MODULE_DAS(cbind cbind cbind_boost)

	SETUP_CPP11(libDasModuleClangBind)
ENDIF()
