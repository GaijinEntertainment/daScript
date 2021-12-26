
IF ((NOT DAS_SFML_INCLUDED) AND ((NOT ${DAS_SFML_DISABLED}) OR (NOT DEFINED DAS_SFML_DISABLED)))
    SET(DAS_SFML_INCLUDED TRUE)
    MESSAGE(STATUS "dasSFML module included.")

	SET(DAS_SFML_DIR ${PROJECT_SOURCE_DIR}/modules/dasSFML)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_SFML_DIR})

	include(ExternalProject)
	IF(APPLE)
		ExternalProject_Add(
			SFML
			SOURCE_DIR ${DAS_SFML_DIR}/SFML
			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DAS_SFML_DIR}/libsfml -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=false
			BINARY_DIR ${DAS_SFML_DIR}/libsfml/build
		)
	ELSE()
		ExternalProject_Add(
			SFML
			SOURCE_DIR ${DAS_SFML_DIR}/SFML
			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DAS_SFML_DIR}/libsfml -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_LINKER=${CMAKE_LINKER} -DBUILD_SHARED_LIBS=false
			BINARY_DIR ${DAS_SFML_DIR}/libsfml/build
		)
	ENDIF()

	# libDasModuleSFML
	SET(DAS_SFML_MODULE_SRC
		${DAS_SFML_DIR}/src/dasSFML.main.cpp
	)

	SET(DAS_SFML_BIND_DIR ${DAS_SFML_DIR})
	SET(DAS_SFML_BIND_MODULE_SRC
		${DAS_SFML_BIND_DIR}/src/need_dasSFML.h
		${DAS_SFML_BIND_DIR}/src/dasSFML.h
		${DAS_SFML_BIND_DIR}/src/dasSFML.cpp
	# user include
		${DAS_SFML_BIND_DIR}/src/dasSFML.main.cpp
	# generated binding includes
		${DAS_SFML_BIND_DIR}/src/dasSFML.alias.add.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.dummy.add.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.enum.add.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.enum.class.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.enum.decl.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.enum.decl.cast.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.struct.add.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.struct.postadd.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.struct.class.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.struct.decl.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.struct.impl.inc
		${DAS_SFML_BIND_DIR}/src/dasSFML.struct.cast.inc
	# generated functions
		${DAS_SFML_BIND_DIR}/src/dasSFML.func_1.cpp
		${DAS_SFML_BIND_DIR}/src/dasSFML.func_2.cpp
		${DAS_SFML_BIND_DIR}/src/dasSFML.func_3.cpp
		${DAS_SFML_BIND_DIR}/src/dasSFML.func_4.cpp
	)

	IF(APPLE)
		find_library(COCOA_LIBRARY Cocoa)
		find_library(IOKIT_LIBRARY IOKit)
		find_library(METAL_LIBRARY Metal)
		find_library(QUARTZCORE_LIBRARY QuartzCore)
		find_library(CARBON_LIBRARY Carbon)
		SET(SFML_LIBRARIES
			${DAS_SFML_DIR}/libsfml/lib/libsfml-system-s.a
			${DAS_SFML_DIR}/libsfml/lib/libsfml-window-s.a
			${DAS_SFML_DIR}/libsfml/lib/libsfml-audio-s.a
			${DAS_SFML_DIR}/libsfml/lib/libsfml-graphics-s.a
			${DAS_SFML_DIR}/libsfml/lib/libsfml-network-s.a
			${DAS_SFML_DIR}/libsfml/lib/freetype.framework
			${COCOA_LIBRARY}
			${IOKIT_LIBRARY}
			${METAL_LIBRARY}
			${QUARTZCORE_LIBRARY}
			${CARBON_LIBRARY}
		)
	ELSEIF(UNIX)
		SET(SFML_LIBRARIES
		${DAS_SFML_DIR}/libsfml/lib/sfml-main.a
		${DAS_SFML_DIR}/libsfml/lib/sfml-system-s.a
		${DAS_SFML_DIR}/libsfml/lib/sfml-window-s.a
		${DAS_SFML_DIR}/libsfml/lib/sfml-audio-s.a
		${DAS_SFML_DIR}/libsfml/lib/sfml-graphics-s.a
		${DAS_SFML_DIR}/libsfml/lib/sfml-network-s.a
		${DAS_SFML_DIR}/libsfml/lib/freetype.a
		)
	ELSE()
		SET(SFML_LIBRARIES
			${DAS_SFML_DIR}/libsfml/lib/sfml-main.lib
			${DAS_SFML_DIR}/libsfml/lib/sfml-system-s.lib
			${DAS_SFML_DIR}/libsfml/lib/sfml-window-s.lib
			${DAS_SFML_DIR}/libsfml/lib/sfml-audio-s.lib
			${DAS_SFML_DIR}/libsfml/lib/sfml-graphics-s.lib
			${DAS_SFML_DIR}/libsfml/lib/sfml-network-s.lib
			${DAS_SFML_DIR}/libsfml/lib/freetype.lib
			Winmm.lib
			Opengl32.lib
			legacy_stdio_definitions.lib
		)
	ENDIF()


	ADD_MODULE_LIB(libDasModuleSFML)
	ADD_MODULE_CPP(dasSFML)
	ADD_MODULE_DAS(sfml dassfml sfml_boost)
	ADD_LIBRARY(libDasModuleSFML ${DAS_SFML_BIND_MODULE_SRC} ${DAS_SFML_MODULE_PLATFORM_SRC})
	TARGET_LINK_LIBRARIES(libDasModuleSFML ${SFML_LIBRARIES})
	ADD_DEPENDENCIES(libDasModuleSFML SFML)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleSFML PUBLIC
		${DAS_SFML_DIR}/libsfml/include
		${DAS_SFML_DIR}/src
	)

	SETUP_CPP11(libDasModuleSFML)
ENDIF()
