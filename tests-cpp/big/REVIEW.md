# tests-cpp/big Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/writing_cpp_tests.md`.

**A diff that leaves a test carrying the `big` ctest label - touching its sources, the
`CMakeLists.txt` that registers it, or its labels - says in the PR that the test ran and passed
on the author's machine: the command (`ctest -L big` or the test's own binary) and its pass
count.** Per-PR CI runs `ctest -L small` only; `-L big` runs on the nightly and on a manual
full-workflow run, so a PR's own lanes never show that a big-labelled test passes, and
`ctest -L big` on a tree configured without the test's module reports no tests found and still
exits 0.
