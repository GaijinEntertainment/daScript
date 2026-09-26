IF(NOT DAS_LLAMA_INCLUDED)
    SET(DAS_LLAMA_INCLUDED TRUE)
    MESSAGE(STATUS "dasLLAMA module included.")

    ADD_MODULE_DAS_FROM_DESCRIPTOR(dasllama dasllama)

    # dasLLAMA's tests are never in test_aot (see tests/aot/CMakeLists.txt); the engine's AOT
    # tier is the opt-in examples/dasLLAMA target, which AOTs the CPU modules itself.

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/dasllama
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
        FILES_MATCHING
        PATTERN "*.das"
    )
    # dasllama/ carries [arch] citations; ship generated excerpts of the cited sections
    # (registry processed by the root CMakeLists)
    set_property(GLOBAL APPEND PROPERTY DAS_ARCH_EXTRACT_SPECS
        "modules/dasLLAMA/dasllama|modules/dasLLAMA|${DAS_INSTALL_MODULESDIR}/dasLLAMA")
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
    )
    # checked-in Silero-VAD weights (MIT, see the LICENSE beside them) — ship with the SDK
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/models/silero_vad.bin
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/models/silero_vad.LICENSE
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/models/jfk_ask_not.wav
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/models/jfk_ask_not.LICENSE
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
    # the ONE model set (provenance + serving-catalog rows) — dasllama-server's /catalog is a
    # view over it, so the bundle must carry it or the server dies at require
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/performance/model_specs.das
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA/performance
    )
    # shipped defaults profiles ([tune_scope(defaults = ...)]): an untuned SDK box adopts its
    # CPU class's winners instead of racing — the scope resolves this dir against the installed
    # dasllama_tune_scope.das, so the bundle must carry it
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/performance/defaults
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA/performance
        FILES_MATCHING
        PATTERN "*.tune-defaults.json"
    )
    # third-party notices: the ported reference implementations (MIT) + the weights terms
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.PARAKEET DESTINATION ${DAS_INSTALL_DOCDIR} RENAME PARAKEET.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.SILERO DESTINATION ${DAS_INSTALL_DOCDIR} RENAME SILERO.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.LLAMA_CPP DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLAMA_CPP.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.VLLM DESTINATION ${DAS_INSTALL_DOCDIR} RENAME VLLM.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.APACHE-2.0 DESTINATION ${DAS_INSTALL_DOCDIR} RENAME APACHE-2.0.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.STYLETTS2 DESTINATION ${DAS_INSTALL_DOCDIR} RENAME STYLETTS2.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.POCKET_TTS DESTINATION ${DAS_INSTALL_DOCDIR} RENAME POCKET_TTS.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.CMUDICT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME CMUDICT.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.SPACY DESTINATION ${DAS_INSTALL_DOCDIR} RENAME SPACY.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.UD_EWT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME UD_EWT.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.CC-BY-4.0 DESTINATION ${DAS_INSTALL_DOCDIR} RENAME CC-BY-4.0.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/THIRD_PARTY_NOTICES.md DESTINATION ${DAS_INSTALL_DOCDIR} RENAME DASLLAMA_THIRD_PARTY_NOTICES.md)
ENDIF()

if(NOT DAS_TOOLS_DISABLED)
    add_custom_target(check_model_mint
        COMMAND ${CMAKE_COMMAND} -E env PYTHONDONTWRITEBYTECODE=1 ${DAS_PYTHON} examples/dasLLAMA/wasm/test_mint_models.py
        COMMAND ${CMAKE_COMMAND} -E env PYTHONDONTWRITEBYTECODE=1 ${DAS_PYTHON} modules/dasLLAMA/harness/test_convert_pocket.py
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Testing the browser model-set mint and the Pocket converter predicates"
    )
endif()

if(NOT DAS_TOOLS_DISABLED)
    das_lane_dastest_target(run_tests_dasllama_server COMMENT "Testing dasllama-server"
        COMPILE_ONLY_FILES
            utils/dasllama-server/test_openai_server.das
            utils/dasllama-server/test_openai_server_think.das
            utils/dasllama-server/test_openai_server_stream.das
            utils/dasllama-server/test_openai_server_vision.das
            utils/dasllama-server/test_openai_server_audio.das
            utils/dasllama-server/test_openai_server_speech.das
            utils/dasllama-server/test_openai_server_mtp.das
            utils/dasllama-server/test_cli.das
        FILES
            utils/dasllama-server/test_cli_args.das
            utils/dasllama-server/test_exchange_client.das
            utils/dasllama-server/test_model_catalog.das
            utils/dasllama-server/test_setup_mode.das
            utils/dasllama-server/test_model_override.das
            utils/dasllama-server/test_slot_served.das
        JIT_FILES utils/dasllama-server/test_worker_dispatch.das)
    
    das_lane_dastest_target(run_tests_dasllama_env_registry COMMENT "Testing the dasLLAMA env-knob registry"
        FILES modules/dasLLAMA/tests/test_env_registry.das)
    
    das_lane_dastest_target(run_tests_dasllama_facade_lint COMMENT "Testing the dasllama facade lint"
        JIT_FILES
            modules/dasLLAMA/tests/failed_dasllama_lint_require.das
            modules/dasLLAMA/tests/failed_dasllama_lint_sidedoor.das
            modules/dasLLAMA/tests/test_dasllama_lint_escape.das
            modules/dasLLAMA/tests/test_dasllama_lint_contracts.das)
endif()
