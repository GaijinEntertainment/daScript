#!/usr/bin/env bash
# Parses every tracked gen2 .das file with the tree-sitter-daslang grammar and fails when a parse has an error. A file
# whose first "options gen2" line says "options gen2 = false" is gen1 and is left out, and so is each file that
# ci/tree_sitter_compiler_rejected_sources.txt lists. Run it from the root of the repository, with the tree-sitter CLI
# on PATH.
set -euo pipefail

readonly GRAMMAR='tree-sitter-daslang'
readonly REJECTED='ci/tree_sitter_compiler_rejected_sources.txt'
readonly TIMEOUT_MICROSECONDS=10000000
# detectGen2Syntax (src/ast/ast_parse.cpp) also skips comments and strings and matches mid-line; this job has no
# daslang binary to run it.
readonly GEN2_OPTION=$'^(\xef\xbb\xbf)?[[:space:]]*options[[:space:]]+gen2([^_[:alnum:]]|$)'
readonly GEN2_OPT_OUT='options[[:space:]]+gen2[[:space:]=]*false'

work=''
cleanup() {
  [[ -z "${work}" ]] || rm -rf "${work}"
}

main() {
  work=$(mktemp -d)
  trap cleanup EXIT

  git ls-files -- '*.das' | sort > "${work}/all"
  { git grep -m1 -E -e "${GEN2_OPTION}" -- '*.das' || true; } \
    | sed -n -E "s/^([^:]*):.*${GEN2_OPT_OUT}.*/\\1/p" | sort > "${work}/gen1"
  sed -e '/^#/d' -e '/^[[:space:]]*$/d' "${REJECTED}" | sort > "${work}/rejected"

  local untracked
  untracked=$(comm -13 "${work}/all" "${work}/rejected")
  if [[ -n "${untracked}" ]]; then
    printf '%s lists files that git does not track:\n%s\n' "${REJECTED}" "${untracked}" >&2
    exit 1
  fi

  comm -23 "${work}/all" "${work}/gen1" | comm -23 - "${work}/rejected" > "${work}/paths"
  printf 'parsing %s files\n' "$(wc -l < "${work}/paths" | tr -d ' ')"
  XDG_CACHE_HOME="${PWD}/.cache/tree-sitter-parse-sources" tree-sitter parse --grammar-path "${GRAMMAR}" \
    --paths "${work}/paths" --quiet --stat --timeout "${TIMEOUT_MICROSECONDS}"
}

main "$@"
