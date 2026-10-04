# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A dasImgui test file - a `.das` file that requires an `imgui/*` module and is run as a test,
by dastest (a `[test]` function) or by a CTest row in this folder's `CMakeLists.txt` - lives
under `modules/dasImgui/tests`; a diff that adds or changes one anywhere else is a defect.**

**A dasImgui test file, wherever the diff puts it, also answers to the `tests/` subfolder's
checklist (`modules/dasImgui/tests/REVIEW.md`).**

**A diff that changes any non-`.md` file under this folder, beyond comments, runs the test suite
on the author's host OS before the PR: `preflight --only imgui`.**

**A diff that flips the `DAS_IMGUI_WASM_RELAXED_SIMD` default to ON, or appends `-mrelaxed-simd`
to `IMGUI_WASM_FLAGS` unconditionally, is a defect.** Safari and every iOS browser refuse a whole
module carrying one relaxed opcode, so the page fails to load rather than running slower.

**A diff that changes a `-m` feature flag in this folder's `CMakeLists.txt` `IMGUI_WASM_FLAGS`
makes the matching change to `web/CMakeLists.txt`'s `add_compile_options` in the same change.**
Neither build inherits the other's flags.
