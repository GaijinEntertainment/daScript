SET(PROFILE_MAIN_LIB_SRC
    examples/profile/test_profile.cpp
    examples/profile/test_profile.h
)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_LIB_SRC)

ADD_LIBRARY(libDaScriptProfile ${PROFILE_MAIN_LIB_SRC})
SETUP_CPP11(libDaScriptProfile)

file(GLOB PROFILE_SRC
"${CMAKE_SOURCE_DIR}/examples/profile/tests/*.das"
"${CMAKE_SOURCE_DIR}/examples/profile/hash_test/*.das"
)
list(SORT PROFILE_SRC)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
${CMAKE_SOURCE_DIR}/examples/profile/main.cpp
${CMAKE_SOURCE_DIR}/examples/profile/test_profile.h
)
list(SORT PROFILE_MAIN_SRC)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_SRC)

### 1. setup custom AOT utility
SET(PROFILER_DASAOT_MAIN_SRC
${CMAKE_SOURCE_DIR}/examples/profile/main_aot.cpp
${DAS_DASCRIPT_MAIN_SRC}
)
add_executable(profiler_dasAot ${PROFILER_DASAOT_MAIN_SRC} )
target_compile_definitions(profiler_dasAot PUBLIC MAIN_FUNC_NAME=dascript_dummy_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_LINK_LIBRARIES(profiler_dasAot libDaScript libDaScriptTest libDaScriptProfile Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(profiler_dasAot libDaScript libDaScriptTest libDaScriptProfile ${DAS_MODULES_LIBS})
SETUP_CPP11(profiler_dasAot)
SETUP_LTO(profiler_dasAot)

add_custom_target(daScriptProfileAot)
SET(PROFILE_GENERATED_SRC)
DAS_AOT_DIR("${PROFILE_SRC}" PROFILE_GENERATED_SRC daScriptProfileAot profiler_dasAot)
SOURCE_GROUP_FILES("generated" PROFILE_GENERATED_SRC)

add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} ${PROFILE_GENERATED_SRC} ${AOT_GENERATED_SRC} ${DAS_DASCRIPT_MAIN_SRC})
target_compile_definitions(daScriptProfile PUBLIC MAIN_FUNC_NAME=dascript_app_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_INCLUDE_DIRECTORIES(daScriptProfile PUBLIC ${PROJECT_SOURCE_DIR}/examples/profile)
TARGET_LINK_LIBRARIES(daScriptProfile libDaScript libDaScriptTest libDaScriptProfile Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(daScriptProfile libDaScript libDaScriptProfile ${DAS_MODULES_LIBS})
SETUP_CPP11(daScriptProfile)
add_dependencies(daScriptProfile daScriptProfileAot dasAotStub)
#target_precompile_headers(daScriptProfile PUBLIC include/daScript/misc/platform.h)

