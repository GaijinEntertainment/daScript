SET(PROFILE_MAIN_LIB_SRC
    examples/profile/test_profile.cpp
    examples/profile/test_profile.h
)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_LIB_SRC)

ADD_MODULE_LIB(libDaScriptProfile testProfile ${PROFILE_MAIN_LIB_SRC})
TARGET_LINK_LIBRARIES(libDaScriptProfile PUBLIC libDaScript)
SETUP_CPP11(libDaScriptProfile)

file(GLOB PROFILE_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/examples/profile/tests/*.das"
"${CMAKE_CURRENT_SOURCE_DIR}/examples/profile/extra_test/*.das"
"${CMAKE_CURRENT_SOURCE_DIR}/examples/profile/hash_test/*.das"
)
list(SORT PROFILE_SRC)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/profile/main.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/profile/test_profile.h
)
list(SORT PROFILE_MAIN_SRC)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_SRC)

### 1. setup custom AOT utility
add_custom_target(daScriptProfileAot)
SET(PROFILE_GENERATED_SRC)
# Use dynamic daslang, since we need to load `testProfile` module.
DAS_AOT("${PROFILE_SRC}" PROFILE_GENERATED_SRC daScriptProfileAot daslang_dyn)
SOURCE_GROUP_FILES("generated" PROFILE_GENERATED_SRC)

add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} ${PROFILE_GENERATED_SRC} ${DAS_DASCRIPT_MAIN_SRC})
target_compile_definitions(daScriptProfile PUBLIC MAIN_FUNC_NAME=dascript_app_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_INCLUDE_DIRECTORIES(daScriptProfile PUBLIC ${PROJECT_SOURCE_DIR}/examples/profile)
TARGET_LINK_LIBRARIES(daScriptProfile libDaScriptProfile libDaScriptAot Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(daScriptProfile libDaScriptProfile libDaScriptAot ${DAS_MODULES_LIBS})
SETUP_CPP11(daScriptProfile)
add_dependencies(daScriptProfile daScriptProfileAot)
#target_precompile_headers(daScriptProfile PUBLIC include/daScript/misc/platform.h)

