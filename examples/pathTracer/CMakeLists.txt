### 1. setup custom AOT utility
SET(PATHTRACER_DASAOT_MAIN_SRC
${CMAKE_SOURCE_DIR}/examples/pathTracer/path_tracer_aot.cpp
${DAS_DASCRIPT_MAIN_SRC}
)
add_executable(pathtracer_dasAot ${PATHTRACER_DASAOT_MAIN_SRC} )
target_compile_definitions(pathtracer_dasAot PUBLIC MAIN_FUNC_NAME=dascript_dummy_main) # dascript_dummy_main is here to rename main function from daScript
TARGET_LINK_LIBRARIES(pathtracer_dasAot libDaScript libDaScriptProfile libDaScriptTest Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(pathtracer_dasAot libDaScript libDaScriptProfile libDaScriptTest ${DAS_MODULES_LIBS})
SETUP_CPP11(pathtracer_dasAot)
SETUP_LTO(pathtracer_dasAot)

### 2. setup aot target for the path tracer
add_custom_target(pathtracer_dasAotStub)
SET(PATHTRACER_AOT_GENERATED_SRC)
DAS_AOT("examples/pathTracer/path_tracer.das" PATHTRACER_AOT_GENERATED_SRC pathtracer_dasAotStub pathtracer_dasAot)

### setup main path tracer executable
SET(PATHTRACER_AOT_SRC
${CMAKE_SOURCE_DIR}/examples/pathTracer/path_tracer.cpp
${CMAKE_SOURCE_DIR}/examples/pathTracer/path_tracer.das
${CMAKE_SOURCE_DIR}/examples/pathTracer/toy_path_tracer_profile.das
)
add_executable(path_tracer ${PATHTRACER_AOT_SRC} ${PATHTRACER_AOT_GENERATED_SRC} ${AOT_GENERATED_SRC})
target_include_directories(path_tracer PUBLIC ${PROJECT_SOURCE_DIR}/examples/pathTracer)
TARGET_LINK_LIBRARIES(path_tracer libDaScript ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(path_tracer libDaScript pathtracer_dasAotStub)
SETUP_CPP11(path_tracer)
