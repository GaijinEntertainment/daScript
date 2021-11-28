file(GLOB PROFILE_SRC
"${CMAKE_SOURCE_DIR}/examples/profile/tests/*.das"
)
list(SORT PROFILE_SRC)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
${CMAKE_SOURCE_DIR}/examples/profile/main.cpp
${CMAKE_SOURCE_DIR}/examples/profile/test_profile.h
)
list(SORT PROFILE_MAIN_SRC)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_SRC)

add_custom_target(daScriptProfileAot)
SET(PROFILE_GENERATED_SRC)
DAS_AOT_DIR("${PROFILE_SRC}" PROFILE_GENERATED_SRC daScriptProfileAot daScript)
SOURCE_GROUP_FILES("generated" PROFILE_GENERATED_SRC)

add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} ${PROFILE_GENERATED_SRC} ${AOT_GENERATED_SRC})
TARGET_INCLUDE_DIRECTORIES(daScriptProfile PUBLIC ${PROJECT_SOURCE_DIR}/examples/profile)
TARGET_LINK_LIBRARIES(daScriptProfile libDaScript libDaScriptProfile Threads::Threads)
ADD_DEPENDENCIES(daScriptProfile libDaScript libDaScriptProfile)
SETUP_CPP11(daScriptProfile)
add_dependencies(daScriptProfile daScriptProfileAot dasAotStub)
#target_precompile_headers(daScriptProfile PUBLIC include/daScript/misc/platform.h)

