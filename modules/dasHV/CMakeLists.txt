
IF ((NOT DAS_HV_INCLUDED) AND ((NOT ${DAS_HV_DISABLED}) OR (NOT DEFINED DAS_HV_DISABLED)))
    SET(DAS_HV_INCLUDED TRUE)
    MESSAGE(STATUS "dasHV module included.")

	SET(DAS_HV_DIR ${PROJECT_SOURCE_DIR}/modules/dasHV)

	include(ExternalProject)

	IF(WIN32)
		set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/openssl")
		find_package(OpenSSL QUIET PATHS ${OPENSSL_ROOT_DIR})
		IF(NOT OpenSSL_FOUND)
			# Build openssl from sources
			if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
				set(OPENSSL_ARCH "VC-WIN64A")
			else()
				set(OPENSSL_ARCH "VC-WIN32")
			endif()
			set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
			set(OPENSSL_LIBRARIES ${OPENSSL_ROOT_DIR}/lib)
			set(OPENSSL_LIBRARIES_FILES ${OPENSSL_ROOT_DIR}/lib/libcrypto.lib ${OPENSSL_ROOT_DIR}/lib/libssl.lib)

			# Note that under Ninja it should be run from VS developer code Prompt to setup all variables (or call vcvarsall.bat)
			ExternalProject_Add(openssl_build
				URL "https://www.openssl.org/source/openssl-3.5.1.tar.gz"
				URL_HASH SHA256=529043b15cffa5f36077a4d0af83f3de399807181d607441d734196d889b641f
				PREFIX ${OPENSSL_ROOT_DIR}
				CONFIGURE_COMMAND perl Configure ${OPENSSL_ARCH} no-shared --prefix=${OPENSSL_ROOT_DIR} --openssldir=${OPENSSL_ROOT_DIR}
				BUILD_COMMAND nmake
				INSTALL_COMMAND nmake install_sw
				BUILD_IN_SOURCE 1
				BUILD_BYPRODUCTS ${OPENSSL_LIBRARIES_FILES}
			)
			list(APPEND OPENSSL_LIBRARIES_FILES Crypt32.lib)
			SET(OPENSSL_FROM_SOURCES TRUE)
		ELSE()
			SET(OPENSSL_LIBRARIES_FILES OpenSSL::Crypto OpenSSL::SSL)
		ENDIF()

		SET(HV_LIBRARIES ${DAS_HV_DIR}/hv/lib/hv_static.lib)
	ELSE()
		find_package(OpenSSL REQUIRED)
		SET(HV_LIBRARIES ${DAS_HV_DIR}/hv/lib/libhv_static.a)
		SET(OPENSSL_LIBRARIES_FILES OpenSSL::Crypto OpenSSL::SSL)
	ENDIF()


	SET(HV_CMAKE_FLAGS
		-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
		-DCMAKE_INSTALL_PREFIX=${DAS_HV_DIR}/hv
		-DCMAKE_BUILD_TYPE=Release
		-DBUILD_EXAMPLES=OFF
		-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON
		-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
		-DCMAKE_LINKER=${CMAKE_LINKER}
		-DWITH_OPENSSL=ON
		-DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
		-DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
		-DOPENSSL_LIBRARIES="${OPENSSL_CRYPTO_LIBRARY};${OPENSSL_SSL_LIBRARY}"
	)
	IF(DAS_USE_SANITIZER STREQUAL "address" OR DAS_USE_SANITIZER STREQUAL "asan")
		LIST(APPEND HV_CMAKE_FLAGS
			"-DCMAKE_C_FLAGS=/fsanitize=address"
			"-DCMAKE_CXX_FLAGS=/fsanitize=address"
		)
	ENDIF()
	ExternalProject_Add(
		LIBHV
		URL https://github.com/ithewei/libhv/archive/refs/tags/v1.3.4.tar.gz
		URL_HASH SHA256=f0a9a197f90da55cc3ff104f9c7a27cc927f117b6c18613c3292726068588e10
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libhv
		CMAKE_ARGS ${HV_CMAKE_FLAGS}
		BUILD_BYPRODUCTS ${HV_LIBRARIES}
		BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
		INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
	)
	# Copy cert files from downloaded libhv source (not stored in repo to avoid pushing private keys)
	SET(LIBHV_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/libhv/src/LIBHV)
	ExternalProject_Add_Step(LIBHV copy_certs
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBHV_SRC_DIR}/cert ${DAS_HV_DIR}/cert
		DEPENDEES install
		COMMENT "Copying libhv cert files to ${DAS_HV_DIR}/cert"
	)
	IF(OPENSSL_FROM_SOURCES)
		add_dependencies(LIBHV openssl_build)
	ENDIF()

	IF(APPLE)
		list(APPEND HV_LIBRARIES
			"-framework CoreFoundation"
			"-framework Security"
		)
	ENDIF()

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_HV_DIR})

	# libDasModuleHV
	SET(DAS_HV_MODULE_SRC
		${DAS_HV_DIR}/src/dasHV.h
		${DAS_HV_DIR}/src/dasHV.cpp
	)

	ADD_MODULE_CPP(HV)
	ADD_MODULE_LIB(libDasModuleHV dasModuleHV ${DAS_HV_MODULE_SRC} ${DAS_HV_MODULE_PLATFORM_SRC})
	MACRO(SETUP_HV lib)
		TARGET_INCLUDE_DIRECTORIES(${lib} PRIVATE "${DAS_HV_DIR}/hv/include")
		TARGET_LINK_LIBRARIES(${lib} PRIVATE ${HV_LIBRARIES} ${OPENSSL_LIBRARIES_FILES})
		ADD_DEPENDENCIES(${lib} LIBHV)
		SETUP_CPP11(${lib})
	ENDMACRO()
	SETUP_HV(libDasModuleHV)
	SETUP_HV(dasModuleHV)

	ADD_MODULE_DAS(dashv dashv dashv_boost)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasHV/dashv
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasHV
        FILES_MATCHING
        PATTERN "*.das"
    )

	install(DIRECTORY ${DAS_HV_DIR}/cert
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasHV
        FILES_MATCHING
        PATTERN "*.crt"
    )

	install(DIRECTORY ${DAS_HV_DIR}/cert
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasHV
        FILES_MATCHING
        PATTERN "*.key"
    )

	install(FILES ${DAS_HV_DIR}/HV.LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME HV.LICENSE)

ENDIF()
