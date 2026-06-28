# Documentation targets. Added from the root via add_subdirectory(doc) after the
# daslang target exists (das2rst runs daslang). CMAKE_CURRENT_BINARY_DIR here is
# <build>/doc, so stamps and generated HTML/LaTeX land under build/doc/.

# Regenerate stdlib RST from daslib source comments. The freshness guards
# (// stub / Uncategorized / untracked) stay inline in doc.yml. HTML/LaTeX build
# lives in build_doc below.
if(NOT DAS_TOOLS_DISABLED)
    add_custom_target(check_docs_generated
        COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/doc/reflections/das2rst.das
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Regenerating stdlib RST (das2rst)"
    )
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
            COMMAND $<TARGET_FILE:daslang> "${PROJECT_SOURCE_DIR}/doc/reflections/das2rst.das"
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

        # CI doc lane target: warnings-as-errors (-W) HTML + LaTeX, mirroring
        # doc.yml. Explicit (non-ALL); das_documentation above stays the local /
        # install-oriented build. Both regenerate RST via the das2rst.stamp dep.
        add_custom_target(build_doc
            COMMAND ${Python3_EXECUTABLE} -m sphinx -W --keep-going
                    -b html -d "${DAS_DOC_DOCTREE_DIR}" "${DAS_DOC_SOURCE_DIR}" "${DAS_DOC_BUILD_DIR}"
            COMMAND ${Python3_EXECUTABLE} -m sphinx -W --keep-going
                    -b latex -d "${DAS_DOC_DOCTREE_DIR}" "${DAS_DOC_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/doc_latex"
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
            COMMENT "Building documentation HTML + LaTeX (Sphinx -W)"
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
