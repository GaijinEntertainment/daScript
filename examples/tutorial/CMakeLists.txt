MACRO(DAS_TUTORIAL tutorial_name tutorial_src)
    add_executable(${tutorial_name} ${tutorial_src})
    TARGET_LINK_LIBRARIES(${tutorial_name} libDaScript Threads::Threads)
    ADD_DEPENDENCIES(${tutorial_name} libDaScript)
    SETUP_CPP11(${tutorial_name})
ENDMACRO()

####################
# tutorial 00 - nano
####################
SET(TUTORIAL_00_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial00.cpp
)
DAS_TUTORIAL(tutorial00 "${TUTORIAL_00_SRC}")

##################################################
# tutorial 01 - all error checking, external files
##################################################
SET(TUTORIAL_01_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial01.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial01.das
)
DAS_TUTORIAL(tutorial01 "${TUTORIAL_01_SRC}")

##########################################
# tutorial 02 - module, function, constant
##########################################
SET(TUTORIAL_02_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02.das
)
DAS_TUTORIAL(tutorial02 "${TUTORIAL_02_SRC}")

##########################################################
# tutorial 02 AOT - same as tutorial02, only setup for AOT
##########################################################

### 1. setup custom AOT utility
SET(TUTORIAL_02_DASAOT_MAIN_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02_dasaot.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02aot.h
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02module.cpp
${PROJECT_SOURCE_DIR}/utils/daScript/main.cpp
)
add_executable(tutorial02_dasAot ${TUTORIAL_02_DASAOT_MAIN_SRC} )
target_compile_definitions(tutorial02_dasAot PUBLIC MAIN_FUNC_NAME=dascript_dummy_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_LINK_LIBRARIES(tutorial02_dasAot libDaScript libDaScriptProfile libDaScriptTest Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(tutorial02_dasAot libDaScript libDaScriptProfile libDaScriptTest ${DAS_MODULES_LIBS})
SETUP_CPP11(tutorial02_dasAot)
SETUP_LTO(tutorial02_dasAot)

### 2. setup aot target for the tutorial
add_custom_target(tutorial02_dasAotStub)
SET(TUTORIAL_02_AOT_GENERATED_SRC)
DAS_AOT("tutorial02.das" TUTORIAL_02_AOT_GENERATED_SRC tutorial02_dasAotStub tutorial02_dasAot)

### setup main tutorial executable
SET(TUTORIAL_02_AOT_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02aot.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02aot.h
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02module.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial02.das
)
add_executable(tutorial02aot ${TUTORIAL_02_AOT_SRC} ${TUTORIAL_02_AOT_GENERATED_SRC})
target_include_directories(tutorial02aot PUBLIC ${PROJECT_SOURCE_DIR}/examples/tutorial)
TARGET_LINK_LIBRARIES(tutorial02aot libDaScript)
ADD_DEPENDENCIES(tutorial02aot libDaScript tutorial02_dasAotStub)
SETUP_CPP11(tutorial02aot)

###########################
# tutorial 03 - custom type
###########################
SET(TUTORIAL_03_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial03.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial03.das
)
DAS_TUTORIAL(tutorial03 "${TUTORIAL_03_SRC}")

#################################
# tutorial 04 - C++ class adapter
#################################
SET(TUTORIAL_04_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial04.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial04.das
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial04module.das
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial04module.das.inc
)
DAS_TUTORIAL(tutorial04 "${TUTORIAL_04_SRC}")
target_include_directories(tutorial04 PUBLIC ${CMAKE_SOURCE_DIR}/src/builtin)   # we need RTTI to bind StructInfo and such
CMAKE_XXD(${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial04module.das "TEXT")

##########################
# tutorial 05 - coroutines
##########################
SET(TUTORIAL_05_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial05.cpp
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial05.das
)
DAS_TUTORIAL(tutorial05 "${TUTORIAL_05_SRC}")

#################################
# tutorial 06 - expression parser
#################################
SET(TUTORIAL_06_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial06.cpp
)
DAS_TUTORIAL(tutorial06 "${TUTORIAL_06_SRC}")

#################################
# tutorial 07 - "C" interface
#################################
SET(TUTORIAL_07_SRC
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial07.c
${CMAKE_SOURCE_DIR}/examples/tutorial/tutorial07.das
)
DAS_TUTORIAL(tutorial07 "${TUTORIAL_07_SRC}")
