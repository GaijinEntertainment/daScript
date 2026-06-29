# Generate the dasImgui playground samples (furier, path_tracer_lab) from the canonical
# examples/graphics sources — no committed copies, single source of truth. Run at stage time
# (stage_site_playground), so they live only in the gitignored staged samples tree.
#
# The playground writes every sample file FLAT into the run cwd (basename only, see
# web/examples/ui/src/main.js), so path_tracer_lab's canonical cross-tree
#   require ../pathTracer/path_tracer.das
# is rewritten to a bare same-dir `require path_tracer`, and the header ships beside main.das.
# furier is single-file and copies verbatim.
#
# Inputs:  -DREPO=<repo root>  -DDEST=<staged samples/examples dir>
cmake_minimum_required(VERSION 3.16)

if(NOT REPO OR NOT DEST)
    message(FATAL_ERROR "stage_playground_imgui_samples: REPO and DEST must be set")
endif()

file(MAKE_DIRECTORY "${DEST}/path_tracer_lab")

# furier — single file, verbatim.
configure_file(
    "${REPO}/examples/graphics/furier_opengl_imgui_example.das"
    "${DEST}/furier.das"
    COPYONLY)

# path tracer header — verbatim, beside the flattened main so `require path_tracer` resolves.
configure_file(
    "${REPO}/examples/pathTracer/path_tracer.das"
    "${DEST}/path_tracer_lab/path_tracer.das"
    COPYONLY)

# path tracer main — rewrite the one cross-tree require to a same-dir require.
set(_pt_src "${REPO}/examples/graphics/path_tracer_lab_opengl_imgui_example.das")
file(READ "${_pt_src}" _pt_main)
string(FIND "${_pt_main}" "require ../pathTracer/path_tracer.das" _has_req)
if(_has_req EQUAL -1)
    # The canonical require moved/renamed — the rewrite would silently no-op and the playground
    # sample would fail to resolve the header. Fail loudly so it's caught at build, not at runtime.
    message(FATAL_ERROR
        "stage_playground_imgui_samples: '${_pt_src}' no longer contains "
        "'require ../pathTracer/path_tracer.das' — update the rewrite in this script.")
endif()
string(REPLACE
    "require ../pathTracer/path_tracer.das"
    "require path_tracer"
    _pt_main "${_pt_main}")
file(WRITE "${DEST}/path_tracer_lab/main.das" "${_pt_main}")

message(STATUS "Staged dasImgui playground samples (furier, path_tracer_lab) into ${DEST}")
