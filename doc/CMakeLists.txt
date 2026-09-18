# Documentation targets. Added from the root via add_subdirectory(doc) after the
# daslang target exists (das2rst runs daslang). CMAKE_CURRENT_BINARY_DIR here is
# <build>/doc, so stamps and generated HTML land under build/doc/.

# Regenerate stdlib RST from daslib source comments. The HTML build lives in
# build_doc below; check_docs_fresh holds the guards that run once every
# generator (das2rst, imgui2rst, vulkan2rst) has written its pages.
if(NOT DAS_TOOLS_DISABLED)
    add_custom_target(check_docs_generated
        COMMAND $<TARGET_FILE:daslang> -documentation ${PROJECT_SOURCE_DIR}/doc/reflections/das2rst.das
                -- --digest ${CMAKE_CURRENT_BINARY_DIR}/everything.md
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Regenerating stdlib RST (das2rst)"
    )
    # The shipped digest is generated but tracked: skills/daslang ships to agents with no
    # daslang to generate it. So the lane checks, and updating the tracked copy is this target -
    # a copy of what the generator already wrote, never a second das2rst run to disagree with it.
    add_custom_target(regen_docs_digest
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/everything.md
                ${PROJECT_SOURCE_DIR}/skills/daslang/references/everything.md
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Updating the shipped module digest (skills/daslang/references/everything.md)"
    )
    add_custom_target(check_docs_fresh
        COMMAND ${CMAKE_COMMAND} -DDAS_ROOT=${PROJECT_SOURCE_DIR} -DDAS_DOC_BUILD=${CMAKE_CURRENT_BINARY_DIR}
                -P ${PROJECT_SOURCE_DIR}/doc/check_docs_fresh.cmake
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Checking generated documentation is fresh"
    )
    if(TARGET libDasModuleImgui)
        add_custom_target(run_imgui2rst
            COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/modules/dasImgui/utils/imgui2rst.das
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Regenerating the dasImgui stdlib RST (imgui2rst)"
        )
    endif()
    if(TARGET libDasModuleVulkan)
        add_custom_target(run_vulkan2rst
            COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/modules/dasVulkan/utils/vulkan2rst.das
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Regenerating the dasVulkan stdlib RST (vulkan2rst)"
        )
    endif()
endif()

# ── Documentation build (optional) ──────────────────────────────────────────
# Requires the `daslang` target (das2rst stdlib RST generator runs it). If the
# user explicitly asked for docs but also disabled tools, fail early rather
# than silently skipping — mirrors the DAS_AOT_EXAMPLES check in the root file.
if(DAS_BUILD_DOCUMENTATION)
    if(DAS_TOOLS_DISABLED)
        message(FATAL_ERROR "DAS_BUILD_DOCUMENTATION requires DAS_TOOLS_DISABLED to be OFF (the docs pipeline runs daslang via das2rst.das)")
    endif()
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_FOUND)
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c "import sphinx; print(sphinx.__version__)"
            RESULT_VARIABLE SPHINX_CHECK_RESULT
            OUTPUT_VARIABLE SPHINX_VERSION
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
    if(Python3_FOUND AND SPHINX_CHECK_RESULT EQUAL 0)
        message(STATUS "Sphinx ${SPHINX_VERSION} found — documentation build enabled")
        set(DAS_DOC_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/doc/source")
        set(DAS_DOC_BUILD_DIR   "${CMAKE_CURRENT_BINARY_DIR}/doc_html")
        set(DAS_DOC_DOCTREE_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc_doctrees")

        # Step 1: regenerate stdlib RST from daslib source comments (requires daslang)
        add_custom_command(
            OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            COMMAND $<TARGET_FILE:daslang> -documentation "${PROJECT_SOURCE_DIR}/doc/reflections/das2rst.das"
                    -- --digest ${CMAKE_CURRENT_BINARY_DIR}/everything.md
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating stdlib RST documentation (das2rst)"
        )

        # Step 2: build HTML with Sphinx
        add_custom_command(
            OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp
            COMMAND ${Python3_EXECUTABLE} -m sphinx
                    -b html
                    -d "${DAS_DOC_DOCTREE_DIR}"
                    "${DAS_DOC_SOURCE_DIR}"
                    "${DAS_DOC_BUILD_DIR}"
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
            COMMENT "Building HTML documentation (Sphinx)"
        )

        add_custom_target(das_documentation ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp
        )

        # CI doc lane target: warnings-as-errors (-W) HTML, mirroring doc.yml.
        # Explicit (non-ALL); das_documentation above stays the local /
        # install-oriented build. Both regenerate RST via the das2rst.stamp dep.
        add_custom_target(build_doc
            COMMAND ${Python3_EXECUTABLE} -m sphinx -W --keep-going
                    -b html -d "${DAS_DOC_DOCTREE_DIR}" "${DAS_DOC_SOURCE_DIR}" "${DAS_DOC_BUILD_DIR}"
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
            COMMENT "Building documentation HTML (Sphinx -W)"
        )

        install(DIRECTORY ${DAS_DOC_BUILD_DIR}/
            DESTINATION ${DAS_INSTALL_DOCDIR}/doc
            PATTERN ".buildinfo" EXCLUDE
            PATTERN ".doctrees" EXCLUDE
        )
    else()
        message(WARNING "DAS_BUILD_DOCUMENTATION is ON but Sphinx was not found. "
                        "Install Python 3 and run: pip install sphinx sphinx-rtd-theme")
    endif()
endif()

if(NOT DAS_TOOLS_DISABLED)
    add_custom_target(check_doc_verify
        COMMAND $<TARGET_FILE:daslang> ./utils/internal/doc-verify/main.das -- --daslang $<TARGET_FILE:daslang>
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Verifying authored-doc code blocks compile (doc-verify)"
    )

    add_custom_target(stage_doc_assets
        COMMAND bash ${PROJECT_SOURCE_DIR}/utils/internal/docs-assets/fetch.sh
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Staging the docs-assets tutorial recordings"
    )

    set(_doc_generators check_docs_generated stage_doc_assets)
    foreach(_t run_imgui2rst run_vulkan2rst)
        if(TARGET ${_t})
            list(APPEND _doc_generators ${_t})
        endif()
    endforeach()
    add_dependencies(check_docs_fresh ${_doc_generators})
    add_dependencies(regen_docs_digest check_docs_generated)
    # check_doc_verify walks doc/source, which the generators rewrite; it is a nightly member of
    # run_extended_all, so under run_all it would otherwise read a page mid-write
    add_dependencies(check_doc_verify ${_doc_generators})

    set(_doc_all ${_doc_generators} check_docs_fresh)
    if(TARGET build_doc)
        add_dependencies(build_doc check_docs_fresh)
        list(APPEND _doc_all build_doc)
    endif()
    add_custom_target(run_doc COMMENT "Running the doc lane")
    add_dependencies(run_doc ${_doc_all})
endif()
