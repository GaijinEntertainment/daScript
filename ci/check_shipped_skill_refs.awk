# Fail if a SHIPPED skill cites a repo path that can never exist in an SDK bundle.
#
# Why this exists: install/skills.list ships skills/*.md verbatim into the bundle.
# A skill that says "see src/ast/ast_infer_type.cpp" is a dead end for an SDK reader
# (and for Claude running in an SDK install) because src/ is not shipped and never
# will be. The fix is never to ship src/ -- it is to say so, so the reader skips the
# line instead of hunting for a path that is not there.
#
# The escape hatch is the token `repo-only`, honored at two scopes:
#   * in a HEADING  -> exempts that section, and the exemption RESETS at the next
#                      heading of any level. A trailing-marker rule would leak the
#                      exemption through the whole rest of the file, which is wrong
#                      for files like cpp_integration.md where a repo-internals
#                      section sits in the middle.
#   * on a LINE     -> exempts just that line, for one-off mentions in prose.
#
# Fenced code blocks are skipped entirely: they quote tool output and source, and
# that text must stay verbatim (e.g. a compiler warning that literally prints a
# doc/source path).
#
# NOT matched yet: tests/. That class is still being sorted into inline-the-pattern
# (where the snippet is the payload) vs quarantine. Add tests/ to the pattern below
# when that lands.
#
# Usage:  awk -f ci/check_shipped_skill_refs.awk <bundle>/skills/*.md

/^[[:space:]]*```/ { fence = !fence; next }
fence               { next }

/^#+[[:space:]]/ {
    exempt = (tolower($0) ~ /repo-only/) ? 1 : 0
    next
}

exempt                      { next }
tolower($0) ~ /repo-only/   { next }

{
    line = $0
    while (match(line, /(^|[^A-Za-z0-9_.\/-])(src\/[A-Za-z0-9_.\/+-]*[A-Za-z0-9_+-]|benchmarks\/[A-Za-z0-9_.\/+-]*[A-Za-z0-9_+-]|modules\/[A-Za-z0-9_-]+\/(src|tests|tutorial|harness)[A-Za-z0-9_.\/+-]*)/)) {
        hit = substr(line, RSTART, RLENGTH)
        sub(/^[^A-Za-z0-9_]/, "", hit)
        printf "  %s:%d: %s\n", FILENAME, FNR, hit
        bad = 1
        line = substr(line, RSTART + RLENGTH)
    }
}

END {
    if (bad) {
        print ""
        print "  A shipped skill cites a path that is not in the bundle."
        print "  Either drop the path, or mark it: add 'repo-only' to the line,"
        print "  or to the heading of a section that is entirely repo internals."
        exit 1
    }
}
