
IF ((NOT DAS_SQLITE_INCLUDED) AND ((NOT ${DAS_SQLITE_DISABLED}) OR (NOT DEFINED DAS_SQLITE_DISABLED)))
    SET(DAS_SQLITE_INCLUDED TRUE)
    MESSAGE(STATUS "dasSQLITE module included.")

	SET(DAS_SQLITE_DIR ${PROJECT_SOURCE_DIR}/modules/dasSqlite)
	SET(SQLITE_INCLUDE_DIR ${DAS_SQLITE_DIR}/sqlite)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_SQLITE_DIR})

	# sqlite library
	SET(DAS_SQLITE_SRC
	  	${DAS_SQLITE_DIR}/sqlite/sqlite3.c
  		${DAS_SQLITE_DIR}/sqlite/sqlite3.h
	  	${DAS_SQLITE_DIR}/sqlite/sqlite3ext.h
	)

	# sqlite shell
	SET(DAS_SQLITE_SHELL_SRC
	  	${DAS_SQLITE_DIR}/sqlite/shell.c
	)

	SET(DAS_SQLITE_MODULE_SRC
		${DAS_SQLITE_DIR}/src/need_dasSQLITE.h
		${DAS_SQLITE_DIR}/src/dasSQLITE.h
		${DAS_SQLITE_DIR}/src/dasSQLITE.cpp
	# user include
		${DAS_SQLITE_DIR}/src/dasSQLITE.main.cpp
	# generated binding includes
		${DAS_SQLITE_DIR}/src/dasSQLITE.alias.add.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.dummy.add.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.enum.add.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.enum.class.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.enum.decl.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.enum.decl.cast.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.struct.add.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.struct.postadd.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.struct.class.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.struct.decl.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.struct.impl.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.struct.cast.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.func.reg.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.func.decl.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.func.aot.inc
		${DAS_SQLITE_DIR}/src/dasSQLITE.func.aot.decl.inc
	# generated functions
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_1.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_2.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_3.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_4.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_5.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_6.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_7.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_8.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_9.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_10.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_11.cpp
		${DAS_SQLITE_DIR}/src/dasSQLITE.func_12.cpp
	)

	ADD_DEFINITIONS(-DSQLITE_THREADSAFE=1)
	ADD_DEFINITIONS(-DSQLITE_ENABLE_COLUMN_METADATA=1)
	ADD_DEFINITIONS(-DSQLITE_ENABLE_STMT_SCANSTATUS=1)
	ADD_DEFINITIONS(-DSQLITE_ENABLE_SNAPSHOT=1)

	ADD_MODULE_LIB(libDasModuleSQLITE)
	ADD_MODULE_CPP(dasSQLITE)
	# ADD_MODULE_NATIVE(SQLITE_boost)
	ADD_LIBRARY(libDasModuleSQLITE ${DAS_SQLITE_MODULE_SRC} ${DAS_SQLITE_SRC})
	TARGET_INCLUDE_DIRECTORIES(libDasModuleSQLITE PUBLIC ${SQLITE_INCLUDE_DIR})
	SETUP_CPP11(libDasModuleSQLITE)

	add_executable(sqlite_shell ${DAS_SQLITE_SRC} ${DAS_SQLITE_SHELL_SRC})
	TARGET_INCLUDE_DIRECTORIES(libDasModuleSQLITE PUBLIC ${SQLITE_INCLUDE_DIR})
	SETUP_CPP11(sqlite_shell)
	SETUP_LTO(sqlite_shell)

    ADD_MODULE_DAS(sqlite daslib sqlite_boost)

ENDIF()
