# Freshness guards over the generated stdlib RST, run after every generator
# (das2rst, imgui2rst, vulkan2rst) has written its pages.
#   cmake -DDAS_ROOT=<tree> -P doc/check_docs_fresh.cmake
# Invoked by the check_docs_fresh target; doc.yml runs that target and nothing else.

set(_failed 0)

file(GLOB_RECURSE _handmade "${DAS_ROOT}/doc/source/stdlib/handmade/*")
foreach(_f IN LISTS _handmade)
    file(STRINGS "${_f}" _stub REGEX "// stub")
    if(_stub)
        message("ERROR: ${_f} carries a `// stub` entry. Please provide proper documentation.")
        set(_failed 1)
    endif()
endforeach()

file(GLOB_RECURSE _generated "${DAS_ROOT}/doc/source/stdlib/generated/*")
foreach(_f IN LISTS _generated)
    file(STRINGS "${_f}" _uncat REGEX "^Uncategorized$")
    if(_uncat)
        message("ERROR: ${_f} has an Uncategorized section.")
        message("Add the function(s) to a group_by_regex() in the generator that owns the page:")
        message("doc/reflections/das2rst.das, modules/dasImgui/utils/imgui2rst.das, or")
        message("modules/dasVulkan/utils/vulkan2rst.das.")
        set(_failed 1)
    endif()
endforeach()

execute_process(
    COMMAND git ls-files --others --exclude-standard doc/source/stdlib/
    WORKING_DIRECTORY "${DAS_ROOT}"
    OUTPUT_VARIABLE _untracked
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if(_untracked)
    message("ERROR: a generator wrote pages that are not tracked in git:")
    message("${_untracked}")
    message("Run the generators locally and commit the new files.")
    set(_failed 1)
endif()

# The shipped digest is generated but tracked: skills/daslang ships to agents with no daslang to
# generate it with. The generator wrote its copy to the build dir; the tree's copy must match.
# A digest carries the symbols of the modules its build loaded, so only the configuration the doc
# lane runs - DAS_BUILD_DOCUMENTATION, which it sets and an ordinary build does not - holds the
# reading the tracked copy is compared against.
if(DAS_DOC_BUILD AND DAS_DOC_GATE)
    file(SHA256 "${DAS_DOC_BUILD}/everything.md" _digest_built)
    file(SHA256 "${DAS_ROOT}/skills/daslang/references/everything.md" _digest_tracked)
    if(NOT _digest_built STREQUAL _digest_tracked)
        message("ERROR: skills/daslang/references/everything.md does not match what das2rst generates.")
        message("Update it: cmake --build <build> --target regen_docs_digest")
        set(_failed 1)
    endif()
endif()

if(_failed)
    message(FATAL_ERROR "generated documentation is not fresh")
endif()
message(STATUS "generated documentation is fresh")
