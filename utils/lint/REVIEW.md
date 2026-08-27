# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that removes `lint` from `DAS_UTILS_SHIPPED_EXES` in `utils/CMakeLists.txt` is a
defect.** The removal drops the prebuilt lint exe from the release bundle, and the gate
cannot see a removed entry.

**A diff that shrinks the set of rule ids `REVIEW.das` (beside this file) scans is a
defect** - whether by editing the gate or by deleting an id's last scannable spelling. The
gate checks that every emitted rule id has a fixture and a `doc/source/reference/language/lint.rst`
(repo root) section, so a shrunk set retires those checks with no warning.

**A diff that makes a file emit a rule id the gate `REVIEW.das` (beside this file) does not
scan adds that file to the gate's module set, in the same change.**
