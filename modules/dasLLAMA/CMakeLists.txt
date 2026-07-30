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

    # dasLLAMA IS NEVER AOT'D — -jit only; no AOT source/depends exports (see tests/aot/CMakeLists.txt).

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
