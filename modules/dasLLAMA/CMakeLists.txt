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
    # the tune scope's tuner set: the tuner ([tune_scope(tuner = "../harness/dasllama_tuner.das")]
    # in dasllama_tune_scope.das - the auto policy spawns it on a box the shipped profile covers
    # only partly), both halves it runs (gen_tune_probe, tune_kernels), what they require from
    # harness/ and tests/, and the token-block racer. harness/ and tests/ ship only these, so the
    # arch-extract registration below names them file by file
    set(DAS_LLAMA_TUNER_FILES
        modules/dasLLAMA/harness/dasllama_tuner.das
        modules/dasLLAMA/harness/gen_tune_probe.das
        modules/dasLLAMA/harness/tune_kernels.das
        modules/dasLLAMA/harness/tuner_cli.das
        modules/dasLLAMA/harness/tuner_noise.das
        modules/dasLLAMA/harness/tuner_phrases.das
        modules/dasLLAMA/harness/_gen_probe_fixture.das
        modules/dasLLAMA/harness/tune_tb.das
        modules/dasLLAMA/tests/_kq_dot.das
        modules/dasLLAMA/tests/_compares.das)
    # dasllama/, the tuner set and the box probes carry [arch] citations; ship generated excerpts
    # of the cited sections (registry processed by the root CMakeLists)
    string(JOIN "," _das_llama_arch_scan modules/dasLLAMA/dasllama ${DAS_LLAMA_TUNER_FILES}
        modules/dasLLAMA/performance/box_ident.das)
    set_property(GLOBAL APPEND PROPERTY DAS_ARCH_EXTRACT_SPECS
        "${_das_llama_arch_scan}|modules/dasLLAMA|${DAS_INSTALL_MODULESDIR}/dasLLAMA")
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
    # the tuner set (above), each file under its own folder, + the tuning doc
    foreach(_f IN LISTS DAS_LLAMA_TUNER_FILES)
        get_filename_component(_fdir ${_f} DIRECTORY)
        get_filename_component(_fsub ${_fdir} NAME)
        install(FILES ${PROJECT_SOURCE_DIR}/${_f} DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA/${_fsub})
    endforeach()
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/tune_for_this_box.md
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLLAMA
    )
    # the ONE model set (provenance + serving-catalog rows) — dasllama-server's /catalog is a
    # view over it - and the engine-free box probes (CPU brand, RAM, WMI) the server's hardware
    # line and its checkpoint budget read; the bundle must carry both or the server dies at require
    install(FILES
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/performance/model_specs.das
        ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/performance/box_ident.das
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
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.GIGAAM DESTINATION ${DAS_INSTALL_DOCDIR} RENAME GIGAAM.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.SILERO DESTINATION ${DAS_INSTALL_DOCDIR} RENAME SILERO.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.LLAMA_CPP DESTINATION ${DAS_INSTALL_DOCDIR} RENAME LLAMA_CPP.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.VLLM DESTINATION ${DAS_INSTALL_DOCDIR} RENAME VLLM.LICENSE)
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLLAMA/LICENSE.MLX DESTINATION ${DAS_INSTALL_DOCDIR} RENAME MLX.LICENSE)
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
        COMMAND ${CMAKE_COMMAND} -E env PYTHONDONTWRITEBYTECODE=1 ${DAS_PYTHON} modules/dasLLAMA/harness/test_tts_ref_bench.py
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Testing the browser model-set mint, the Pocket converter predicates and the TTS reference bench's voice rules"
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
            utils/dasllama-server/test_openai_server_speech_multi.das
            utils/dasllama-server/test_openai_server_vision_multi.das
            utils/dasllama-server/test_openai_server_mtp.das
            utils/dasllama-server/test_openai_server_embed.das
            utils/dasllama-server/test_cli.das
        FILES
            utils/dasllama-server/test_cli_args.das
            utils/dasllama-server/test_exchange_client.das
            utils/dasllama-server/test_model_catalog.das
            utils/dasllama-server/test_catalog_download.das
            utils/dasllama-server/test_setup_mode.das
            utils/dasllama-server/test_user_state.das
            utils/dasllama-server/test_model_override.das
            utils/dasllama-server/test_slot_served.das
            utils/dasllama-server/test_server_flags.das
            utils/dasllama-server/test_cache_probe.das
            utils/dasllama-server/test_openai_server_translate.das
        JIT_FILES
            utils/dasllama-server/test_worker_dispatch.das
            utils/dasllama-server/test_constraints.das)
    
    das_lane_dastest_target(run_tests_dasllama_convert COMMENT "Testing dasllama-convert"
        JIT_FILES utils/dasllama-convert/test_convert.das)

    das_lane_dastest_target(run_tests_dasllama_env_registry COMMENT "Testing the dasLLAMA env-knob registry"
        FILES modules/dasLLAMA/tests/test_env_registry.das)

    # engine-free: the box tag and the box probes the bench and the server share
    das_lane_dastest_target(run_tests_dasllama_box_ident COMMENT "Testing the dasLLAMA box probes"
        FILES modules/dasLLAMA/tests/test_box_ident.das)

    das_lane_dastest_target(run_tests_dasllama_facade_lint COMMENT "Testing the dasllama facade lint"
        JIT_FILES
            modules/dasLLAMA/tests/failed_dasllama_lint_require.das
            modules/dasLLAMA/tests/failed_dasllama_lint_sidedoor.das
            modules/dasLLAMA/tests/test_dasllama_lint_escape.das
            modules/dasLLAMA/tests/test_dasllama_lint_contracts.das)
endif()
