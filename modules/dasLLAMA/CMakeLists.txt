IF(NOT DAS_LLAMA_INCLUDED)
    SET(DAS_LLAMA_INCLUDED TRUE)
    MESSAGE(STATUS "dasLLAMA module included.")

    ADD_MODULE_DAS(dasllama dasllama dasllama_math)
    ADD_MODULE_DAS(dasllama dasllama dasllama_math_default)
    ADD_MODULE_DAS(dasllama dasllama dasllama_math_aarch64_neon)
    ADD_MODULE_DAS(dasllama dasllama dasllama_quant)
    ADD_MODULE_DAS(dasllama dasllama dasllama_gguf)
    ADD_MODULE_DAS(dasllama dasllama dasllama_unicode)
    ADD_MODULE_DAS(dasllama dasllama dasllama_tokenizer)
    ADD_MODULE_DAS(dasllama dasllama dasllama_bpe)
    ADD_MODULE_DAS(dasllama dasllama dasllama_common)
    ADD_MODULE_DAS(dasllama dasllama dasllama_audio)
    ADD_MODULE_DAS(dasllama dasllama dasllama_audio_io)
    ADD_MODULE_DAS(dasllama dasllama dasllama_whisper)
    ADD_MODULE_DAS(dasllama dasllama dasllama_qwen3a)
    ADD_MODULE_DAS(dasllama dasllama dasllama_parakeet)
    ADD_MODULE_DAS(dasllama dasllama dasllama_canary)
    ADD_MODULE_DAS(dasllama dasllama dasllama_gemma4a)
    ADD_MODULE_DAS(dasllama dasllama dasllama_vad)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_llama)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen2)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen3)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_phi3)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_gemma2)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_gemma3)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_gemma4)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen2moe)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen3moe)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen35)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_gptoss)
    ADD_MODULE_DAS(dasllama dasllama dasllama_transformer)
    ADD_MODULE_DAS(dasllama dasllama dasllama_asr)
    ADD_MODULE_DAS(dasllama dasllama dasllama_chat)
    ADD_MODULE_DAS(dasllama dasllama dasllama)

    # AOT-able sources (for test_aot / any consumer); empty when disabled. Spans dasLLVM helpers
    # dasLLAMA's math kernels require, plus the dasllama module graph.
    SET(DASLLAMA_AOT_FILES
        modules/dasLLAMA/tests/_model_tier.das
        modules/dasLLVM/daslib/aarch64_neon.das
        modules/dasLLVM/daslib/x64_avx.das
        modules/dasLLVM/daslib/f16_cvt.das
        modules/dasLLAMA/dasllama/dasllama_math.das
        modules/dasLLAMA/dasllama/dasllama_math_default.das
        modules/dasLLAMA/dasllama/dasllama_math_aarch64_neon.das
        modules/dasLLAMA/dasllama/dasllama_gemm_schema.das
        modules/dasLLAMA/dasllama/dasllama_math_gen.das
        modules/dasLLAMA/dasllama/dasllama_quant.das
        modules/dasLLAMA/dasllama/dasllama_gguf.das
        modules/dasLLAMA/dasllama/dasllama_unicode.das
        modules/dasLLAMA/dasllama/dasllama_tokenizer.das
        modules/dasLLAMA/dasllama/dasllama_bpe.das
        modules/dasLLAMA/dasllama/dasllama_common.das
        modules/dasLLAMA/dasllama/dasllama_prefix.das
        modules/dasLLAMA/dasllama/dasllama_audio.das
        modules/dasLLAMA/dasllama/dasllama_whisper.das
        modules/dasLLAMA/dasllama/dasllama_qwen3a.das
        modules/dasLLAMA/dasllama/dasllama_parakeet.das
        modules/dasLLAMA/dasllama/dasllama_canary.das
        modules/dasLLAMA/dasllama/dasllama_gemma4a.das
        modules/dasLLAMA/dasllama/dasllama_vad.das
        modules/dasLLAMA/dasllama/dasllama_arch_llama.das
        modules/dasLLAMA/dasllama/dasllama_arch_qwen2.das
        modules/dasLLAMA/dasllama/dasllama_arch_qwen3.das
        modules/dasLLAMA/dasllama/dasllama_arch_phi3.das
        modules/dasLLAMA/dasllama/dasllama_arch_gemma2.das
        modules/dasLLAMA/dasllama/dasllama_arch_gemma3.das
        modules/dasLLAMA/dasllama/dasllama_arch_gemma4.das
        modules/dasLLAMA/dasllama/dasllama_arch_qwen2moe.das
        modules/dasLLAMA/dasllama/dasllama_arch_qwen3moe.das
        modules/dasLLAMA/dasllama/dasllama_arch_qwen35.das
        modules/dasLLAMA/dasllama/dasllama_arch_gptoss.das
        modules/dasLLAMA/dasllama/dasllama_arch_mistral3.das
        modules/dasLLAMA/dasllama/dasllama_arch_glm4moe.das
        modules/dasLLAMA/dasllama/dasllama_image.das
        modules/dasLLAMA/dasllama/dasllama_transformer.das
        modules/dasLLAMA/dasllama/dasllama_asr.das
        modules/dasLLAMA/dasllama/dasllama_chat.das
        modules/dasLLAMA/dasllama/dasllama.das
)
    FILE(GLOB DASLLAMA_AOT_DEPENDS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/dasLLAMA/dasllama/*.das")
    SET(DASLLAMA_AOT_DEPENDS "${DASLLAMA_AOT_DEPENDS}")

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/dasllama
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
        FILES_MATCHING
        PATTERN "*.das"
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
    )
    # checked-in Silero-VAD weights (MIT, see the LICENSE beside them) — ship with the SDK
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/models/silero_vad.bin
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/models/silero_vad.LICENSE
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA/models
    )
    # per-box tuning: the profile producer ([tuned] in the module is the consumer) + its doc
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/harness/tune_kernels.das
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/harness/tune_tb.das
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA/harness
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/tune_for_this_box.md
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
    )
ENDIF()
