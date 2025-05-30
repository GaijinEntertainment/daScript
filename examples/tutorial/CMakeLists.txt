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
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial00.cpp
)
DAS_TUTORIAL(tutorial00 "${TUTORIAL_00_SRC}")

####################
# tutorial 00 - utf8
####################
SET(TUTORIAL_00_UTF8_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial00utf8.cpp
)
DAS_TUTORIAL(tutorial00utf8 "${TUTORIAL_00_UTF8_SRC}")

##################################################
# tutorial 01 - all error checking, external files
##################################################
SET(TUTORIAL_01_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial01.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial01.das
)
DAS_TUTORIAL(tutorial01 "${TUTORIAL_01_SRC}")

##########################################
# tutorial 02 - module, function, constant
##########################################
SET(TUTORIAL_02_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02.das
)
DAS_TUTORIAL(tutorial02 "${TUTORIAL_02_SRC}")

##########################################################
# tutorial 02 AOT - same as tutorial02, only setup for AOT
##########################################################

### 1. setup custom AOT utility
SET(TUTORIAL_02_DASAOT_MAIN_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02_dasaot.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02aot.h
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02module.cpp
${DAS_DASCRIPT_MAIN_SRC}
)
add_executable(tutorial02_dasAot ${TUTORIAL_02_DASAOT_MAIN_SRC} )
target_compile_definitions(tutorial02_dasAot PUBLIC MAIN_FUNC_NAME=dascript_dummy_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_LINK_LIBRARIES(tutorial02_dasAot libDaScript libDaScriptTest Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(tutorial02_dasAot libDaScript libDaScriptTest ${DAS_MODULES_LIBS})
SETUP_CPP11(tutorial02_dasAot)
SETUP_LTO(tutorial02_dasAot)

### 2. setup aot target for the tutorial
add_custom_target(tutorial02_dasAotStub)
SET(TUTORIAL_02_AOT_GENERATED_SRC)
DAS_AOT("examples/tutorial/tutorial02.das" TUTORIAL_02_AOT_GENERATED_SRC tutorial02_dasAotStub tutorial02_dasAot)

### setup main tutorial executable
SET(TUTORIAL_02_AOT_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02aot.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02aot.h
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02module.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02.das
)
add_executable(tutorial02aot ${TUTORIAL_02_AOT_SRC} ${TUTORIAL_02_AOT_GENERATED_SRC})
target_include_directories(tutorial02aot PUBLIC ${PROJECT_SOURCE_DIR}/examples/tutorial)
TARGET_LINK_LIBRARIES(tutorial02aot libDaScript)
ADD_DEPENDENCIES(tutorial02aot libDaScript tutorial02_dasAotStub)
SETUP_CPP11(tutorial02aot)

## Example of standalone context
SET(TUTORIAL_02_STANDALONE_CTX_GENERATED_SRC)
DAS_AOT_STANDALONE("examples/tutorial/tutorial02.das" TUTORIAL_02_STANDALONE_CTX_GENERATED_SRC tutorial02_dasAotStub tutorial02_dasAot "aot")
add_executable(tutorial_02_standalone
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02_standalone_ctx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial02.das
        ${TUTORIAL_02_STANDALONE_CTX_GENERATED_SRC})
ADD_DEPENDENCIES(tutorial_02_standalone libDaScript tutorial02_dasAotStub)
target_include_directories(tutorial_02_standalone PUBLIC ${PROJECT_SOURCE_DIR}/examples/tutorial)
TARGET_LINK_LIBRARIES(tutorial_02_standalone libDaScript)
SETUP_CPP11(tutorial_02_standalone)

###########################
# tutorial 03 - custom type
###########################
SET(TUTORIAL_03_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial03.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial03.das
)
DAS_TUTORIAL(tutorial03 "${TUTORIAL_03_SRC}")

#################################
# tutorial 04 - C++ class adapter
#################################
SET(TUTORIAL_04_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial04.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial04.das
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial04module.das
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial04module.das.inc
)
DAS_TUTORIAL(tutorial04 "${TUTORIAL_04_SRC}")
target_include_directories(tutorial04 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/builtin)   # we need RTTI to bind StructInfo and such
CMAKE_TEXT_XXD(tutorial04 ${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial04module.das)
ADD_PROJECT_XXD_DEPENDS(tutorial04)

##########################
# tutorial 05 - coroutines
##########################
SET(TUTORIAL_05_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial.inc
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial05.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial05.das
)
DAS_TUTORIAL(tutorial05 "${TUTORIAL_05_SRC}")

#################################
# tutorial 06 - expression parser
#################################
SET(TUTORIAL_06_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial06.cpp
)
DAS_TUTORIAL(tutorial06 "${TUTORIAL_06_SRC}")

#################################
# tutorial 07 - "C" interface
#################################
SET(TUTORIAL_07_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial07.c
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial07.das
)
DAS_TUTORIAL(tutorial07 "${TUTORIAL_07_SRC}")

SET(TUTORIAL_07_UNALIGNED_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial07unaligned.c
)
DAS_TUTORIAL(tutorial07unaligned "${TUTORIAL_07_UNALIGNED_SRC}")

# "C" interface, loading from source file text
SET(TUTORIAL_07A_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial07a.c
)
DAS_TUTORIAL(tutorial07a "${TUTORIAL_07A_SRC}")

##########################################
# tutorial 08- serialization
##########################################
SET(TUTORIAL_08_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial08.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial08.das
)
DAS_TUTORIAL(tutorial08 "${TUTORIAL_08_SRC}")

##########################################
# tutorial 09- gc and gc roots
##########################################
SET(TUTORIAL_09_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial09.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorial/tutorial09.das
)
DAS_TUTORIAL(tutorial09 "${TUTORIAL_09_SRC}")
