# dastest Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff under this folder applies `utils/REVIEW.md` (repo root) too** - dastest is a shipped
tool: `CMakeLists.txt` (repo root) installs `dastest/dastest.das`.

**A test of dastest itself, and any file it spawns, wherever the diff puts it, answers to the
`tests/` subfolder's checklist.** A test of dastest itself is a `[test]` file that asserts on
what this folder's own `.das` modules do - not one that only uses dastest to run.

**A diff that adds, renames, or removes a public function in `review_gate.das` updates the
`review_gate.das` section of `README.md` in the same change.**

**A diff that adds a flag dastest accepts - a `DastestArgs` field in `dastest_clargs.das` or a
raw-argv check in `dastest.das` - or changes an existing flag's name, default, or doc text, adds
or updates that flag's bullet in `README.md` in the same change -
under `Internal arguments` when only dastest passes the flag, to its own worker processes; under
`dastest.das arguments` otherwise.**

**A diff that adds a test of dastest itself puts it under `tests/` (this folder) with no
registration** - the extended lane runs that whole directory on every PR.
