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
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_llama)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen2)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_qwen3)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_phi3)
    ADD_MODULE_DAS(dasllama dasllama dasllama_arch_gemma2)
    ADD_MODULE_DAS(dasllama dasllama dasllama_transformer)
    ADD_MODULE_DAS(dasllama dasllama dasllama_chat)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/dasllama
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
        FILES_MATCHING
        PATTERN "*.das"
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
    )
ENDIF()
