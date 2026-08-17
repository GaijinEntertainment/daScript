# GLSL -> SPIR-V build helper for apps that author shaders in GLSL.
# dasVulkan's native path compiles daScript shaders (via dasSpirv) and ships
# no GLSL step, so DAS_VULKAN_COMPILE_SHADER fills that gap: the consuming
# app's CMake calls it per shader.  It drives the Vulkan SDK tools found on
# PATH; the module build itself never touches the SDK (vendored headers +
# volk), so including this file is inert -- only calling the macro needs the
# tools.
FIND_PROGRAM(DAS_VULKAN_GLSLANG_VALIDATOR_EXE glslangValidator)
FIND_PROGRAM(DAS_VULKAN_SPIRV_OPT_EXE spirv-opt)
FIND_PROGRAM(DAS_VULKAN_SPIRV_DIS_EXE spirv-dis)

# DAS_VULKAN_COMPILE_SHADER(<input> <extra_deps>
#     [OUTPUT_DIR <dir>]            # default: CMAKE_CURRENT_BINARY_DIR
#     [GLSLANG_PRE_ARGS <args...>]
#     [GLSLANG_ARGS <args...>]
#     [SPIRV_OPT_ARGS <args...>])
MACRO(DAS_VULKAN_COMPILE_SHADER input extra_deps)
    IF(NOT DAS_VULKAN_GLSLANG_VALIDATOR_EXE OR NOT DAS_VULKAN_SPIRV_OPT_EXE
            OR NOT DAS_VULKAN_SPIRV_DIS_EXE)
        MESSAGE(FATAL_ERROR
            "DAS_VULKAN_COMPILE_SHADER needs glslangValidator, spirv-opt "
            "and spirv-dis on PATH (the Vulkan SDK).")
    ENDIF()
    CMAKE_PARSE_ARGUMENTS(DS "" "OUTPUT_DIR"
        "GLSLANG_PRE_ARGS;GLSLANG_ARGS;SPIRV_OPT_ARGS" ${ARGN})
    get_filename_component(input_src ${input} ABSOLUTE)
    get_filename_component(input_dir ${input_src} DIRECTORY)
    get_filename_component(input_name_ext ${input} NAME)
    IF(DS_OUTPUT_DIR)
        SET(out_dir ${DS_OUTPUT_DIR})
    ELSE()
        SET(out_dir ${CMAKE_CURRENT_BINARY_DIR})
    ENDIF()
    # Preprocessed GLSL, kept for inspection.  glslangValidator -E
    # writes to stdout, so redirect it (hence no VERBATIM here).
    ADD_CUSTOM_COMMAND(
        DEPENDS ${input_src} ${extra_deps}
        OUTPUT ${out_dir}/${input_name_ext}.pre.glsl
        WORKING_DIRECTORY ${input_dir}
        USES_TERMINAL
        COMMAND_EXPAND_LISTS
        COMMAND ${DAS_VULKAN_GLSLANG_VALIDATOR_EXE} ${DS_GLSLANG_PRE_ARGS}
            ${input_name_ext} > ${out_dir}/${input_name_ext}.pre.glsl
        COMMENT "glslangValidator -E ${input} to ${input_name_ext}.pre.glsl")
    ADD_CUSTOM_COMMAND(
        DEPENDS ${input_src} ${extra_deps}
        OUTPUT ${out_dir}/${input_name_ext}.base.spv
        WORKING_DIRECTORY ${input_dir}
        VERBATIM
        USES_TERMINAL
        COMMAND_EXPAND_LISTS
        COMMAND ${DAS_VULKAN_GLSLANG_VALIDATOR_EXE} ${DS_GLSLANG_ARGS}
            ${input_name_ext} -o ${out_dir}/${input_name_ext}.base.spv
        COMMENT "glslangValidator ${input} to ${input_name_ext}.base.spv")
    ADD_CUSTOM_COMMAND(
        DEPENDS ${out_dir}/${input_name_ext}.base.spv
        OUTPUT ${out_dir}/${input_name_ext}.base.spvasm
        WORKING_DIRECTORY ${input_dir}
        VERBATIM
        COMMAND_EXPAND_LISTS
        COMMAND ${DAS_VULKAN_SPIRV_DIS_EXE} --nested-indent --comment
            --no-color ${out_dir}/${input_name_ext}.base.spv
            -o ${out_dir}/${input_name_ext}.base.spvasm
        COMMENT "spirv-dis ${input_name_ext}.base.spv")
    ADD_CUSTOM_COMMAND(
        DEPENDS ${out_dir}/${input_name_ext}.base.spv
        OUTPUT ${out_dir}/${input_name_ext}.spv
        WORKING_DIRECTORY ${input_dir}
        VERBATIM
        COMMAND_EXPAND_LISTS
        COMMAND ${DAS_VULKAN_SPIRV_OPT_EXE} ${DS_SPIRV_OPT_ARGS}
            ${out_dir}/${input_name_ext}.base.spv
            -o ${out_dir}/${input_name_ext}.spv
        COMMENT "spirv-opt to ${input_name_ext}.spv")
    ADD_CUSTOM_COMMAND(
        DEPENDS ${out_dir}/${input_name_ext}.spv
        OUTPUT ${out_dir}/${input_name_ext}.spvasm
        WORKING_DIRECTORY ${input_dir}
        VERBATIM
        COMMAND_EXPAND_LISTS
        COMMAND ${DAS_VULKAN_SPIRV_DIS_EXE} --nested-indent --comment
            --no-color ${out_dir}/${input_name_ext}.spv
            -o ${out_dir}/${input_name_ext}.spvasm
        COMMENT "spirv-dis ${input_name_ext}.spv")
ENDMACRO()
