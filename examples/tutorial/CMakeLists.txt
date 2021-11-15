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
tutorial00.cpp
)
DAS_TUTORIAL(tutorial00 "${TUTORIAL_00_SRC}")

##################################################
# tutorial 01 - all error checking, external files
##################################################
SET(TUTORIAL_01_SRC
tutorial01.cpp
tutorial01.das
)
DAS_TUTORIAL(tutorial01 "${TUTORIAL_01_SRC}")

##########################################
# tutorial 02 - module, function, constant
##########################################
SET(TUTORIAL_02_SRC
tutorial.inc
tutorial02.cpp
tutorial02.das
)
DAS_TUTORIAL(tutorial02 "${TUTORIAL_02_SRC}")

##########################################################
# tutorial 02 AOT - same as tutorial02, only setup for AOT
##########################################################

### 1. setup custom AOT utility
SET(TUTORIAL_02_DASAOT_MAIN_SRC
tutorial02_dasaot.cpp
tutorial02aot.h
tutorial02module.cpp
${PROJECT_SOURCE_DIR}/utils/daScript/main.cpp
)
add_executable(tutorial02_dasAot ${TUTORIAL_02_DASAOT_MAIN_SRC} )
target_compile_definitions(tutorial02_dasAot PUBLIC MAIN_FUNC_NAME=dascript_dummy_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_LINK_LIBRARIES(tutorial02_dasAot libDaScript libDaScriptProfile libDaScriptTest libDasModuleUriparser libUriParser Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(tutorial02_dasAot libDaScript libDaScriptProfile libDaScriptTest libDasModuleUriparser libUriParser ${DAS_MODULES_LIBS})
SETUP_CPP11(tutorial02_dasAot)
SETUP_LTO(tutorial02_dasAot)

### 2. setup aot target for the tutorial
add_custom_target(tutorial02_dasAotStub)
SET(TUTORIAL_02_AOT_GENERATED_SRC)
DAS_AOT("tutorial02.das" TUTORIAL_02_AOT_GENERATED_SRC tutorial02_dasAotStub tutorial02_dasAot)

### setup main tutorial executable
SET(TUTORIAL_02_AOT_SRC
tutorial02aot.cpp
tutorial02aot.h
tutorial02module.cpp
tutorial02.das
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
tutorial.inc
tutorial03.cpp
tutorial03.das
)
DAS_TUTORIAL(tutorial03 "${TUTORIAL_03_SRC}")

#################################
# tutorial 04 - C++ class adapter
#################################
SET(TUTORIAL_04_SRC
tutorial.inc
tutorial04.cpp
tutorial04.das
tutorial04module.das
tutorial04module.das.inc
)
DAS_TUTORIAL(tutorial04 "${TUTORIAL_04_SRC}")
target_include_directories(tutorial04 PUBLIC ${CMAKE_SOURCE_DIR}/src/builtin)   # we need RTTI to bind StructInfo and such
XXD(tutorial04module.das)

##########################
# tutorial 05 - coroutines
##########################
SET(TUTORIAL_05_SRC
tutorial.inc
tutorial05.cpp
tutorial05.das
)
DAS_TUTORIAL(tutorial05 "${TUTORIAL_05_SRC}")

#################################
# tutorial 06 - expression parser
#################################
SET(TUTORIAL_06_SRC
tutorial06.cpp
)
DAS_TUTORIAL(tutorial06 "${TUTORIAL_06_SRC}")

#################################
# tutorial 07 - "C" interface
#################################
SET(TUTORIAL_07_SRC
tutorial07.c
tutorial07.das
)
DAS_TUTORIAL(tutorial07 "${TUTORIAL_07_SRC}")
