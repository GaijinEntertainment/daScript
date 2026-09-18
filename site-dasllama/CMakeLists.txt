if(DAS_TOOLS_DISABLED)
    return()
endif()


set(_site_script ${CMAKE_CURRENT_BINARY_DIR}/check_dasllama_site.cmake)
file(WRITE ${_site_script} [==[
function(_run)
    execute_process(COMMAND ${ARGN} WORKING_DIRECTORY "${DAS_ROOT}" RESULT_VARIABLE _rc)
    if(NOT _rc EQUAL 0)
        string(REPLACE ";" " " _cmd "${ARGN}")
        message(FATAL_ERROR "failed (${_rc}): ${_cmd}")
    endif()
endfunction()
_run("${PYTHON}" site-dasllama/build_news.py --root site-dasllama)
_run(git diff --exit-code -- site-dasllama/index.html site-dasllama/stories.html site-dasllama/stories site-dasllama/feed.xml site-dasllama/sitemap.xml)
execute_process(
    COMMAND git ls-files --others --exclude-standard -- site-dasllama/stories
    WORKING_DIRECTORY "${DAS_ROOT}"
    OUTPUT_VARIABLE _untracked
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if(_untracked)
    message(FATAL_ERROR "story pages the generator wrote but nobody committed:\n${_untracked}")
endif()
_run("${PYTHON}" site-dasllama/test_metadata.py)
]==])

add_custom_target(check_dasllama_site
    COMMAND ${CMAKE_COMMAND} -DDAS_ROOT=${PROJECT_SOURCE_DIR} -DPYTHON=${DAS_PYTHON}
            -P ${_site_script}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "dasllama.io: the news region is regenerated and every page carries its metadata"
)
