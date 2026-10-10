#!/usr/bin/env bash
set -euo pipefail

readonly EX_USAGE=64
readonly EX_DATAERR=65
readonly COMMIT_TRAILER='daScript-Commit'
readonly MERGE_TRAILER='daScript-Merge'

prefix='tree-sitter-daslang'
source_slug='GaijinEntertainment/daScript'

usage() {
  cat >&2 <<EOF
Usage: ${0##*/} [--prefix DIR] [--source-slug OWNER/REPO] SOURCE_REF MIRROR_REF

Copies onto MIRROR_REF each change that the first-parent history of
SOURCE_REF makes to DIR, and prints the commit that the mirror branch must
point to. Run it in a repository that holds both histories.

A merge whose branch started where DIR had its state before the merge is
copied commit by commit; every other change becomes one commit. Each copy
keeps the author, the committer, and the message of its source commit, and
records the source commit in a ${COMMIT_TRAILER} trailer.

Options:
  --prefix DIR              the directory of the mirrored tree in
                            SOURCE_REF (default: ${prefix})
  --source-slug OWNER/REPO  the repository that a "#N" reference in a
                            copied message points to
                            (default: ${source_slug})
EOF
  exit "${EX_USAGE}"
}

die() {
  printf '%s: %s\n' "${0##*/}" "$1" >&2
  exit "${2:-1}"
}

prefix_tree() {
  git rev-parse --quiet --verify "$1:${prefix}" 2>/dev/null || true
}

trailer_value() {
  git log -1 --format="%(trailers:key=$2,valueonly)" "$1" | head -n 1
}

newest_commit_with_tree() {
  local source="$1" tree="$2" oid rev
  while read -r oid rev; do
    if [[ "${oid}" == "${tree}" ]]; then
      printf '%s\n' "${rev}"
      return 0
    fi
  done < <(
    git rev-list --first-parent "${source}" \
      | awk -v p="${prefix}" '{ print $0 ":" p " " $0 }' \
      | git cat-file --batch-check='%(objectname) %(rest)'
  )
  return 1
}

link_references() {
  sed -E "s~(^|[[:space:](])#([0-9]+)~\\1${source_slug}#\\2~g"
}

copy_commit() {
  local rev="$1" tree="$2" parent="$3" mainline="$4"
  local identity an ae ad cn ce cd message
  local -a trailers
  identity=$(git log -1 --date=raw \
    --format='%an%n%ae%n%ad%n%cn%n%ce%n%cd' "${rev}")
  {
    IFS= read -r an
    IFS= read -r ae
    IFS= read -r ad
    IFS= read -r cn
    IFS= read -r ce
    IFS= read -r cd
  } <<< "${identity}"
  message=$(git log -1 --format=%B "${rev}" | link_references)
  trailers=(--trailer "${COMMIT_TRAILER}: ${rev}")
  if [[ "${mainline}" != "${rev}" ]]; then
    trailers+=(--trailer "${MERGE_TRAILER}: ${mainline}")
  fi
  printf '%s\n' "${message}" \
    | git interpret-trailers "${trailers[@]}" \
    | GIT_AUTHOR_NAME="${an}" GIT_AUTHOR_EMAIL="${ae}" \
      GIT_AUTHOR_DATE="${ad}" GIT_COMMITTER_NAME="${cn}" \
      GIT_COMMITTER_EMAIL="${ce}" GIT_COMMITTER_DATE="${cd}" \
      git commit-tree "${tree}" -p "${parent}"
}

main() {
  local -a args=()
  while (( $# > 0 )); do
    case "$1" in
      --prefix)
        [[ -n "${2:-}" ]] || die '--prefix needs a directory' "${EX_USAGE}"
        prefix="$2"
        shift 2
        ;;
      --source-slug)
        [[ "${2:-}" =~ ^[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+$ ]] \
          || die '--source-slug needs OWNER/REPO' "${EX_USAGE}"
        source_slug="$2"
        shift 2
        ;;
      -h|--help) usage ;;
      --) shift; args+=("$@"); break ;;
      -*) die "unknown option $1" "${EX_USAGE}" ;;
      *) args+=("$1"); shift ;;
    esac
  done
  (( ${#args[@]} == 2 )) || usage

  local source tip tip_tree state
  source=$(git rev-parse --quiet --verify "${args[0]}^{commit}") \
    || die "${args[0]} is not a commit" "${EX_USAGE}"
  tip=$(git rev-parse --quiet --verify "${args[1]}^{commit}") \
    || die "${args[1]} is not a commit" "${EX_USAGE}"
  tip_tree=$(git rev-parse "${tip}^{tree}")

  state=$(trailer_value "${tip}" "${MERGE_TRAILER}")
  if [[ -z "${state}" ]]; then
    state=$(trailer_value "${tip}" "${COMMIT_TRAILER}")
  fi
  if [[ -z "${state}" ]]; then
    state=$(newest_commit_with_tree "${source}" "${tip_tree}") \
      || die "no first-parent commit of ${args[0]} has the tree of ${tip}" \
        "${EX_DATAERR}"
    printf 'start from %s, which has the tree of %s\n' "${state}" "${tip}" >&2
  fi
  git merge-base --is-ancestor "${state}" "${source}" \
    || die "${state}, the last copied commit, is not in ${args[0]}" \
      "${EX_DATAERR}"
  [[ "$(prefix_tree "${state}")" == "${tip_tree}" ]] \
    || die "the tree of ${tip} differs from ${prefix} at ${state}" \
      "${EX_DATAERR}"

  local revs rev tree first_tree base steps step step_tree previous
  local -a parents
  revs=$(git rev-list --first-parent --reverse "${state}..${source}")
  while read -r rev <&3; do
    [[ -n "${rev}" ]] || continue
    read -r -a parents <<< "$(git log -1 --format=%P "${rev}")"
    tree=$(prefix_tree "${rev}")
    first_tree=$(prefix_tree "${parents[0]}")
    if [[ "${tree}" == "${first_tree}" ]]; then
      continue
    fi
    [[ -n "${tree}" ]] || die "${rev} has no ${prefix}" "${EX_DATAERR}"

    base=''
    if (( ${#parents[@]} == 2 )); then
      base=$(git merge-base "${parents[0]}" "${parents[1]}")
    fi
    if [[ -n "${base}" && "$(prefix_tree "${base}")" == "${first_tree}" ]]
    then
      previous="${first_tree}"
      steps=$(git rev-list --first-parent --reverse "${base}..${parents[1]}")
      while read -r step <&4; do
        [[ -n "${step}" ]] || continue
        step_tree=$(prefix_tree "${step}")
        if [[ -n "${step_tree}" && "${step_tree}" != "${previous}" ]]; then
          tip=$(copy_commit "${step}" "${step_tree}" "${tip}" "${rev}")
          printf 'copied %s as %s\n' "${step}" "${tip}" >&2
          previous="${step_tree}"
        fi
      done 4<<< "${steps}"
      if [[ "${previous}" == "${tree}" ]]; then
        continue
      fi
    fi
    tip=$(copy_commit "${rev}" "${tree}" "${tip}" "${rev}")
    printf 'copied %s as %s\n' "${rev}" "${tip}" >&2
  done 3<<< "${revs}"

  printf '%s\n' "${tip}"
}

main "$@"
