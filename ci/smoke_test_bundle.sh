#!/usr/bin/env bash
# Verify that every utility in a daslang release bundle at least launches.
#
# Designed to catch install-rule regressions like the v0.6.2-RC3
# `utils/mcp/cpp_search_config.das` miss, where the README-documented
# launch (`daslang.exe utils/mcp/main.das`) failed at parse time because
# a top-level peer of `main.das` was omitted from CMake's install(FILES …)
# list.
#
# Usage:  bash ci/smoke_test_bundle.sh <bundle-root>
#
# <bundle-root> is the directory produced by `cmake --install … --prefix <dir>`
# (contains bin/, daslib/, utils/, modules/, …).
#
# Exit 0 if every check passes; non-zero with a per-check failure log otherwise.

set -u

if [[ $# -lt 1 ]]; then
    echo "usage: $0 <bundle-root>" >&2
    exit 2
fi

# Canonicalize to absolute up front. The script `cd "$BUNDLE"` later, so any
# relative path baked into $DASLANG / exe paths would resolve from the wrong
# cwd and every check would fail with command-not-found.
BUNDLE="$(cd "$1" 2>/dev/null && pwd -P)" \
    || { echo "ERROR: bundle dir not found or not enterable: $1" >&2; exit 2; }

# Resolved BEFORE the `cd "$BUNDLE"` below: the shipped-skill checker lives beside
# this script in the repo, which is not part of the bundle.
CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

# Two exe naming conventions exist:
#   * CPP_SUFFIX — for binaries built via cmake `add_executable` (daslang,
#     daslang-live): platform-natural suffix (.exe on Windows,
#     none on Linux/macOS).
#   * TOOL_EXE_SUFFIX — the shipped tool exes (DAS_UTILS_SHIPPED_EXES in
#     utils/CMakeLists.txt): ALWAYS `.exe` on every platform.
# Use `-f` (file-exists) rather than `-x` (executable bit) — Windows Git-Bash
# doesn't see the +x bit on Linux ELF binaries even when this script runs
# locally on a Linux-bundle for cross-platform repro.
if [[ -f "$BUNDLE/bin/daslang.exe" ]]; then
    DASLANG="$BUNDLE/bin/daslang.exe"
    CPP_SUFFIX=".exe"
elif [[ -f "$BUNDLE/bin/daslang" ]]; then
    DASLANG="$BUNDLE/bin/daslang"
    CPP_SUFFIX=""
else
    echo "ERROR: daslang binary not found in $BUNDLE/bin/" >&2
    exit 2
fi
TOOL_EXE_SUFFIX=".exe"

cd "$BUNDLE"

# Source-based tests — `daslang -compile-only` parses + infers + links requires
# without executing. This is what catches install-rule misses: a missing peer
# `.das` file surfaces as `error[20605]: missing prerequisite …`.
#
# Add a row when a new util ships a .das entry point — every shipped tool keeps
# its source form in the bundle, exe or not. The list is intentionally explicit
# (no glob) so removing a util is a deliberate one-line change.
# EVERY installed .das entry point belongs here. The list was previously missing
# five of them, which is how `utils/mcp/setup.das` shipped absent from the bundle
# through 0.6.4 while both the installed README and skills/mcp_tools.md documented
# running it: nothing ever compiled it, so nothing noticed.
COMPILE_TESTS=(
    "aot|utils/aot/main.das"
    "aot-llvm|utils/aot/main_llvm_aot.das"
    "benchctl|utils/benchctl/main.das"
    "dap|utils/dap/main.das"
    "das-fmt|utils/das-fmt/dasfmt.das"
    "dascov|utils/dascov/main.das"
    "dasllama-convert|utils/dasllama-convert/main.das"
    "dasllama-server|utils/dasllama-server/main.das"
    "dasllama-cli|utils/dasllama-server/cli.das"
    "dasllama-server-bench|utils/dasllama-server/server_bench.das"
    "daspkg|utils/daspkg/main.das"
    "detect-dupe|utils/detect-dupe/main.das"
    "fix-lint-errors|utils/fix-lint-errors/main.das"
    "gen1-to-gen2|utils/gen1-to-gen2/main.das"
    "jobque-timeline|utils/jobque-timeline/main.das"
    "lint|utils/lint/main.das"
    "lsp-nav|utils/lsp/subtools/nav.das"
    "lsp-validate|utils/lsp/subtools/validate.das"
    "mcp|utils/mcp/main.das"
    "mcp-cpp|utils/mcp/cpp_main.das"
    "mcp-setup|utils/mcp/setup.das"
    "tools|utils/tools/main.das"
    "watchdog-das|utils/watchdog/main.das"
    # Not an entry point, but the library an adopting repo's REVIEW.das requires
    # (REVIEW_COMMON.md contract) — a bundle where it does not compile breaks
    # every external gate.
    "review-gate|dastest/review_gate.das"
    # In-tree module das layers — these catch the missing-payload class (the
    # dasImgui merge shipped binaries + descriptor but zero .das for a while:
    # the descriptor resolved to files the bundle did not carry).
    "imgui-example|modules/dasImgui/examples/features/button_repeat.das"
    # A third field names the module the row needs; a bundle without that module
    # skips the row (dasVulkan is off by default on Apple: the root CMakeLists).
    "vulkan-example|modules/dasVulkan/examples/smoke.das|dasVulkan"
    "vulkan-tutorial|modules/dasVulkan/tutorials/01_triangle/triangle_tut.das|dasVulkan"
)

# Tools intentionally NOT in COMPILE_TESTS:
#   find-dupe — require chain needs the `anthropic/anthropic` daspkg package
#               fetched at runtime + ANTHROPIC_API_KEY.

# Prebuilt exes in bin/: `cpp` rows are add_executable targets (platform suffix), presence-checked;
# `tool` rows are DAS_UTILS_SHIPPED_EXES (always `.exe`), also launched (`--help`, exit 0) - a copy
# loads the runtime through daslang's rpath, and an rpath into the build tree is dead on a user's box.
SHIPPED_EXE_TESTS=(
    "daslang-live|cpp"
    "watchdog|cpp"
    "benchctl|tool"
    "dascov|tool"
    "das-fmt|tool"
    "daspkg|tool"
    "dastest|tool"
    "detect-dupe|tool"
    "lint|tool"
)

PASS=0
FAIL=0
LOG="$(mktemp)"

# The build tree's lib/ (CMAKE_LIBRARY_OUTPUT_DIRECTORY, beside ci/) is also on every
# tool copy's rpath, so on the runner that built the bundle a build-tree rpath resolves
# and the launch check would pass without the bundle-relative entry. Hide it for the
# run; Windows has no rpath and locks open DLL dirs, so only POSIX.
BUILD_LIB="$(cd "$CI_DIR/.." && pwd -P)/lib"
HIDDEN_LIB=""
if [[ -z "$CPP_SUFFIX" && -d "$BUILD_LIB" ]]; then
    HIDDEN_LIB="$BUILD_LIB.smokehidden"
    mv "$BUILD_LIB" "$HIDDEN_LIB"
fi
restore_and_clean() {
    rm -f "$LOG" "$LOG.suite" "$LOG.lint"
    if [[ -n "$HIDDEN_LIB" && -d "$HIDDEN_LIB" ]]; then
        # A concurrent build may have recreated lib/ meanwhile; mv into an existing
        # dir would NEST the hidden copy instead of restoring it - merge then.
        if [[ -e "$BUILD_LIB" ]]; then
            cp -a "$HIDDEN_LIB"/. "$BUILD_LIB"/ && rm -rf "$HIDDEN_LIB"
        else
            mv "$HIDDEN_LIB" "$BUILD_LIB"
        fi
    fi
}
trap restore_and_clean EXIT

run_check() {
    local label="$1"; shift
    printf '  %-30s ' "$label"
    if "$@" > "$LOG" 2>&1; then
        echo "OK"
        PASS=$((PASS + 1))
    else
        local rc=$?
        echo "FAIL (exit $rc)"
        sed 's/^/      /' "$LOG"
        FAIL=$((FAIL + 1))
    fi
}

echo "==================================================================="
echo "daslang bundle smoke test"
echo "  bundle : $BUNDLE"
echo "  daslang: $DASLANG"
echo "==================================================================="

echo
echo "Source compile (-compile-only):"
for entry in "${COMPILE_TESTS[@]}"; do
    name="${entry%%|*}"
    rest="${entry#*|}"
    path="${rest%%|*}"
    mod=""
    [[ "$rest" == *"|"* ]] && mod="${rest#*|}"
    if [[ -n "$mod" && ! -d "$BUNDLE/modules/$mod" ]]; then
        printf '  %-30s SKIP (modules/%s is not in this bundle)\n' "$name" "$mod"
        continue
    fi
    run_check "$name" "$DASLANG" -compile-only "$path"
done

echo
echo "Prebuilt exes (bin/) - presence, tool rows also launched:"
for entry in "${SHIPPED_EXE_TESTS[@]}"; do
    name="${entry%%|*}"
    kind="${entry#*|}"
    case "$kind" in
        tool) suffix="$TOOL_EXE_SUFFIX" ;;
        cpp)    suffix="$CPP_SUFFIX" ;;
        *)      echo "ERROR: unknown kind '$kind' for $name" >&2; FAIL=$((FAIL + 1)); continue ;;
    esac
    exe="$BUNDLE/bin/${name}${suffix}"
    if [[ ! -f "$exe" ]]; then
        printf '  %-30s MISSING (%s)\n' "$name" "$exe"
        FAIL=$((FAIL + 1))
    elif [[ "$kind" == tool ]]; then
        run_check "$name (launch)" "$exe" --help
    else
        printf '  %-30s OK\n' "$name"
        PASS=$((PASS + 1))
    fi
done

echo
echo "Shipped headers' includes:"
# A shipped header that includes a file the install rule leaves out compiles in the tree and
# fails in every SDK consumer: every `#include "daScript/..."` / `<daScript/...>` written by a
# shipped header or .inc must resolve inside the bundle's include/.
include_misses=0
include_count=0
while IFS= read -r inc; do
    include_count=$((include_count + 1))
    if [[ ! -f "$BUNDLE/include/$inc" ]]; then
        header="$(grep -rlE "#include[[:space:]]*[<\"]$inc[>\"]" "$BUNDLE/include/daScript" | head -1)"
        printf '  %-52s MISSING (included by %s)\n' "include/$inc" "${header#"$BUNDLE/"}"
        include_misses=$((include_misses + 1))
    fi
done < <(grep -rhoE '#include[[:space:]]*[<"]daScript/[^">]+[">]' "$BUNDLE/include/daScript" \
         | sed -E 's/.*[<"](daScript[^">]+)[">].*/\1/' | sort -u)
if [[ "$include_count" -gt 0 && "$include_misses" -eq 0 ]]; then
    printf '  %-52s OK (%d distinct includes resolve)\n' "include/daScript/**" "$include_count"
    PASS=$((PASS + 1))
else
    [[ "$include_count" -eq 0 ]] && printf '  %-52s MISSING (no shipped header found)\n' "include/daScript/**"
    FAIL=$((FAIL + 1))
fi

echo
echo "Runtime launch:"
# A stdio JSON-RPC server (the MCP server and the DAP bridge run from source, the watchdog's
# LSP front in the static exe) is started on empty stdin: a clean exit is the only safe
# did-it-start probe. The watchdog's own help is captured, not piped, so its exit code counts.
run_check "mcp.das (empty stdin)" bash -c \
    "'$DASLANG' utils/mcp/main.das < /dev/null"
run_check "dap.das (empty stdin)" bash -c \
    "'$DASLANG' utils/dap/main.das < /dev/null"
run_check "watchdog --help" bash -c \
    "out=\"\$('$BUNDLE/bin/watchdog${CPP_SUFFIX}' --help)\" && printf '%s' \"\$out\" | grep -q -- '--stable-seconds'"
run_check "watchdog --lsp (empty stdin)" bash -c \
    "'$BUNDLE/bin/watchdog${CPP_SUFFIX}' --lsp < /dev/null"
# a `-tool` front exits 2 without a daslang beside the watchdog, so exit 0 proves the bundle layout
for front in lsp mcp dap; do
    run_check "watchdog -tool $front (empty stdin)" bash -c \
        "'$BUNDLE/bin/watchdog${CPP_SUFFIX}' -tool $front < /dev/null"
done
run_check "daslang -tool lists the tools" bash -c \
    "'$DASLANG' -tool | grep -qx '    lint'"
# lint.exe must resolve the project's own modules/ from its cwd - a missed module reports main.das skipped
PROJECT="$(mktemp -d)"
mkdir -p "$PROJECT/modules/greeter"
printf 'options gen2\nrequire daslib/fio\n[export]\ndef initialize(project_path : string) {\n    register_native_path("greeter", "greeter", "{project_path}/greeter.das")\n}\n' \
    > "$PROJECT/modules/greeter/.das_module"
printf 'options gen2\nmodule greeter public\ndef public greet() : string => "hello"\n' > "$PROJECT/modules/greeter/greeter.das"
printf 'options gen2\nrequire greeter/greeter\n[export]\ndef main() {\n    print("{greet()}\\n")\n}\n' > "$PROJECT/main.das"
run_check "lint.exe sees a project module" bash -c \
    "set -o pipefail; cd '$PROJECT' && '$BUNDLE/bin/lint${TOOL_EXE_SUFFIX}' main.das --no-cache | tee '$LOG.lint' \
     && grep -Eq '^1 files, 0 issue\(s\), 0 error\(s\)\$' '$LOG.lint'"
rm -rf "$PROJECT"

# Two prebuilt tools past --help. dastest.exe must compile and run a shipped suite
# (isolated mode also spawns its own workers); lint.exe over daslib must resolve every
# module native path - its summary line grows ", N skipped" when it cannot, so the
# anchored grep is what rejects a skip.
run_check "dastest.exe runs a shipped suite" bash -c \
    "set -o pipefail; '$BUNDLE/bin/dastest${TOOL_EXE_SUFFIX}' --test utils/common/tests --isolated-mode | tee '$LOG.suite' \
     && grep -Eq '^[1-9][0-9]* tests, [1-9][0-9]* passed, 0 failed, 0 errors' '$LOG.suite'"
# LINT026 is armed: the arch-extract excerpts installed beside daslib make every shipped
# [arch] citation resolve, and every excerpt anchor is cited - this run doubles as the
# bundle's citation-closure check for daslib.
run_check "lint.exe lints daslib, no skips" bash -c \
    "set -o pipefail; '$BUNDLE/bin/lint${TOOL_EXE_SUFFIX}' daslib | tee '$LOG.lint' \
     && grep -Eq '^[0-9]+ files, 0 issue\(s\), 0 error\(s\)\$' '$LOG.lint'"

# The same closure check for every module that ships an arch-extract excerpt: a lint run
# on the document arms its folder's [arch] passes without compiling any .das - every
# citation in the walked tree must resolve, and every excerpt anchor must be cited. One
# run per module: the shallowest excerpt's folder walk covers the module subtree.
if [[ -d "$BUNDLE/modules" ]]; then
    for mod_dir in "$BUNDLE"/modules/*/; do
        arch_md=$(find "$mod_dir" -maxdepth 3 -name 'ARCHITECTURE*.md' 2>/dev/null \
            | awk '{ print gsub("/","/"), $0 }' | sort -n | head -1 | cut -d' ' -f2-)
        [[ -n "$arch_md" ]] || continue
        run_check "arch citations resolve: ${arch_md#"$BUNDLE"/}" bash -c \
            "set -o pipefail; '$BUNDLE/bin/lint${TOOL_EXE_SUFFIX}' '$arch_md' | tee '$LOG.lint' \
             && grep -Eq ', 0 issue\(s\), 0 error\(s\)\$' '$LOG.lint'"
    done
fi

# Shipped skills must not send the reader to a path the bundle does not contain.
# This is the same class of bug as the utils/mcp/setup.das miss: the docs promised
# something the install rules never delivered. Here the right answer is the reverse
# of shipping it -- src/ and tests/ are never going in the bundle -- so the check
# demands the line be marked `repo-only` instead.
echo
echo "Shipped skills:"
# nothing lists the shipped skills any more, so the layout itself is the assertion
printf '  %-30s ' "bundle layout"
if [[ -f "$BUNDLE/skills/daslang/SKILL.md" && -d "$BUNDLE/skills/daslang/references" \
      && ! -d "$BUNDLE/skills/internal" && -f "$BUNDLE/.claude/skills/daslang/SKILL.md" \
      && -f "$BUNDLE/REVIEW_COMMON.md" && -f "$BUNDLE/ARCHITECTURE_COMMON.md" \
      && -f "$BUNDLE/.claude/agents/dragon.md" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL (need skills/daslang/{SKILL.md,references/}, .claude/skills/daslang, .claude/agents/dragon.md, REVIEW_COMMON.md, ARCHITECTURE_COMMON.md, and no skills/internal/)"
    FAIL=$((FAIL + 1))
fi
# Repo-internal rule/record documents never ship: REVIEW.md / REVIEW.das,
# ARCHITECTURE.md, MASTERPLAN logs, followup/perf/profile ledgers.
# The exceptions: REVIEW_COMMON.md and ARCHITECTURE_COMMON.md at the bundle root
# (adopting repos vendor from them), and arch-extract excerpts - generated
# ARCHITECTURE*.md recognized by their first-line banner.
printf '  %-30s ' "no internal rule/record docs"
REVIEW_LEAKS=$(find "$BUNDLE" \( -name "REVIEW*.md" -o -name "REVIEW*.das" \
    -o -name "ARCHITECTURE*.md" -o -name "MASTERPLAN*.md" \
    -o -name "PERF_LEDGER.md" -o -name "PROFILE.md" -o -name "THINKING.md" \
    -o -name "followup_*.md" \) \
    ! -path "$BUNDLE/REVIEW_COMMON.md" ! -path "$BUNDLE/ARCHITECTURE_COMMON.md" 2>/dev/null)
if [[ -n "$REVIEW_LEAKS" ]]; then
    REVIEW_LEAKS=$(echo "$REVIEW_LEAKS" | while IFS= read -r f; do
        head -c 64 "$f" 2>/dev/null | grep -q '^<!-- GENERATED by arch-extract' || echo "$f"
    done)
fi
if [[ -z "$REVIEW_LEAKS" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL (internal rule/record docs in bundle):"
    echo "$REVIEW_LEAKS" | sed 's/^/    /'
    FAIL=$((FAIL + 1))
fi
# No installed file may name a utils/internal/ path -- that class is a shipped
# tutorial/scaffold invoking a tool the bundle does not carry (found live: the
# AOT integration scaffolds). skills/ is excluded here: its own gate above owns
# skills content, with repo-only marker semantics this raw grep cannot honor.
# setup.das is excluded: it PROBES for the in-repo das-herd behind an exists-check, so
# the literal is functional and inert in a bundle.
# CHANGELIST.md is excluded: release history legitimately NAMES the utils/internal
# split; prose there is documentation, not a reference that can dangle.
printf '  %-30s ' "no utils/internal references"
INTERNAL_REFS="$(grep -rIl 'utils/internal' "$BUNDLE" --exclude-dir=skills --exclude=setup.das --exclude=CHANGELIST.md 2>/dev/null || true)"
if [[ -z "$INTERNAL_REFS" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL"
    printf '%s\n' "$INTERNAL_REFS" | sed 's/^/    /'
    FAIL=$((FAIL + 1))
fi

# Every third-party notice the release-module set (plus the default-on modules)
# installs must land at the bundle root -- an install rule silently not firing
# (module off, typo'd RENAME, guard block not entered) is exactly the failure
# mode that left shipped binaries without their notices.
printf '  %-30s ' "third-party licenses present"
MISSING_LICENSES=""
LICENSES=(URIPARSER DAG_NOISE VEC_MATH FMT FAST_FLOAT LUAU GLTF_SAMPLE_ASSETS
          HV OPENSSL LLVM Z3 MINIAUDIO OPENMPT CIPIC PUGIXML GLFW
          IMGUI FREETYPE MD4C JETBRAINS_MONO KHRONOS_GL
          TREE_SITTER TREE_SITTER_ICU TREE_SITTER_C
          TREE_SITTER_CPP TREE_SITTER_MARKDOWN CLIP MINFFT SPIRV_HEADERS
          STB DROID_SANS_MONO MESHOPTIMIZER)
# dasVulkan's notices ship with the module, and a bundle built without it carries neither.
[[ -d "$BUNDLE/modules/dasVulkan" ]] && LICENSES+=(VULKAN_HEADERS VOLK)
for lic in "${LICENSES[@]}"; do
    [[ -f "$BUNDLE/$lic.LICENSE" ]] || MISSING_LICENSES="$MISSING_LICENSES $lic"
done
[[ -f "$BUNDLE/LICENSE" ]] || MISSING_LICENSES="$MISSING_LICENSES <root>"
if [[ -z "$MISSING_LICENSES" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL (missing:$MISSING_LICENSES)"
    FAIL=$((FAIL + 1))
fi

# Soundfonts are fetched locally by devs and carry unclear redistribution terms;
# the examples install excludes them by PATTERN, and this is that rule's
# inverted control -- delete the PATTERN and this goes red on any box holding one.
printf '  %-30s ' "no soundfonts in bundle"
SF2_LEAKS=$(find "$BUNDLE" -iname '*.sf2' 2>/dev/null)
if [[ -z "$SF2_LEAKS" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL (soundfonts in bundle):"
    echo "$SF2_LEAKS" | sed 's/^/    /'
    FAIL=$((FAIL + 1))
fi

printf '  %-30s ' "references resolve in bundle"
# Skipped rather than failed when no python3 is present, so this script stays
# runnable on a bare box.
# Probe by EXECUTING, not by `command -v`: Windows ships a WindowsApps python3 shim that
# is on PATH but exits "Permission denied" until the user installs from the Store.
PY=""
for cand in python3 python py; do
    if "$cand" -c "pass" >/dev/null 2>&1; then PY="$cand"; break; fi
done
if [[ -z "$PY" ]]; then
    echo "SKIP (no python3)"
elif SKILL_REFS="$("$PY" "$CI_DIR/check_shipped_skills.py" "$BUNDLE" 2>&1)"; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL"
    printf '%s\n' "$SKILL_REFS" | sed 's/^/    /'
    FAIL=$((FAIL + 1))
fi

echo
echo "==================================================================="
if [[ $FAIL -eq 0 ]]; then
    echo "ALL OK ($PASS checks passed)"
    exit 0
else
    echo "FAILED: $FAIL failure(s), $PASS pass(es)"
    exit 1
fi
